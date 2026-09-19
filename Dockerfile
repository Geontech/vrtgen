FROM docker.io/library/python:3.12-slim
WORKDIR /app
COPY . ./
RUN python3 -m pip --no-cache-dir install .
ENTRYPOINT ["vrtpktgen"]
