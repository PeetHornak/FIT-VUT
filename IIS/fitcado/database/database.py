import logging

from sqlalchemy import create_engine
from sqlalchemy.pool import NullPool
from sqlalchemy.exc import SQLAlchemyError
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import scoped_session, sessionmaker

LOG = logging.getLogger("database")


def init_connection(url):
    """Create database engine, session and model base for specific database url.

    Args:
        url (str): Database url.

    Returns:
        tuple: Tuple containing engine, session and model base.

    """
    db_engine = create_engine(url, pool_size=20, max_overflow=5)
    db_session = scoped_session(sessionmaker(bind=db_engine))
    base = declarative_base(bind=db_engine)
    return db_engine, db_session, base


def commit_session(db_session, raise_exc=False):
    """Try to commit passed session.

    Args:
        db_session (:obj:`sqlalchemy.orm.session.Session`): Session to commit.
        raise_exc (bool, optional): `True` if exception shall be raised. Default is `True`.

    Returns:
        bool: `True` if session was committed else `False`.

    """
    try:
        db_session.commit()
        return True
    except SQLAlchemyError:
        db_session.rollback()
        if raise_exc:
            raise
        LOG.info("commit_session.raised_exception_silenced")
    return False
