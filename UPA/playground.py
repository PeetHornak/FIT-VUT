import matplotlib.pyplot as plt
import seaborn as sns
from upa.utils.logger import create_logger

LOG = create_logger(__name__)

sns.set_theme(style="ticks", palette="pastel")

# Load the example tips dataset
tips = sns.load_dataset("tips")

# Draw a nested boxplot to show bills by day and time
sns.boxplot(x="day", y="total_bill",
            hue="smoker", palette=["m", "g"],
            data=tips)
sns.despine(offset=10, trim=True)
LOG.info('GOT PLOT')
plt.show()
