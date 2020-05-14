#!/usr/bin/env python3

import collections

my_counter = collections.Counter()


def log_and_count(key=None, counts=None):
    def decorator(func):
        def wrapper(*args, **kwargs):
            print(f"called {func.__name__} with {args} and {kwargs}")
            counts[key or func.__name__] += 1
            func(*args, **kwargs)

        return wrapper

    return decorator


@log_and_count(key="basic functions", counts=my_counter)
def f1(a, b=2):
    return a ** b


@log_and_count(key="basic functions", counts=my_counter)
def f2(a, b=3):
    return a ** 2 + b


@log_and_count(counts=my_counter)
def f3(a, b=5):
    return a ** 3 - b
