import psycopg2
from upa import settings
from upa.loaders.mongo_loader import MongoInterface


class Databases(object):
    def __init__(self):
        self._mongo = None
        self._postgres = None

    @property
    def MONGO(self):
        if not self._mongo:
            self._mongo = MongoInterface()
        return self._mongo

    @property
    def POSTGRES(self):
        if not self._postgres:
            self._postgres = psycopg2.connect(settings.POSTGRES_URI)
        if self._postgres.closed:
            self._postgres = psycopg2.connect(settings.POSTGRES_URI)
        return self._postgres


database = Databases()
