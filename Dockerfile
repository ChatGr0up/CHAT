FROM ubuntu:24.04

ARG APP_DIR=/app
ENV APP_DIR=${APP_DIR}
ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && apt upgrade -y && \
    apt install -y \
        build-essential=12.10ubuntu1 \
        cmake=3.28.3-1build7 \
        libjsoncpp-dev=1.9.5-6build1 \
        libmysqlclient-dev=8.0.42-0ubuntu0.24.04.1 \
        uuid-dev=2.39.3-9ubuntu6.2 \
        libpq-dev=16.8-0ubuntu0.24.04.1 \
        libsqlite3-dev=3.45.1-1ubuntu2.1 \
        libhiredis-dev=1.2.0-6ubuntu3 \
        libyaml-cpp-dev=0.8.0+dfsg-6build1 \
        libdrogon-dev=1.8.7+ds-1.1build1 \
        libbrotli-dev=1.1.0-2build2 \
        librdkafka-dev=2.3.0-1build2 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR ${APP_DIR}
COPY . ${APP_DIR}
CMD ["/bin/bash"]
