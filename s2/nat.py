#!python3
# import sys


def toBase(num, n=1, base=2**32):
    num = int(num) ** int(n)
    res = []
    while num:
        res.append(num % base)
        num //= base
    print(res)
    return res


# print(toBase32(sys.argv[1], sys.argv[2] if len(sys.argv) == 3 else 1))

base = 2 * (1777 ** 3)
num = 0
aux = 1
for i in range(6):
    num += aux * i
    aux *= base
    toBase(num)
toBase(num, base=base)
