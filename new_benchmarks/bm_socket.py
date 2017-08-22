import perf

import sys

from six.moves import xrange


def add_cmdline_args(cmd, args):
    if args.benchmark:
        cmd.append(args.benchmark)

from socket import socket, AF_INET, SOCK_STREAM, SOL_SOCKET, SO_REUSEADDR


def server(address):
    sock = socket(AF_INET, SOCK_STREAM)
    sock.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)

    sock.bind(address)
    sock.listen(5)
    while True:
        client, addr = sock.accept()
        handler(client)


def handler(client):
    while True:
        req = client.recv(100)

        if not req:
            break
        client.send(b'1')


# server(('',25000))


PACKET_COUNT = 1000000

def bench_echo_socket(loops):

    range_it = xrange(loops)
    t0 = perf.perf_counter()

    sock = socket(AF_INET, SOCK_STREAM)
    sock.connect(('', 25000))

    for i in xrange(PACKET_COUNT):
        sock.send(b'1')


    dt = perf.perf_counter() - t0

    return dt


BENCHMARKS = {
"echo_socket": bench_echo_socket,
}


if __name__ == "__main__":
    runner = perf.Runner(add_cmdline_args=add_cmdline_args)

    runner.metadata['description'] = "Performance of the socket module"

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
