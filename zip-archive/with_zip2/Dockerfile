FROM python:2.7.13-stretch
RUN pip install perf
ADD ./inmemory_zip_importer.py ./ 
ADD ./startup.py ./
ADD ./common_module_archive.py ./
ADD ./bm_python_startup.py ./
#RUN python bm_python_startup.py 
