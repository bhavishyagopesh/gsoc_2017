FROM python:3.6.2-stretch

RUN python -m pip install perf
ADD ./bm_python_startup.py  ./
ADD ./common_module_archive.py ./ 
RUN python common_module_archive.py 
RUN export PYTHONPATH=/common_module_archive.zip
RUN python -m site
RUN python bm_python_startup.py 


