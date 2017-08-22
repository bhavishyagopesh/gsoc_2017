"""
Benchmark smtplib module.
"""

# This benchmark requires a running smtp service on port 25
# For Eg: PostFix

import perf
from six.moves import xrange
import smtplib
import email.utils
from email.mime.text import MIMEText


def add_cmdline_args(cmd, args):
    if args.benchmark:
        cmd.append(args.benchmark)


def bench_smtplib(loops):
    range_it = xrange(loops)
    t0 = perf.perf_counter()

    for _ in range_it:
        msg = MIMEText('This is the body of the message.')
        msg['To'] = email.utils.formataddr(('Recipient',
                                            'recipient@example.com'))
        msg['From'] = email.utils.formataddr(('Author',
                                              'author@example.com'))
        msg['Subject'] = 'Simple test message'

        server = smtplib.SMTP('localhost', 25)

        try:
            server.sendmail('author@example.com',
                            ['recipient@example.com'],
                            msg.as_string())
        finally:
            server.quit()

    dt = perf.perf_counter() - t0

    return dt


BENCHMARKS = {
    "smtplib":     bench_smtplib,
}


if __name__ == "__main__":
    runner = perf.Runner(add_cmdline_args=add_cmdline_args)

    runner.metadata['description'] = "Performance of the smtplib module"

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
