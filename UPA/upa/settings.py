import os

from dotenv import load_dotenv

load_dotenv()

POSTGRES_URI = os.environ["POSTGRES_URI"]

MONGO_USERNAME = os.environ["MONGO_USERNAME"]

MONGO_PASSWORD = os.environ["MONGO_PASSWORD"]

MONGO_HOSTNAME = os.environ["MONGO_HOSTNAME"]

MONGO_COLLECTION = os.environ["MONGO_COLLECTION"]

MONGO_DB = os.environ["MONGO_DATABASE"]

MONGO_PORT = 27017

BATCH_SIZE = int(os.environ.get("BATCH_SIZE", 32000))

DATA_URL = "https://control.netmetr.cz/"

DOWNLOAD_ENDPOINT = "RMBTStatisticServer/export/"

ROOT_PATH = os.path.dirname(os.path.realpath(__file__)).rstrip("upa")

RAW_DATA_PATH = ROOT_PATH + "database/raw/"

FALLBACK_DATA_PATH = ROOT_PATH = "mini_data.csv"

MONGO_CONNECTION_STRING = f"mongodb://{MONGO_USERNAME}:{MONGO_PASSWORD}@{MONGO_HOSTNAME}:{MONGO_PORT}"
