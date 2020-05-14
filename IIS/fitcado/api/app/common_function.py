import hashlib
import logging
import os
from base64 import b64encode

from fitcado.database import SESSION, models

LOG = logging.getLogger("common func")


def get_user(nickname):
    user = SESSION.query(models.User).filter(models.User.nickname == nickname)
    return user


def hash_password(password):
    salt = b64encode(os.urandom(64))
    key = hashlib.pbkdf2_hmac("sha256", password.encode("utf-8"), salt, 100000)
    return salt.decode("utf-8") + b64encode(key).decode("utf-8")


def compare_password(hashed_pwd, unhashed_pwd):
    salt = hashed_pwd[:88]
    password = hashed_pwd[88:]
    new_hashed_pwd = hashlib.pbkdf2_hmac("sha256", unhashed_pwd.encode("utf-8"), salt.encode("utf-8"), 100000)
    new_hashed_pwd = b64encode(new_hashed_pwd).decode("utf-8")
    return new_hashed_pwd == password
