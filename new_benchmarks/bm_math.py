"""
Benchmark math module.
"""


import perf
from six.moves import xrange
import math


def add_cmdline_args(cmd, args):
    if args.benchmark:
        cmd.append(args.benchmark)


def bench_basic_math(loops):
    range_it = xrange(loops)
    t0 = perf.perf_counter()

    for _ in range_it:
        for i in xrange(1000):
            # To benchmark isinf()
            x = 10.0**180
            math.isinf(x)
            math.isinf(x*x)

            # To benchmark Float to ints
            VALUES = [-1.5, -0.8, -0.5, -0.2, 0, 0.2, 0.5, 0.8, 1, ]
            for i in VALUES:
                math.trunc(i)
                math.floor(i)
                math.ceil(i)

            # To benchmark modf()
            for i in xrange(10):
                math.modf(i/2.0)

            # To benchmark frexp()
            for i in [0.1, 0.5, 4.0]:
                math.frexp(i)

            # To benchmark ldexp()
            VALUES = [(.8, -3), (.5, 0), (.5, 3)]
            for m, e in VALUES:
                math.ldexp(m, e)

            # To benchmark fabs()
            VALUES = [-1.1, -0.0, 0.0, 1.1]
            for i in VALUES:
                math.fabs(i)

            # To benchmark factorial()
            VALUES = [0, 1.0, 2.0, 3.0, 4.0, 5.0]
            for i in VALUES:
                math.factorial(i)

            # To benchmark gamma() and lgamma()
            VALUES = [1.1, 2.2, 3.3, 4.4, 5.5, 6.6]
            for i in VALUES:
                math.gamma(i)
                math.lgamma(i)

            # To benchmark fmod()
            VALUES = [(5, 2), (5, -2), (-5, 2), ]
            for x, y in VALUES:
                math.fmod(x, y)

            # To benchmark pow()
            VALUES = [(2, 3), (2.1, 3.2), (1.0, 5), (2.0, 0)]
            for x, y in VALUES:
                math.pow(x, y)

            # To benchmark sqrt()
            VALUES = [9.0, 3.0]
            for i in VALUES:
                math.sqrt(i)

            # To benchmark log
            VALUES = [(8, 2), (8, 2), (.5, 2)]
            for x, y in VALUES:
                math.log(x, y)

            # To benchmark trigonometric functions
            for deg in xrange(0, 361, 30):
                rad = math.radians(deg)
                math.sin(rad)
                math.cos(rad)
                math.tan(rad)

            # To benchmark hyperbolic functions
            for i in xrange(0, 11, 2):
                x = i/10.0
                math.sinh(x)
                math.cosh(x)
                math.tanh(x)

            # To benchmark Gauss function
            VALUES = [-3, -2, -1, -0.5, -0.25, 0, 0.25, 0.5, 1, 2, 3]
            for i in VALUES:
                math.erf(i)
                math.erfc(i)

    dt = perf.perf_counter() - t0

    return dt


BENCHMARKS = {
    "basic_math":     bench_basic_math,
}


if __name__ == "__main__":
    runner = perf.Runner(add_cmdline_args=add_cmdline_args)

    runner.metadata['description'] = "Performance of the math module"

    parser = runner.argparser
    parser.add_argument("benchmark", nargs='?', choices=sorted(BENCHMARKS))

    options = runner.parse_args()

    if options.benchmark:
        benchmarks = (options.benchmark,)
    else:
        benchmarks = sorted(BENCHMARKS)

    for bench in benchmarks:
        name = '%s' % bench
        bench_func = BENCHMARKS[bench]

        runner.bench_time_func(name, bench_func, inner_loops=10)
