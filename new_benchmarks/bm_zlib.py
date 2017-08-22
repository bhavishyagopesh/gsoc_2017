"""
Benchmark zlib module.
"""


import perf
from six.moves import xrange
import zlib
import binascii


def add_cmdline_args(cmd, args):
    if args.benchmark:
        cmd.append(args.benchmark)

DATA = b'This is to benchmark zlib.'*100000
COMPRESSED = zlib.compress(DATA)
DECOMPRESSED = zlib.decompress(COMPRESSED)


def bench_compress(loops):
    range_it = xrange(loops)
    t0 = perf.perf_counter()

    for _ in range_it:
        zlib.compress(DATA)

    dt = perf.perf_counter() - t0

    return dt


def bench_decompress(loops):
    range_it = xrange(loops)
    t0 = perf.perf_counter()

    for _ in range_it:
        zlib.decompress(COMPRESSED)

    dt = perf.perf_counter() - t0

    return dt


BENCHMARKS = {
    "compress":     bench_compress,
    "decompress":   bench_decompress,
}


if __name__ == "__main__":
    runner = perf.Runner(add_cmdline_args=add_cmdline_args)

    runner.metadata['description'] = "Performance of the zlib module"

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
