import dataclasses
import datetime
import time

from upa.database import dataclass, transaction
from upa.database.dataclass import ASN, Databases, Speedtest
from upa.loaders.data_loader import get_asns_country_pair
from upa.utils.logger import create_logger

LOG = create_logger(__name__)


def load_data_from_mongo_into_postgre(start, end):
    load_asns_to_postgre(start, end)
    transaction.drop_tests()
    LOG.info("Dropping current tests in Postgres.")
    LOG.info("Loading tests to postgres in batches.")
    for batch in transaction.load_data_from_mongo(start, end):
        s = time.perf_counter()
        speedtests_tuple = prepare_data(batch)
        transaction.load_data_to_postgre(
            speedtests_tuple, dataclass.get_dataclass_field_names(Speedtest), Databases.speedtest
        )  # referencing to asn
        LOG.info(f"Batch took {time.perf_counter()-s:0.4f} seconds.")


def load_asns_to_postgre(start, end):
    LOG.info("Loading asn/country into DB.")
    loaded_asns = transaction.fetch_asns_in_postgre()
    asns = transaction.load_asns_from_mongo(start, end)
    asns = filter(lambda a: (a,) not in loaded_asns, map(int, asns))
    asns_tuple = get_asns_country_pair(asns)
    # load asn records first because of references
    transaction.load_data_to_postgre(asns_tuple, dataclass.get_dataclass_field_names(ASN), Databases.asn)


def prepare_data(batch):
    speedtests_tuple = list()
    for row in batch:
        try:
            speedtest = Speedtest(**row)
        except ValueError:
            LOG.warning("prepare_data invalid dato, skipping it.")
            continue

        speedtests_tuple.append(dataclasses.astuple(speedtest))

    return tuple(speedtests_tuple)


if __name__ == "__main__":
    s = time.perf_counter()
    start = datetime.datetime(2020, 9, 1)
    end = datetime.datetime(2020, 9, 2)
    load_data_from_mongo_into_postgre(start, end)
    LOG.info(f"Took {time.perf_counter() - s:0.4f} seconds.")
