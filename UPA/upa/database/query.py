import random

import matplotlib.pyplot as plt
import pandas as pd
from upa.database import transaction
from upa.database.database import database
from upa.utils.logger import create_logger

LOG = create_logger(__name__)


def create_box_plot(ax, dato, xlabel, ylabel):
    ax.yaxis.set_ticks_position("none")
    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)
    ax.boxplot(dato)


def create_hist(ax, dato, label):
    ax.set_xlabel(label)
    ax.yaxis.set_ticks_position("none")
    ax.grid(color="grey", axis="y", linestyle="-", linewidth=0.25, alpha=0.5)
    ax.grid(color="grey", axis="x", linestyle="-", linewidth=0.25, alpha=0.5)
    ax.hist(dato, len(dato) * 2, density=False, histtype="barstacked", orientation="horizontal")


def characteristics_query(column=None, value=None):
    """Get characteristics for specific user (open_uuid) or subnet (ip_anonym)."""
    if column is None or value is None:
        column = random.choice(["ip_anonym", "open_test_uuid"])
        value = transaction.get_random_subnet(column)[0][0]

    DEFAULT_COLUMNS = ("download_kbit", "upload_kbit", "ping_ms")
    X, Y = 2, 3
    query = """
    SELECT speedtest.{}, speedtest.{}, speedtest.{}
    FROM speedtest WHERE speedtest.{} = '{}'
    """.format(
        *DEFAULT_COLUMNS, column, value
    )

    with database.POSTGRES.cursor() as cur:
        cur.execute(query)
        rows = cur.fetchall()
    df = pd.DataFrame(rows, columns=DEFAULT_COLUMNS)

    plt.style.use("seaborn-muted")
    fig, axs = plt.subplots(Y, X, figsize=(10, 13))
    for y in range(Y):
        label = DEFAULT_COLUMNS[y]
        create_box_plot(axs[y, 0], df[label].values, *label.split("_"))
        create_hist(axs[y, 1], df[label].values, label.replace("_", "/"))

    axs[0, 0].set_title(f"{column}: {value}", fontsize=15)
    fig.subplots_adjust(left=0.08, right=0.98, bottom=0.05, top=0.9, hspace=0.4, wspace=0.3)
    fig.tight_layout()
    return fig


def asn_query():
    """Query data from database, plot them and return figure."""
    # Query asn and print the most frequent ones
    query = """
            SELECT speedtest.asn, country, COUNT(speedtest.asn) as count
            FROM speedtest, asn WHERE speedtest.asn = asn.asn
            GROUP BY speedtest.asn, asn.country ORDER BY count DESC LIMIT 10;
            """

    con = database.POSTGRES
    cur = con.cursor()
    cur.execute(query)
    rows_asn = cur.fetchall()

    # Query asn and print the most frequent country
    query = """
            SELECT country, COUNT(country) as count
            FROM speedtest, asn WHERE speedtest.asn = asn.asn
            GROUP BY country ORDER BY count DESC LIMIT 10;
            """
    cur.execute(query)
    rows_country = cur.fetchall()

    cur.close()
    con.close()

    fig = plot_queries(rows_asn, rows_country)

    return fig


def plot_queries(asn_result, country_result):
    """Plot data from queries and return figure."""
    asn_labels = []
    asn_count = []
    country_labels = []
    country_count = []
    for row in asn_result:
        asn_labels.append(f"ASN{row[0]}")
        asn_count.append(row[2])

    for row in country_result:
        country_labels.append(row[0])
        country_count.append(row[1])

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(10, 13))
    ax1.bar(asn_labels, asn_count)
    for i in range(len(asn_labels)):
        ax1.annotate(asn_labels[i], (i - 0.5, asn_count[i] + 1200))

    ax1.set_ylabel("Frequency")
    ax1.set_axisbelow(True)
    # Rotate xticks and avoid warning with FixedLocator
    ax1.set_xticks(ax1.get_xticks())
    ax1.set_xticklabels(asn_labels, rotation=65)
    ax1.yaxis.grid(True, linestyle="--", which="major", color="grey", alpha=0.4)
    ax1.tick_params(which="both", top="off", right="off", left="on", bottom="off")
    ax1.set_title("ASN occurrence")

    ax2.bar(country_labels, country_count)
    for i in range(len(asn_labels)):
        ax2.annotate(country_labels[i], (i - 0.5, country_count[i] + 4000))

    ax2.set_ylabel("Frequency")
    ax2.set_axisbelow(True)
    ax2.yaxis.grid(True, linestyle="--", which="major", alpha=0.45)
    ax2.tick_params(which="both", top="off", right="off", left="on", bottom="off")
    ax2.set_title("ASN country occurrence")

    return fig


def query2_pie_prepare_data(results):
    data = {}
    for r in results:
        current_range = r[1]
        data.setdefault(current_range, list())  # set ranges

        data[current_range].append((r[2], r[3]))

    return data


def query2():
    results = transaction.query_3_download()
    x = [round(num[2] / 99999, 2) for num in results]
    y = [r[1] for r in results]
    fig = plt.figure()
    ax = fig.add_axes([0, 0, 1, 1])
    ax.bar(x, y)

    fig, axs = plt.subplots(2, 2, figsize=(10, 13))

    axs[0, 0].bar(x, y)
    axs[0, 0].set_xlabel("x * 10^5 download_kbit")
    axs[0, 0].set_ylabel("Count of tests")

    results = transaction.query_3_upload()
    x = [round(num[2] / 99999, 2) for num in results]
    y = [r[1] for r in results]
    axs[0, 1].bar(x, y)
    axs[0, 1].set_xlabel("x * 10^5 upload_kbit")
    axs[0, 1].set_ylabel("Count of tests")

    results = transaction.query_3_differencies_between_users_model()
    data = query2_pie_prepare_data(results)

    data1 = data["0-99999"]
    data2 = data["900000-999999"]
    vals = [x[0] for x in data1]
    labels = [x[1] for x in data1]

    axs[1, 0].pie(vals)
    axs[1, 0].legend(labels, loc="lower right")
    axs[1, 0].set_title("Top 5 models in first download range")
    vals = [x[0] for x in data2]
    labels = [x[1] for x in data2]

    axs[1, 1].pie(vals)
    axs[1, 1].legend(labels, loc="lower right")
    axs[1, 1].set_title("Top 5 models in last download range")
    return fig


if __name__ == "__main__":
    characteristics_query("ip_anonym", "82.46.124.0/24")
    asn_fig = asn_query()
