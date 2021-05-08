import sys
import ast

fp = 0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff
a = -0x3
p_x = 0x6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296
p_y = 0x4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5


def calc_s(mod, point_x, point_y, current_x, current_y, a):
    if current_y == point_y and current_x == point_x:
        s = (3 * point_x ** 2 + a) * pow(2 * point_y, -1, mod)
    elif current_x == point_x:
        return None
    else:
        s = (current_y - point_y) * pow(current_x - point_x, -1, mod)
    return s % mod


def calc_new_x(mod, s, point_x, current_x):
    xr = s ** 2 - point_x - current_x
    return xr % mod


def calc_new_y(mod, s, point_x, point_y, xr):
    yr = s * (point_x - xr) - point_y
    return yr % mod


def decrypt(mod, a, point_x, point_y, pub_x, pub_y):
    current_x = point_x
    current_y = point_y
    key = 1
    while True:
        key += 1
        s = calc_s(mod, point_x, point_y, current_x, current_y, a)
        if s is None:
            return 0
        current_x = calc_new_x(mod, s, point_x, current_x)
        current_y = calc_new_y(mod, s, point_x, point_y, current_x)
        if current_x == pub_x and current_y == pub_y:
            return key


if __name__ == '__main__':
    test_x, test_y = ast.literal_eval(sys.argv[1])
    key = decrypt(fp, a, p_x, p_y, test_x, test_y)
    print(key)
