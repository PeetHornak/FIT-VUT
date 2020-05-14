from sqlalchemy import BigInteger, Boolean, Column, DateTime, ForeignKey, Numeric, SmallInteger, String, Table, func
from sqlalchemy.dialects.postgresql import ARRAY
from sqlalchemy.orm import relationship

from . import BASE_MODEL

SERIES_TRAINING_ASSOCIATION = Table(
    "series_training_association",
    BASE_MODEL.metadata,
    Column("series_id", BigInteger, ForeignKey("series.id", ondelete="CASCADE")),
    Column("training_id", BigInteger, ForeignKey("training.id", ondelete="CASCADE")),
)

USER_TRAINER_ASSOCIATION = Table(
    "user_trainer_association",
    BASE_MODEL.metadata,
    Column("user_id", BigInteger, ForeignKey("user.id", ondelete="CASCADE")),
    Column("trainer_id", BigInteger, ForeignKey("trainer.id", ondelete="CASCADE")),
)


class Exercise(BASE_MODEL):
    __tablename__ = "exercise"
    id = Column(BigInteger, primary_key=True)
    name = Column(String, unique=True)
    description = Column(String)
    type = Column(String)
    muscles = Column(ARRAY(String))
    body_part = Column(String)
    multi_joint = Column(Boolean)
    series = relationship("Series", cascade="all,delete")
    inserted = Column(DateTime(timezone=True), server_default=func.now())
    updated = Column(DateTime(timezone=True), onupdate=func.now())


class Series(BASE_MODEL):
    __tablename__ = "series"
    id = Column(BigInteger, primary_key=True)
    exercise_duration_in_seconds = Column(SmallInteger)
    repetitions = Column(SmallInteger)
    break_duration_in_seconds = Column(SmallInteger)
    weight = Column(Numeric)
    exercise_id = Column(BigInteger, ForeignKey("exercise.id"))


class Training(BASE_MODEL):
    __tablename__ = "training"
    id = Column(BigInteger, primary_key=True)
    name = Column(String, unique=True)
    description = Column(String)
    type = Column(String)
    series = relationship("Series", secondary=SERIES_TRAINING_ASSOCIATION, cascade="all,delete")
    plans = relationship("Plan", cascade="all,delete")


class Plan(BASE_MODEL):
    __tablename__ = "plan"
    id = Column(BigInteger, primary_key=True)
    start_date = Column(DateTime(timezone=True))
    end_date = Column(DateTime(timezone=True))
    days_in_week = Column(ARRAY(String))
    training_id = Column(BigInteger, ForeignKey("training.id"))
    user_id = Column(BigInteger, ForeignKey("user.id"))


class User(BASE_MODEL):
    __tablename__ = "user"
    id = Column(BigInteger, primary_key=True)
    name = Column(String)
    nickname = Column(String, unique=True)
    password = Column(String)
    email = Column(String)
    birthdate = Column(DateTime(timezone=True))
    role = Column(String)
    active = Column(Boolean)
    inserted = Column(DateTime(timezone=True), server_default=func.now())
    updated = Column(DateTime(timezone=True), onupdate=func.now())
    body_sizes = relationship("BodySize")
    body_weights = relationship("BodyWeight")
    trainers = relationship("Trainer", secondary=USER_TRAINER_ASSOCIATION)
    trainer_proposal = relationship("TrainerPropose")
    plans = relationship("Plan")


class BodySize(BASE_MODEL):
    __tablename__ = "body_size"
    id = Column(BigInteger, primary_key=True)
    biceps = Column(Numeric)
    waist = Column(Numeric)
    neck = Column(Numeric)
    inserted = Column(DateTime(timezone=True), server_default=func.now())
    updated = Column(DateTime(timezone=True), onupdate=func.now())
    user_id = Column(BigInteger, ForeignKey("user.id"))


class BodyWeight(BASE_MODEL):
    __tablename__ = "body_weight"
    id = Column(BigInteger, primary_key=True)
    weight = Column(Numeric)
    inserted = Column(DateTime(timezone=True), server_default=func.now())
    updated = Column(DateTime(timezone=True), onupdate=func.now())
    user_id = Column(BigInteger, ForeignKey("user.id"))


class Trainer(BASE_MODEL):
    __tablename__ = "trainer"
    id = Column(BigInteger, primary_key=True)
    user_id = Column(BigInteger, ForeignKey("user.id"), unique=True)
    trainer_proposal = relationship("TrainerPropose")


class TrainerPropose(BASE_MODEL):
    __tablename__ = "trainer_propose"
    id = Column(BigInteger, primary_key=True)
    trainer_id = Column(BigInteger, ForeignKey("trainer.id"))
    user_id = Column(BigInteger, ForeignKey("user.id"))
