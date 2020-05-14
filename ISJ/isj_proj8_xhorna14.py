#!/usr/bin/env python3


def first_with_given_key(iterable, key=lambda x: x):
    keys_yielded = {}
    for i in iterable:
        k = repr(key(i))
        if keys_yielded.get(k, None) is None:
            keys_yielded[k] = k
            yield i
