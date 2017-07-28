FROM python:3.6.2-stretch

RUN python -m pip install perf
ADD ./bm_python_startup.py  ./
RUN python bm_python_startup.py 


