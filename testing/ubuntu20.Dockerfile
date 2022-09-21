FROM ubuntu:20.04
ARG branch

COPY . /root/vrtgen

WORKDIR /root/vrtgen

RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections \
        && apt update \
        && apt install -y g++-10 python3-pip wget software-properties-common \
        && wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null \
        && apt-add-repository -y 'deb https://apt.kitware.com/ubuntu/ focal main' \
        && apt update \
        && apt install -y cmake \
        && export CXX=g++-10 \
        && python3 -m pip install --upgrade pip \
        && python3 -m pip install pytest pylint \
        && python3 -m pip install . \
        && cmake -B build -DJUNIT_OUTPUT=yes . \
        && export MAKEFLAGS="-j$(nproc)" \
        && make -C build