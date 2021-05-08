import os
from datetime import datetime

import pandas as pd
import pymongo
from upa.settings import FALLBACK_DATA_PATH, MONGO_COLLECTION, MONGO_CONNECTION_STRING, MONGO_DB, RAW_DATA_PATH
from upa.utils.logger import create_logger

LOG = create_logger(__name__)

DTYPES = {
    "open_uuid": "string",
    "open_test_uuid": "string",
    "download_kbit": "float",
    "upload_kbit": "float",
    "ping_ms": "float",
    "asn": "float",
    "ip_anonym": "string",
    "speed_test_duration_ms": "float",
    "network_type": "string",
    "model": "string",
}


class MongoInterface:
    def __init__(self, db_name=None, collection_name=MONGO_COLLECTION):
        self.client = pymongo.MongoClient(MONGO_CONNECTION_STRING)
        if db_name is None:
            self.db = self.client[MONGO_DB]
        else:
            self.db = self.client[db_name]
        self.change_collection(collection_name)

    def change_collection(self, name):
        self.collection_name = name
        self.collection = self.db[name]
        LOG.info(f'Working with collection "{name}"')

    def load_file(self, file_path):
        LOG.info(f"Loading file {file_path}")
        data = pd.read_csv(
            file_path,
            sep=",",
            parse_dates=["time_utc"],
            date_parser=self.date_parse,
            usecols=self.cols,
            dtype=DTYPES,
        )
        data = data.dropna().to_dict("records")
        self.collection.insert_many(data)

    def load_files_from_dir(self, path=RAW_DATA_PATH):
        found_file = False
        for file in os.scandir(path):
            if "data.csv" not in file.name:
                continue
            found_file = True
            self.load_file(file.path)
        if not found_file:
            LOG.warning(f"Data not found in {path}, using backup data.")
            self.load_files_from_dir(path=FALLBACK_DATA_PATH)
            return
        LOG.info(f'Loaded csv data to collection "{self.collection_name}"')

    @staticmethod
    def date_parse(date):
        return datetime.strptime(date, "%Y-%m-%d %H:%M:%S")

    @staticmethod
    def cols(column):
        return column in DTYPES or column == "time_utc"

    def reset_db(self):
        self.collection.drop()
        LOG.info("Dropped whole collection in MongoDB")


if __name__ == "__main__":
    import time

    s = time.perf_counter()
    m = MongoInterface()
    m.reset_db()
    m.load_files_from_dir()
    LOG.info(f"Took {time.perf_counter() - s:0.4f} seconds.")
