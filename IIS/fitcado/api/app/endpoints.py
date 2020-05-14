# -*- coding: utf-8 -*-
import base64
import datetime
import io
import logging
import os
import secrets

import redis
from connexion.exceptions import Unauthorized
from fitcado.api.app.common_function import compare_password, get_user, hash_password
from fitcado.database import SESSION, commit_session, models
from flask import jsonify, make_response, request, send_file

logging.basicConfig(level=logging.INFO)
LOG = logging.getLogger("endpoints")
REDIS = redis.StrictRedis(port=6379, host="redis", decode_responses=True)

"""Endpoints documentation.

Example:
    Examples can be given using either the ``Example`` or ``Examples``
    sections. Sections support any reStructuredText formatting, including
    literal blocks::

        $ python example_google.py

Section breaks are created by resuming unindented text. Section breaks
are also implicitly created anytime a new section starts.

Attributes:
    module_level_variable1 (int): Module level variables may be documented in
        either the ``Attributes`` section of the module docstring, or in an
        inline docstring immediately following the variable.

        Either form is acceptable, but the two should not be mixed. Choose
        one convention to document module level variables and be consistent
        with it.


Todo:
    * For module TODOs
    * You have to also use ``sphinx.ext.todo`` extension

.. _Google Python Style Guide:
   http://google.github.io/styleguide/pyguide.html

"""


def check_session(cookies):
    session = cookies.get("FITSESSIONID", None)
    if session is None:
        raise Unauthorized("session cookie missing")
    user_id = REDIS.get(session)
    if user_id is None:
        raise Unauthorized("wrong session cookie")

    return user_id


def ping():
    """Pinging back and example of function with types documented in the docstring.

    `PEP 484`_ type annotations are supported. If attribute, parameter, and
    return types are annotated according to `PEP 484`_, they do not need to be
    included in the docstring:

    Args:
       param1 (int): The first parameter.
       param2 (str): The second parameter.

    Returns:
       bool: The return value. True for success, False otherwise.

    .. _PEP 484:
       https://www.python.org/dev/peps/pep-0484/

    """
    return {"ping": "pong"}


def authenticate():
    session = request.cookies.get("FITSESSIONID", None)
    if session is None:
        raise Unauthorized("session cookie missing")
    user_id = REDIS.get(session)
    if user_id is None:
        raise Unauthorized("wrong session cookie")
    user = SESSION.query(models.User).filter(models.User.id == user_id).one()

    return jsonify({"userName": user.nickname, "useRight": 2 if user.role == "admin" else 1}), 200


def login(body):
    user = SESSION.query(models.User).filter(models.User.nickname == body["username"]).one_or_none()
    if user is None:
        return jsonify({"error": "Unknown user"}), 404
    else:
        if user.active:
            if compare_password(user.password, body["password"]):
                session = secrets.token_urlsafe(32)
                REDIS.set(session, user.id, ex=60 * 30)
                return (
                    jsonify({"userName": user.nickname, "useRight": 2 if user.role == "admin" else 1}),
                    200,
                    {"Set-Cookie": f"FITSESSIONID={session}; Max-Age={60 * 30}; Path=/"},
                )
            else:
                return jsonify({"error": "password"}), 401
        else:
            return jsonify({"error": "User is not active"}), 403


def change_password(body):
    user_id = check_session(request.cookies)
    user_session = SESSION.query(models.User).filter(models.User.id == user_id)

    user = user_session.one_or_none()

    if not user:
        return jsonify({"error": "Internal server error"}), 500

    if not compare_password(user.password, body["old_password"]):
        return jsonify({"error": "Unauthorized to change password"}), 401

    new_pwd = hash_password(body["new_password"])
    user_session.update({"password": new_pwd})
    if commit_session(SESSION, raise_exc=False):
        return 200

    return jsonify({"error": "Internal server error"}), 500


def change_profile_image(body):
    user_id = check_session(request.cookies)
    user = SESSION.query(models.User).filter(models.User.id == user_id)

    if not user.one_or_none():
        return jsonify({"error": "Internal server error"}), 500

    user.update({"image_path": body["image_path"]})

    if commit_session(SESSION, raise_exc=False):
        return 200

    return jsonify({"error": "Internal server error"}), 500


def add_exercise(body):
    if SESSION.query(models.Exercise).filter(models.Exercise.name == body["name"]).one_or_none():
        return jsonify({"error": "Exercise with same name is already in DB."}), 409

    exercise = models.Exercise(**body)
    SESSION.add(exercise)
    if commit_session(SESSION, raise_exc=False):
        return 200

    return jsonify({"error": "Failed to add exercise. Please try again."}), 400


def get_exercise_all():
    exercises = SESSION.query(models.Exercise).all()

    result = [
        {
            "description": exercise.description,
            "type": exercise.type,
            "muscles": exercise.muscles,
            "multi_joint": exercise.multi_joint,
            "name": exercise.name,
            "id": exercise.id,
        }
        for exercise in exercises
    ]

    return jsonify({"items": result})


def add_user(body):
    body["password"] = hash_password(body["password"])

    user = get_user(body["nickname"]).one_or_none()
    if user:
        return jsonify({"code": 409, "error": "User with this nickname already exists."}), 409

    user = models.User(**body)
    user.active = True
    SESSION.add(user)

    if commit_session(SESSION, raise_exc=True):
        session = secrets.token_urlsafe(32)
        REDIS.set(session, user.id, ex=60 * 30)
        return (
            jsonify({"session": session}),
            200,
            {"Set-Cookie": f"FITSESSIONID={session}; Max-Age={60 * 30}; Path=/"},
        )

    return jsonify({"error": "Failed to register. Please try again"}), 500


def deactivate_user(user_id):
    admin_id = check_session(cookies=request.cookies)
    if not admin_id:
        return jsonify({"error": "Not authorized"}), 401
    admin = SESSION.query(models.User).filter(models.User.id == admin_id).one_or_none()
    if not admin:
        return jsonify({"error": "User not exist"}), 404
    if not admin.role == "admin":
        return jsonify({"error": "User not admin"}), 401
    user = SESSION.query(models.User).filter(models.User.id == user_id).one_or_none()
    if not user:
        return jsonify({"error": "User not exist"}), 404
    user.active = False
    if not commit_session(SESSION):
        return jsonify({"error": "Change was not done"}), 404
    else:
        return "User deactivated"

def activate_user(user_id):
    admin_id = check_session(cookies=request.cookies)
    if not admin_id:
        return jsonify({"error": "Not authorized"}), 401
    admin = SESSION.query(models.User).filter(models.User.id == admin_id).one_or_none()
    if not admin:
        return jsonify({"error": "User not exist"}), 404
    if not admin.role == "admin":
        return jsonify({"error": "User not admin"}), 401
    user = SESSION.query(models.User).filter(models.User.id == user_id).one_or_none()
    if not user:
        return jsonify({"error": "User not exist"}), 404
    user.active = True
    if not commit_session(SESSION):
        return jsonify({"error": "Change was not done"}), 404
    else:
        return "User deactivated"

def update_user(body):
    user_id = check_session(request.cookies)
    user = SESSION.query(models.User).filter(models.User.id == user_id)
    if not user.one_or_none():
        return jsonify({"code": 500, "error": "User does not exists."}), 500

    user_obj = user.one_or_none()
    user.update(
        {
            "nickname": body["nickname"] if body.get("nickname", None) else user_obj.nickname,
            "name": body["name"] if body.get("name", None) else user_obj.name,
            "email": body["email"] if body.get("email", None) else user_obj.email,
        }
    )

    if commit_session(SESSION, raise_exc=False):
        return 200

    return jsonify({"error": "Failed to update user."}), 500


def user_get():
    user_id = check_session(request.cookies)
    user = SESSION.query(models.User).filter(models.User.id == user_id).one_or_none()
    if not user:
        return jsonify({"code": 500, "error": "User does not exists."}), 500

    trainer_nicknames = []
    for proposal in SESSION.query(models.TrainerPropose).filter(models.TrainerPropose.user_id == user_id).all():
        trainer = SESSION.query(models.Trainer).filter(models.Trainer.id == proposal.trainer_id).one_or_none()
        if not trainer:
            return jsonify({"code": 500, "error": "Failed to load. Please refresh."}), 500

        trainer = SESSION.query(models.User).filter(models.User.id == trainer.user_id).one_or_none()
        if not trainer:
            return jsonify({"code": 500, "error": "Failed to load. Please refresh."}), 500

        trainer_nicknames.append(trainer.nickname)

    return jsonify(
        {
            "items": {
                "nickname": user.nickname,
                "name": user.name,
                "birthdate": user.birthdate,
                "role": user.role,
                "email": user.email,
            },
            "trainers": trainer_nicknames,
        }
    )

def get_users():
    admin_id = check_session(cookies=request.cookies)
    if not admin_id:
        return jsonify({"error": "Not authorized"}), 401
    admin = SESSION.query(models.User).filter(models.User.id == admin_id).one_or_none()
    if not admin:
        return jsonify({"error": "User not exist"}), 404
    if not admin.role == "admin":
        return jsonify({"error": "User not admin"}), 401
    users = SESSION.query(models.User).all()
    response = [
        {"nickname": user.nickname, "email": user.email, "active": user.active , "id": user.id} for user in users
    ]
    return jsonify({"items":response})

def submit_body_size(body):
    user_id = check_session(request.cookies)

    body.update({"user_id": user_id})

    body_size = models.BodySize(**body)
    SESSION.add(body_size)

    if commit_session(SESSION, raise_exc=False):
        return 200

    return jsonify({"error": "Failed to submit body size."}), 500


def submit_body_weight(body):
    user_id = check_session(request.cookies)

    body.update({"user_id": user_id})

    body_weight = models.BodyWeight(**body)
    SESSION.add(body_weight)

    if commit_session(SESSION, raise_exc=False):
        return 200

    return jsonify({"error": "Failed to submit body weight"}), 500


def get_user_body_size():
    user_id = check_session(request.cookies)

    body_size = SESSION.query(models.BodySize).filter(models.BodySize.user_id == user_id).all()

    result = [
        {"biceps": size.biceps, "waist": size.waist, "neck": size.waist, "date": size.inserted, "id": size.id}
        for size in body_size
    ]

    return jsonify({"items": result})


def delete_body_size(body_size_id):
    user_id = check_session(request.cookies)
    if not user_id:
        return jsonify({"error": "Unauthorized"}), 401

    body_sizes = SESSION.query(models.BodySize).filter(models.BodySize.user_id == user_id).all()
    if not body_sizes:
        return jsonify({"error": "Found none body sizes for given user"}), 404

    for body_size in body_sizes:
        if body_size.id == body_size_id:
            SESSION.delete(body_size)
            commit_session(SESSION)
            return 200
    return jsonify({"error": "Found none body size with given id"}), 404


def delete_body_weight(body_weight_id):
    user_id = check_session(request.cookies)
    if not user_id:
        return jsonify({"error": "Unauthorized"}), 401

    body_weights = SESSION.query(models.BodyWeight).filter(models.BodyWeight.user_id == user_id).all()
    if not body_weights:
        return jsonify({"error": "Found none body weight for given user"}), 404

    for body_weight in body_weights:
        if body_weight.id == body_weight_id:
            SESSION.delete(body_weight)
            commit_session(SESSION)
            return 200
    return jsonify({"error": "Found none body weight with given id"}), 404


def get_user_body_weight():
    user_id = check_session(request.cookies)

    body_weight = SESSION.query(models.BodyWeight).filter(models.BodyWeight.user_id == user_id).all()

    result = [{"weight": weight.weight, "date": weight.inserted, "id": weight.id} for weight in body_weight]

    return jsonify({"items": result})


def add_training(body):
    if SESSION.query(models.Training).filter(models.Training.name == body["training"]["name"]).one_or_none():
        return jsonify({"code": 409, "error": "Training with specified name already exists."}), 409

    training = models.Training(
        name=body["training"]["name"], description=body["training"]["description"], type=body["training"]["type"]
    )

    for ser in body["series"]:
        exercise_name = ser["exercise"]["name"]
        exercise = SESSION.query(models.Exercise).filter(models.Exercise.name == exercise_name).one_or_none()
        if not exercise:
            return jsonify({"code": 404, "error": "Exercise with specified name does not exists."}), 409

        serie = (
            SESSION.query(models.Series)
            .filter(
                models.Series.break_duration_in_seconds == ser["break_duration_in_seconds"],
                models.Series.repetitions == ser["repetitions"],
                models.Series.exercise_duration_in_seconds == ser["exercise_duration_in_seconds"],
                models.Series.weight == ser["weight"],
                models.Series.exercise_id == exercise.id,
            )
            .one_or_none()
        )

        if not serie:
            serie = models.Series(
                exercise_duration_in_seconds=ser["exercise_duration_in_seconds"],
                repetitions=ser["repetitions"],
                break_duration_in_seconds=ser["break_duration_in_seconds"],
                weight=ser["weight"],
                exercise_id=exercise.id,
            )

        training.series.append(serie)

    SESSION.add(training)
    if commit_session(SESSION, raise_exc=False):
        return 200
    return jsonify({"error": "Failed to add training."}), 500


def get_trainings():
    trainings = SESSION.query(models.Training).all()
    if not trainings:
        return jsonify({"items": []})

    result = {"items": []}
    for training in trainings:
        result["items"].append(
            {"name": training.name, "description": training.description, "type": training.type, "id": training.id}
        )

    LOG.info(result)
    return jsonify(result)


def delete_training(training_id):
    user_id = check_session(cookies=request.cookies)
    if not user_id:
        return jsonify({"error": "Not authorized"}), 401
    user = SESSION.query(models.User).filter(models.User.id == user_id).one_or_none()
    if not user:
        return jsonify({"error": "User not exist"}), 404
    if not user.role == "admin":
        return jsonify({"error": "User not admin"}), 401
    training = SESSION.query(models.Training).filter(models.Training.id == training_id).one_or_none()
    if not training:
        return jsonify({"error": "Training not exist"}), 404
    SESSION.delete(training)
    if not commit_session(SESSION):
        return jsonify({"error": "Change was not done"}), 404
    else:
        return "Training deleted"


def delete_exercise(exercise_id):
    user_id = check_session(cookies=request.cookies)
    if not user_id:
        return jsonify({"error": "Not authorized"}), 401
    user = SESSION.query(models.User).filter(models.User.id == user_id).one_or_none()
    if not user:
        return jsonify({"error": "User not exist"}), 404
    if not user.role == "admin":
        return jsonify({"error": "User not admin"}), 401
    exercise = SESSION.query(models.Exercise).filter(models.Exercise.id == exercise_id).one_or_none()
    if not exercise:
        return jsonify({"error": "Training not exist"}), 404
    SESSION.delete(exercise)
    if not commit_session(SESSION):
        return jsonify({"error": "Change was not done"}), 404
    else:
        return "Exercise deleted"


def get_series(body):
    result = {"items": []}
    for number in body["id"]:
        series = SESSION.query(models.Series).filter(models.Series.id == number).one_or_none()
        if not series:
            return jsonify({"code": 401, "error": "Series with id {} not found".format(number)}), 401

        exercise = SESSION.query(models.Exercise).filter(models.Exercise.id == series.exercise_id).one_or_none()
        if not exercise:
            return jsonify({"code": 500, "error": "Exercise witth id {} not found".format(series.exercise_id)}), 500

        result["items"].append(
            {
                "series": {
                    "repetitions": series.repetitions,
                    "weight": series.weight,
                    "break_duration": series.break_duration_in_seconds,
                    "exercise_duration": series.exercise_duration_in_seconds,
                },
                "exercise": {
                    "description": exercise.description,
                    "type": exercise.type,
                    "muscles": exercise.muscles,
                    "body_part": exercise.body_part,
                    "multi_joint": exercise.multi_joint,
                },
            }
        )

    return result


def update_series(body):
    def get_ids(s_list):
        return (s.id for s in s_list)

    series = SESSION.query(models.Series).filter(models.Series.id == body["series_id"])
    series_obj = series.one_or_none()
    if not series_obj:
        return jsonify({"code": 401, "error": "Series not found."}), 401

    trainings = SESSION.query(models.Training).filter(body["training_id"] != models.Training.id).all()
    trainings_first = next(filter(None, [t if series_obj.id in get_ids(t.series) else None for t in trainings]), None)

    if trainings_first:
        modified_training = (
            SESSION.query(models.Training).filter(models.Training.id == body["training_id"]).one_or_none()
        )
        if not modified_training:
            return jsonify({"code": 401, "error": "Failed to update series."}), 401

        try:
            modified_training.series.remove(series_obj)
        except Exception:
            return jsonify({"code": 409, "error": "Failed to update series."}), 409

        new_series = models.Series(
            repetitions=body["series"]["repetitions"]
            if body["series"].get("repetitions", None)
            else series_obj.repetitions,
            weight=body["series"]["weight"] if body["series"].get("weight", None) else series_obj.weight,
            break_duration_in_seconds=body["series"]["break_duration"]
            if body["series"].get("break_duration")
            else series_obj.break_duration_in_seconds,
            exercise_duration_in_seconds=body["series"]["exercise_duration"]
            if body["series"].get("exercise_duration", None)
            else series_obj.exercise_duration_in_seconds,
            exercise_id=series_obj.exercise_id,
        )
        modified_training.series.append(new_series)
    else:
        series.update(
            {
                "repetitions": body["series"]["repetitions"]
                if body["series"].get("repetitions", None)
                else series_obj.repetitions,
                "weight": body["series"]["weight"] if body["series"].get("weight", None) else series_obj.weight,
                "break_duration_in_seconds": body["series"]["break_duration"]
                if body["series"].get("break_duration")
                else series_obj.break_duration_in_seconds,
                "exercise_duration_in_seconds": body["series"]["exercise_duration"]
                if body["series"].get("exercise_duration", None)
                else series_obj.exercise_duration_in_seconds,
            }
        )

    if commit_session(SESSION, raise_exc=False):
        return 200

    return jsonify({"error": "Failed to update series."}), 500


def add_plan(body):
    user_id = check_session(request.cookies)
    user = SESSION.query(models.User).filter(models.User.id == user_id).one_or_none()
    if not user:
        return jsonify({"code": 501, "error": "User does not exists."}), 501

    plan = models.Plan(
        start_date=datetime.datetime.strptime(body["start_date"], "%Y-%m-%d"),
        end_date=datetime.datetime.strptime(body["end_date"], "%Y-%m-%d"),
        days_in_week=body["days_in_week"],
    )

    train = SESSION.query(models.Training).filter(models.Training.name == body["training"]["name"]).one_or_none()
    if not train:
        return jsonify({"code": 404, "error": "Training {} does not exist.".format(train.name)}), 404
    train.plans.append(plan)
    user.plans.append(plan)

    if commit_session(SESSION, raise_exc=False):
        return ({"code": 200, "status": "success"}), 200
    return jsonify({"error": "Internal server error"}), 500


def add_plan_from_trainer(body):
    trainers_user_id = check_session(request.cookies)
    user_nickname = body["user_nickname"]
    user = SESSION.query(models.User).filter(models.User.nickname == user_nickname).one_or_none()
    if not user:
        return jsonify({"code": 501, "error": "User not in database"}), 501

    trainers_user_id = int(trainers_user_id)
    for users_trainer in user.trainers:
        if users_trainer.user_id == trainers_user_id:
            break
    # trainer is not in user.trainers
    else:
        return jsonify({"code": 401, "error": "You are not training this user"}), 401

    plan = models.Plan(
        start_date=datetime.datetime.strptime(body["start_date"], "%Y-%m-%d"),
        end_date=datetime.datetime.strptime(body["end_date"], "%Y-%m-%d"),
        days_in_week=body["days_in_week"],
    )

    train = SESSION.query(models.Training).filter(models.Training.name == body["training"]["name"]).one_or_none()
    if not train:
        return jsonify({"code": 404, "error": "Training {} does not exist.".format(train.name)}), 404

    train.plans.append(plan)
    user.plans.append(plan)

    if commit_session(SESSION, raise_exc=False):
        return ({"code": 200, "status": "success"}), 200
    return jsonify({"error": "Internal server error"}), 500


def get_plans():
    user_id = check_session(request.cookies)
    user = SESSION.query(models.User).filter(models.User.id == user_id).one_or_none()
    if not user:
        return jsonify({"code": 500, "error": "User does not exists."}), 500

    result = {"items": []}

    for plan in user.plans:
        training = SESSION.query(models.Training).filter(models.Training.id == plan.training_id).one()
        result["items"].append(
            {
                "start_date": plan.start_date,
                "end_date": plan.end_date,
                "days_in_week": plan.days_in_week,
                "id": plan.id,
                "training": {"name": training.name, "type": training.type, "description": training.description},
            }
        )
    return result


def delete_plan(plan_id):
    user_id = check_session(request.cookies)
    user = SESSION.query(models.User).filter(models.User.id == user_id).one_or_none()
    if not user:
        return jsonify({"code": 501, "error": "User does not exists"}), 501
    for plan in user.plans:
        if plan.id == plan_id:
            SESSION.delete(plan)
            if commit_session(SESSION, raise_exc=True):
                return 200
    return jsonify({"code": 404, "error": "Failed to delete plan. Plan not found"}), 404


def connect_user_plan(body):
    user_id = check_session(request.cookies)
    user = SESSION.query(models.User).filter(models.User.id == user_id).one_or_none()
    if not user:
        return jsonify({"code": 500, "error": "User does not exists."}), 500

    plan = SESSION.query(models.Plan).filter(models.Plan.id == body["id"]).one_or_none()
    if not plan:
        return jsonify({"code": 500, "error": "Plan does not exists."}), 500

    user.plans.append(plan)

    if commit_session(SESSION, raise_exc=False):
        return 200
    return jsonify({"error": "Failed to connect user to plan."}), 500


def add_trainer(body):
    user_id = check_session(request.cookies)
    user = SESSION.query(models.User).filter(models.User.id == user_id).one_or_none()
    if not user:
        return jsonify({"code": 500, "error": "User does not exists."}), 500

    # Trainer
    trainers_user_profile = (
        SESSION.query(models.User).filter(models.User.nickname == body["trainer_nickname"]).one_or_none()
    )
    retval = None

    if not trainers_user_profile:
        retval = jsonify({"code": 500, "error": "Trainer not in database"}), 500

    elif trainers_user_profile.id == user.id:
        retval = jsonify({"code": 409, "error": "user and trainer are the same"}), 409

    if retval:
        return retval

    trainer = SESSION.query(models.Trainer).filter(models.Trainer.user_id == trainers_user_profile.id).one_or_none()
    if not trainer:
        return jsonify({"code": 404, "error": "{} is not trainer.".format(trainers_user_profile.nickname)}), 404
    propose = (
        SESSION.query(models.TrainerPropose)
        .filter(models.TrainerPropose.user_id == user_id, models.TrainerPropose.trainer_id == trainer.id)
        .one_or_none()
    )
    if not propose:
        return jsonify({"code": 404, "error": "No proposal found"}), 404
    SESSION.delete(propose)
    if body["accept"]:
        if trainer not in user.trainers:
            user.trainers.append(trainer)

    if commit_session(SESSION):
        return 200

    return jsonify({"error": "Failed to add trainer to user."}), 500


def remove_trainer():
    pass


def delete_trainee(body):
    trainer_id = check_session(request.cookies)
    trainee_nickname = body["trainee_nickname"]
    trainee = SESSION.query(models.User).filter(models.User.nickname == trainee_nickname).one_or_none()
    if not trainee:
        return jsonify({"error": "Trainee does not exist in the system"}), 400
    trainer = SESSION.query(models.Trainer).filter(models.Trainer.user_id == trainer_id).one_or_none()
    if not trainer:
        return jsonify({"error": "You are not in the system, please contact support"}), 400

    # for trainees_trainer in trainee.trainers:
    #     if trainees_trainer.user_id == trainer_id:
    #         trainee.trainers.remove(trainees_trainer)
    trainee.trainers.remove(trainer)

    if commit_session(SESSION, raise_exc=False):
        return jsonify({"code": 200, "status": "success"}), 200
    return jsonify({"error": "Trainee could not be deleted"}), 500


def make_trainer():
    user_id = check_session(request.cookies)
    user = SESSION.query(models.User).filter(models.User.id == user_id).one_or_none()
    if not user:
        return jsonify({"code": 500, "error": "User does not exists."}), 500

    if SESSION.query(models.Trainer).filter(models.Trainer.user_id == user_id).one_or_none():
        return 200

    trainer = models.Trainer(user_id=user_id)
    SESSION.add(trainer)
    if commit_session(SESSION, raise_exc=False):
        return 200
    return jsonify({"error": "Failed to submit for trainer."}), 500


def get_trainer_users():
    user_id = check_session(request.cookies)
    trainer_id = SESSION.query(models.Trainer).filter(models.Trainer.user_id == user_id).one_or_none().id
    users = SESSION.query(models.User).filter(models.User.id != user_id).all()
    my_users_nicknames = []
    for user in users:
        for trainer in user.trainers:
            if trainer_id == trainer.id:
                my_users_nicknames.append(user.nickname)

    result = {"items": my_users_nicknames}
    return result


def trainer_propose(body):
    user_id = check_session(request.cookies)

    trainer = SESSION.query(models.Trainer).filter(models.Trainer.user_id == user_id).one_or_none()
    if not trainer:
        return jsonify({"code": 401, "error": "You are not trainer."}), 401

    user = SESSION.query(models.User).filter(models.User.nickname == body["nickname"]).one_or_none()
    if not user:
        return jsonify({"code": 500, "error": "User does not exists"}), 500

    propose = (
        SESSION.query(models.TrainerPropose)
        .filter(models.TrainerPropose.user_id == user.id, models.TrainerPropose.trainer_id == trainer.id)
        .one_or_none()
    )

    retval = None
    if propose:
        retval = jsonify({"error": "Propose already exists"}), 400

    if trainer in user.trainers:
        retval = jsonify({"error": "You are already training this user"}), 400
    if int(user_id) == user.id:
        retval = jsonify({"error": "You cant propose yourself"}), 400
    if retval:
        return retval

    propose = models.TrainerPropose(user_id=user.id, trainer_id=trainer.id)
    SESSION.add(propose)

    if commit_session(SESSION, raise_exc=False):
        return 200
    return jsonify({"error": "Failed to send proposal."}), 500


def is_trainer():
    user_id = check_session(request.cookies)
    trainer = SESSION.query(models.Trainer).filter(models.Trainer.user_id == user_id).one_or_none()

    if trainer:
        return jsonify({"status": "success"}), 200
    return jsonify({"status": "failure"}), 400


def upload_image(body):
    user_id = check_session(request.cookies)
    with open("/images/{}.jpg".format(user_id), "wb") as outfile:
        outfile.write(body)

    return jsonify({"type": True})


def download_image():
    user_id = check_session(request.cookies)
    dir_path = os.path.dirname("/images/")
    for _, _, files in os.walk(dir_path):
        for file in files:
            logging.info(file)
            if file.startswith(str(user_id)):
                with open("/images/" + file, "rb") as image_file:
                    encoded_string = base64.b64encode(image_file.read())
                    response = make_response(send_file(io.BytesIO(encoded_string), mimetype="image/jpg"))
                    response.headers["Content-Transfer-Encoding"] = "base64"
                    return response

    with open("/images/default.jpg", "rb") as image_file:
        encoded_string = base64.b64encode(image_file.read())
        response = make_response(send_file(io.BytesIO(encoded_string), mimetype="image/jpg"))
        response.headers["Content-Transfer-Encoding"] = "base64"
        return response
