import asyncio
import dataclasses
import datetime
import os
import zipfile
from io import BytesIO

import aiohttp
import orjson as json
import requests
from dateutil import relativedelta
from upa.database.dataclass import ASN
from upa.database.transaction import remove_tests_with_asns
from upa.settings import DATA_URL, DOWNLOAD_ENDPOINT, RAW_DATA_PATH
from upa.utils.logger import create_logger

LOG = create_logger(__name__)


def get_zip_from_url(url):
    """Download zip from given url."""
    r = requests.get(url, stream=True)
    r.raise_for_status()
    zip = zipfile.ZipFile(BytesIO(r.content))
    LOG.info(f"Got {url}.")
    return zip


def build_download_url(year, month):
    """Build url for downloading the zip file for given date."""
    return DATA_URL + DOWNLOAD_ENDPOINT + f"NetMetr-opendata-{year}-{month}.zip"


def get_zips_in_range(start_date, end_date):
    """Get zip for each month inside given range."""
    available_dates = get_available_dates()
    delta = relativedelta.relativedelta(months=1)
    while start_date <= end_date:
        months = available_dates.get(str(start_date.year), None)
        if months is None or start_date.month not in months:
            LOG.info(f"{start_date.year}/{start_date.month} is not available.")

        url = build_download_url(start_date.year, start_date.month)
        try:
            yield get_zip_from_url(url), start_date
        except requests.exceptions.RequestException as e:
            LOG.info(f"{start_date.year}/{start_date.month} failed: {e}")
        finally:
            start_date += delta


def get_available_dates():
    """Get dict with data for available years and their months."""
    r = requests.get(f"{DATA_URL}RMBTStatisticServer/months")
    r.raise_for_status()
    try:
        months = r.json()["revised_months"][0]
        return months
    except Exception as e:
        LOG.error(str(e))
        LOG.warning("Issue with parsing months, returning raw response")
        months = r.json()

    return months


def save_csv_data(zip, date):
    """Get zip and store it as csv file."""
    for name in zip.namelist():
        if ".csv" in name:
            try:
                zip.extract(name, RAW_DATA_PATH)
                new_name = f"{date.year}.{date.month}.{name}"
                os.rename(RAW_DATA_PATH + name, RAW_DATA_PATH + new_name)
                LOG.info(f"Stored data into {new_name}.")
            except Exception as e:
                LOG.info(f'Issue with extracting" {e}')


def get_first_date():
    """Get date for first available csv file."""
    dates = get_available_dates()
    first = min(dates, key=dates.get)
    return datetime.date(int(first), dates[first][0], 1)


def run_downloader(start_date=None, end_date=None):
    """Run the whole downloader, replace dates if missing."""
    if start_date is None:
        start_date = get_first_date()
    if end_date is None:
        end_date = datetime.date.today()
    LOG.info(f"Getting csv files from {start_date} to {end_date}.")
    for zip, date in get_zips_in_range(start_date, end_date):
        save_csv_data(zip, date)


async def get(asn):
    """Get asn using aiohttp."""
    LIMIT = 3
    async with aiohttp.ClientSession() as session:
        url = f"https://stat.ripe.net/data/rir-geo/data.json?resource={asn}"
        for i in range(LIMIT):
            try:
                async with session.get(url=url, raise_for_status=True, timeout=60) as response:
                    resp = await response.read()
                    country = json.loads(resp)["data"]["located_resources"][0]["location"]
                    return [asn, country]
            except aiohttp.ClientResponseError as e:
                LOG.info(f"Code {e.status} from the server for asn {asn}, retry {i+1}/{LIMIT}.")
            except json.JSONDecodeError:
                LOG.info(f"Incorrect response from the server for asn {asn}, retry {i+1}/{LIMIT}.")
            except Exception:
                LOG.info(f"Some unexpected error happened for asn {asn}, retry {i+1}/{LIMIT}.")
        LOG.warning("Unable to get url {}".format(url))
        return [asn, None]


async def get_countries(asns):
    """Get countries for all given asns."""
    LOG.info(f"Getting countries for asns.")
    responses = await asyncio.gather(*[get(asn) for asn in asns])

    failed = []
    results = []
    for r in responses:
        failed.append(r) if r[1] is None else results.append(r)

    return results, failed


def get_asns_country_pair(asns):
    res = list()
    asns, failed = asyncio.run(get_countries(asns))

    remove_tests_with_asns([asn[0] for asn in failed])

    for asn in asns:
        a = ASN(*asn)
        res.append(dataclasses.astuple(a))

    return res


if __name__ == "__main__":
    start = datetime.date(2020, 7, 1)
    end = datetime.date(2020, 10, 1)
    run_downloader(start, end)
