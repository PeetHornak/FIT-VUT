import io

from upa import settings
from upa.database.database import database
from upa.utils.logger import create_logger

LOG = create_logger(__name__)


class StringIteratorIO(io.TextIOBase):
    def __init__(self, iter):
        self._iter = iter
        self._buff = ""

    def readable(self) -> bool:
        return True

    def _read1(self, n=None):
        while not self._buff:
            try:
                self._buff = next(self._iter)
            except StopIteration:
                break
        ret = self._buff[:n]
        self._buff = self._buff[len(ret) :]
        return ret

    def read(self, n=None):
        line = []
        if n is None or n < 0:
            while True:
                m = self._read1()
                if not m:
                    break
                line.append(m)
        else:
            while n > 0:
                m = self._read1(n)
                if not m:
                    break
                n -= len(m)
                line.append(m)
        return "".join(line)


def fetch_asns_in_postgre():
    with database.POSTGRES.cursor() as cur:
        cur.execute("SELECT asn FROM asn")
        return cur.fetchall()


def load_data_to_postgre(tuples, columns, table):
    if not tuples:
        return
    connection = database.POSTGRES
    with connection.cursor() as cursor:
        gen = (";".join(map(lambda x: str(x).strip(), t)).replace("'", "") + "\n" for t in tuples)
        values = StringIteratorIO(gen)
        cursor.copy_from(values, table, sep=";", columns=columns)
        connection.commit()


def load_data_from_mongo(start=None, end=None):
    start_range, end_range = {}, {}
    if start:
        start_range["$gte"] = start
    if end:
        end_range["$lte"] = end

    query = (
        {
            "$and": [
                {"open_test_uuid": {"$ne": None}},
                {"open_uuid": {"$ne": None}},
                {"download_kbit": {"$ne": None}},
                {"upload_kbit": {"$ne": None}},
                {"ping_ms": {"$ne": None}},
                {"model": {"$ne": None}},
                {"network_type": {"$ne": None}},
            ]
        },
        {
            "open_test_uuid": 1,
            "model": 1,
            "network_type": 1,
            "open_uuid": 1,
            "download_kbit": 1,
            "upload_kbit": 1,
            "ping_ms": 1,
            "ip_anonym": 1,
            "asn": 1,
            "_id": 0,
        },
    )

    if start or end:
        query[0]["$and"].append({"time_utc": {**start_range, **end_range}})

    row_count = database.MONGO.collection.count()
    for i in range(row_count // settings.BATCH_SIZE + 1):
        yield database.MONGO.collection.find(*query).limit(settings.BATCH_SIZE).skip(i * settings.BATCH_SIZE)


def load_asns_from_mongo(start, end):
    start_range, end_range = {}, {}
    if start:
        start_range["$gte"] = start
    if end:
        end_range["$lte"] = end

    query = {"time_utc": {**start_range, **end_range}}
    return database.MONGO.collection.find(query).distinct("asn")


def query_3_download():
    connection = database.POSTGRES
    with connection.cursor() as cursor:
        cursor.execute(
            """with ranges as (
            select ten as min, ten + 99999 as max, ten::text||'-'||(ten + 99999)::text AS range
            from generate_series(0,900000, 100000) as ten)
            select r.range, count(s.*) as s_count, avg(s.download_kbit) as average from ranges r
            left join speedtest s on s.download_kbit between r.min and r.max
            group by r.range
            order by r.range;"""
        )
        return cursor.fetchall()


def query_3_upload():
    connection = database.POSTGRES
    with connection.cursor() as cursor:
        cursor.execute(
            """with ranges as (
            select ten as min, ten + 99999 as max, ten::text||'-'||(ten + 99999)::text AS range
            from generate_series(0,900000, 100000) as ten)
            select r.range, count(s.*) as s_count, avg(s.upload_kbit) as average from ranges r
            left join speedtest s on s.upload_kbit between r.min and r.max
            group by r.range
            order by r.range;"""
        )
        return cursor.fetchall()


def query_3_differencies_between_users_network():
    connection = database.POSTGRES
    with connection.cursor() as cursor:
        cursor.execute(
            """
            with ranges as (
            select ten  as min, ten + 99999 as max, ten::text||'-'||(ten + 99999)::text AS range
            from generate_series(0,900000, 100000) as ten)

            SELECT
              *
            FROM (
              SELECT
                ROW_NUMBER() OVER (PARTITION BY range ORDER BY range, test_count desc) AS r,
                t.*
              FROM
            (select r.range as range, count(s.*) as test_count, s.network_type from ranges r
            left join speedtest s on s.download_kbit between r.min and r.max
            group by r.range,s.network_type
            order by r.range, test_count desc) t) x
            WHERE
              x.r <= 5;"""
        )
        return cursor.fetchall()


def query_3_differencies_between_users_model():
    connection = database.POSTGRES
    with connection.cursor() as cursor:
        cursor.execute(
            """
            with ranges as (
            select ten  as min, ten + 99999 as max, ten::text||'-'||(ten + 99999)::text AS range
            from generate_series(0,900000, 100000) as ten)

            SELECT
              *
            FROM (
              SELECT
                ROW_NUMBER() OVER (PARTITION BY range ORDER BY range, test_count desc) AS r,
                t.*
              FROM
            (select r.range as range, count(s.*) as test_count, s.model from ranges r
            left join speedtest s on s.download_kbit between r.min and r.max
            group by r.range,s.model
            order by r.range, test_count desc) t) x
            WHERE
              x.r <= 5;"""
        )
        return cursor.fetchall()


def get_random_subnet(column):
    query = """
            SELECT speedtest.{} FROM speedtest OFFSET floor(random()*{}) LIMIT 1;
    """
    with database.POSTGRES.cursor() as cur:
        cur.execute("SELECT COUNT(*) from speedtest")
        n = cur.fetchall()[0][0]
        cur.execute(query.format(column, n))
        rows = cur.fetchall()
    return rows


def drop_tests():
    query = """truncate table speedtest;"""
    connection = database.POSTGRES
    with connection.cursor() as cursor:
        cursor.execute(query)
        return connection.commit()


def remove_tests_with_asns(asns):
    database.MONGO.collection.delete_many({"asn": {"$in": asns}})
