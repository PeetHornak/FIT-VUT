import logging

logging.basicConfig(level=logging.INFO, format="%(asctime)s :: %(levelname)s :: %(message)s")


def create_logger(name):
    return logging.getLogger(name)
