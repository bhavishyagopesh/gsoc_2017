FROM ubuntu
RUN apt-get update && apt-get install -y python3 python3-pip python3-dev build-essential libpq-dev
RUN pip3 install perf
ADD ./load_and_exec.sh / 
RUN chmod +x load_and_exec.sh
CMD ["/bin/bash","-c","/load_and_exec.sh"]