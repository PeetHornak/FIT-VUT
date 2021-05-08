import datetime

import click
from upa.loaders import data_loader, mongo_loader, mongo_to_postgre
from upa.utils import logger
from upa.utils.exceptions import LoaderException

LOG = logger.create_logger(__name__)


@click.group()
def run():
    pass


@run.command(name="download_one_month")
@click.option("-m", "--month", required=True, type=int, help="Month of data to download")
@click.option("-y", "--year", required=True, type=int, help="Year of data to download")
def download_one_month_data(month, year):
    """Download data for specific month and year."""
    start = datetime.date(year, month, 1)
    data_loader.run_downloader(start, start)


@run.command(name="download_data_in_range")
@click.option("-sm", "--start-month", default=None, type=int, help="Month of the first data")
@click.option("-sy", "--start-year", default=None, type=int, help="Year of the first data")
@click.option("-em", "--end-month", default=None, type=int, help="Month of the last data")
@click.option("-ey", "--end-year", default=None, type=int, help="Year of the last data")
def download_range_data(start_month, start_year, end_month, end_year):
    """Download data in given date range."""
    start = None
    end = None
    if start_month is not None and start_year is not None:
        start = datetime.date(start_year, start_month, 1)
    if end_month is not None and end_year is not None:
        end = datetime.date(end_year, end_month, 1)
    data_loader.run_downloader(start, end)


@run.command(name="print_country_code")
@click.option("--asn", help="Get country code for this ASN code")
def get_country_code(asn):
    """Return country code for ASN number."""
    try:
        LOG.info(f"Getting country code for {asn}")
        country_code = data_loader.get_countries([asn])
        print(f"Country code for {asn} is {country_code}")
    except LoaderException as e:
        LOG.error(str(e))
        LOG.warning("ASN number is not valid or problem with external server occured")


@run.command(name="init_mongo")
@click.option("-c", "--csv", default=None, help="Path to directory with csv, otherwise default path would be used")
def init_mongo(csv):
    """Create mongo database and load all csv files in raw data path."""
    LOG.info("Creating mongo database")
    mongo = mongo_loader.MongoInterface()
    mongo.reset_db()
    if csv:
        mongo.load_files_from_dir(csv)
    else:
        mongo.load_files_from_dir()


@run.command(name="init_postgre")
@click.option("-sm", "--start-month", default=None, type=int, help="Month of first data loaded to postgre database")
@click.option("-sy", "--start-year", default=None, type=int, help="Year of first data loaded to postgre database")
@click.option("-sd", "--start-day", default=1, type=int, help="Day of first data loaded to postgre database")
@click.option("-em", "--end-month", default=None, type=int, help="Month of last data loaded to postgre database")
@click.option("-ey", "--end-year", default=None, type=int, help="Year of last data loaded to postgre database")
@click.option("-ed", "--end-day", default=1, type=int, help="Day of last data loaded to postgre database")
def init_postgre(start_month, start_year, start_day, end_month, end_year, end_day):
    """Create postgre database and load data from mongo database."""
    start = None
    end = None
    if start_month is not None and start_year is not None:
        start = datetime.datetime(start_year, start_month, start_day)
    if end_month is not None and end_year is not None:
        end = datetime.datetime(end_year, end_month, end_day)
    LOG.info("Creating postgre database")
    mongo_to_postgre.load_data_from_mongo_into_postgre(start, end)


@run.command(name="init_both_databases")
@click.option("-sm", "--start-month", default=None, type=int, help="Month of first data loaded to postgre database")
@click.option("-sy", "--start-year", default=None, type=int, help="Year of first data loaded to postgre database")
@click.option("-sd", "--start-day", default=1, type=int, help="Day of first data loaded to postgre database")
@click.option("-em", "--end-month", default=None, type=int, help="Month of last data loaded to postgre database")
@click.option("-ey", "--end-year", default=None, type=int, help="Year of last data loaded to postgre database")
@click.option("-ed", "--end-day", default=1, type=int, help="Day of last data loaded to postgre database")
@click.pass_context
def init_databases(ctx, start_month, start_year, start_day, end_month, end_year, end_day):
    """Create mongo and postgre database and load data to both of them."""
    ctx.invoke(init_mongo)
    ctx.forward(init_postgre)


@run.command(name="update_mongo")
@click.option("--dir", default=None, help="Path to directory with csv files.")
@click.option("--file", default=None, help="Path to specific csv file with data.")
def update_mongo(dir, file):
    """Add new data to mongo from directory or file."""
    mongo = mongo_loader.MongoInterface()
    if dir is not None:
        mongo.load_files_from_dir(dir)
    if file is not None:
        mongo.load_file(file)


if __name__ == "__main__":
    run()
