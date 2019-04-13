# https://en.wikipedia.org/wiki/Bailey–Borwein–Plouffe_formula#BBP_compared_to_other_methods_of_computing_π

# generalize term summation
P = lambda terms: lambda k: 1 / 16 ** k * sum([
    a / (8 * k + b) for a, b in terms
])

# bbp function (for one term, at k)
bbp = P(((4, 1), (-2, 4), (-1, 5), (-1, 6)))

# returns a decimal type w/ the given precision
def withprec (n):
    import decimal
    # hack since technically this is global
    decimal.getcontext().prec = max(n, 1)
    return decimal.Decimal

# sum terms, and combine everything together
pi = lambda n: sum(map(bbp, map(withprec(n), range(n))))

if __name__ == '__main__':
    for i in range(1, 500):
        print(pi(i))
