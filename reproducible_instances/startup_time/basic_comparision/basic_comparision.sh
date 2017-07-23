#!/usr/bin/env sh

python3 -m pip install perf
python2 -m pip install perf

python3 -m pip install performance
python2 -m pip install performance

pyperformance run --python=python2 -o py2.json
pyperformance run --python=python3 -o py3.json
pyperformance compare py2.json py3.json
python3 -m perf compare_to py2.json py3.json --table

