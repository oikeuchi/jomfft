import argparse

def product(factors):
    out = 1
    for f in factors:
        out *= f
    return out

def idempotent(n, p):
    assert(n > 0 and p > 0)
    q = n // p
    assert(q % p != 0)
    i = 1
    while (q * i) % p != 1:
        i += 1
    assert(i < p)
    return q * i

def recursively_append_indices(n, sizes, scales, i, v, out):
    if i < 0:
        out.append(v % n)
    else:
        for j in range(sizes[i]):
            recursively_append_indices(n, sizes, scales, i - 1,
                                  v + scales[i] * j, out)

def input_indices(factors):
    n = product(factors)
    e = []
    for f in factors:
        e.append(idempotent(n, f))
    out = []
    for j in range(factors[-1]):
        recursively_append_indices(n, factors, e, len(factors) - 2,
                                e[-1] * j, out)
    return out

def output_indices(factors):
    n = product(factors)
    g = []
    for f in factors:
        g.append(n //f)
    out = []
    for j in range(factors[-1]):
        recursively_append_indices(n, factors, g, len(factors) - 2,
                               g[-1] * j, out)
    return out

def do_main():
    parser = argparse.ArgumentParser(description="Prime-Factor algorithm index calculator.")
    parser.add_argument('factors', metavar='<factor>', nargs='+', type=int,
            help="Relatively prime factors.")

    args = parser.parse_args()
    if len(args.factors) < 2:
        print("At least two factors are required.")
        exit()
    i = input_indices(args.factors)
    o = output_indices(args.factors)
    print("Input: " + ','.join(map(str, i)))
    print("Output: " + ','.join(map(str, o)))

if __name__ == '__main__':
    do_main()
