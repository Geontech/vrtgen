FROM ubuntu:18.04
ARG branch

COPY . /root/vrtgen

WORKDIR /root/vrtgen

ENV CXX=g++-10
ENV MAKEFLAGS="-j$(nproc)"

RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections \
    && apt update \
    && apt install -f -m \
                   -o Acquire::BrokenProxy="true" \
                   -o Acquire::http::No-Cache="true" \
                   -o Acquire::http::Pipeline-Depth="0" \
                   -y software-properties-common wget uuid-dev libssl-dev git
# Extra repositories for g++, python, and cmake
RUN add-apt-repository -y ppa:ubuntu-toolchain-r/test \
    && add-apt-repository -y ppa:deadsnakes/ppa \
    && wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null \
    && apt-add-repository -y 'deb https://apt.kitware.com/ubuntu/ bionic main' \
    && apt update
# Install g++
RUN apt install -f -m \
                -o Acquire::BrokenProxy="true" \
                -o Acquire::http::No-Cache="true" \
                -o Acquire::http::Pipeline-Depth="0" \
                -y g++-10
# Install Python 3.8
RUN apt install -f -m \
                -o Acquire::BrokenProxy="true" \
                -o Acquire::http::No-Cache="true" \
                -o Acquire::http::Pipeline-Depth="0" \
                -y python3.8-full
RUN wget https://bootstrap.pypa.io/get-pip.py \
    && python3.8 get-pip.py
# Install cmake
RUN apt install -f -m \
                -o Acquire::BrokenProxy="true" \
                -o Acquire::http::No-Cache="true" \
                -o Acquire::http::Pipeline-Depth="0" \
                -y cmake
# nats.c install
RUN export CXX=g++-10 \
    && export CC=gcc-10 \
    && git clone https://github.com/nats-io/nats.c \
    && cd nats.c \
    && cmake -DNATS_BUILD_STREAMING=OFF -B build \
    && cmake --build build \
    && cmake --build build --target install \
    && cd .. \
    && rm -rf nats.c
RUN python3.8 -m pip install --upgrade pip \
    && python3.8 -m pip install pytest pylint \
    && python3.8 -m pip install . \
    && cmake -B build -DJUNIT_OUTPUT=yes -DBUILD_TESTS=ON . \
    && make -C build