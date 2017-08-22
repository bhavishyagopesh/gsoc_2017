"""
Benchmark concurrency.
"""

import perf
# from six.moves import xrange
import threading
import multiprocessing
import sys


def add_cmdline_args(cmd, args):
    if args.benchmark:
        cmd.append(args.benchmark)


def crunch_numbers_worker(CRUNCH_NO):
    x = 0
    while x < CRUNCH_NO:
        x += 1

# Variable deciding the weight of crunch_numbers_worker
CRUNCH_NO = 1000

NUM_WORKERS = 4


def bench_crunch_numbers_threading(loops):
    range_it = xrange(loops)
    t0 = perf.perf_counter()

    for _ in range_it:
        threads = [threading.Thread(target=crunch_numbers_worker(CRUNCH_NO)) for _ in range(NUM_WORKERS)]
        [thread.start() for thread in threads]
        [thread.join() for thread in threads]

    dt = perf.perf_counter() - t0

    return dt


def bench_crunch_numbers_multiprocessing(loops):
    range_it = xrange(loops)
    t0 = perf.perf_counter()

    for _ in range_it:
        processes = [multiprocessing.Process(target=crunch_numbers_worker(CRUNCH_NO)) for _ in range(NUM_WORKERS)]
        [process.start() for process in processes]
        [process.join() for process in processes]

    dt = perf.perf_counter() - t0

    return dt


BENCHMARKS = {
    "threading_concurrency":           bench_crunch_numbers_threading,
    "multiprocessing_concurrency":     bench_crunch_numbers_multiprocessing,
}


if __name__ == "__main__":
    runner = perf.Runner(add_cmdline_args=add_cmdline_args)

    runner.metadata['description'] = "Performance concurrency implemented using threading and multiprocessing"

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
