FROM ubuntu:18.04
ARG branch

COPY . /root/vrtgen

WORKDIR /root/vrtgen

RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections \
    && apt update \
    && apt install -y software-properties-common wget \
    # Extra repositories for g++, python, and cmake
    && add-apt-repository -y ppa:ubuntu-toolchain-r/test \
    && add-apt-repository -y ppa:deadsnakes/ppa \
    && wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null \
    && apt-add-repository -y 'deb https://apt.kitware.com/ubuntu/ bionic main' \
    && apt update \
    # Install g++
    && apt install -y g++-10 \
    && export CXX=g++-10 \
    # Install Python 3.8
    && apt install -y python3.8-full \
    && wget https://bootstrap.pypa.io/get-pip.py \
    && python3.8 get-pip.py \
    # Install cmake
    && apt install -y cmake \
    && python3.8 -m pip install --upgrade pip \
    && python3.8 -m pip install pytest pylint \
    && python3.8 -m pip install . \
    && cmake -B build -DJUNIT_OUTPUT=yes . \
    && export MAKEFLAGS="-j$(nproc)" \
    && make -C build