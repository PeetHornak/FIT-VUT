from .database import commit_session, init_connection

_, SESSION, BASE_MODEL = init_connection("postgresql://postgres:docker@37.205.14.85:5432/postgres")

__all__ = ("BASE_MODEL", "SESSION", "commit_session")
