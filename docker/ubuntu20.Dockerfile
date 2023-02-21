FROM ubuntu:20.04
ARG branch

COPY . /root/vrtgen

WORKDIR /root/vrtgen

RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections \
        && apt update 

RUN apt install -f -m \
                -o Acquire::BrokenProxy="true" \
                -o Acquire::http::No-Cache="true" \
                -o Acquire::http::Pipeline-Depth="0" \
                -y g++-10 python3-pip wget software-properties-common

RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null \
    && apt-add-repository -y 'deb https://apt.kitware.com/ubuntu/ focal main' \
    && apt update \
    && apt install -f -m \
                   -o Acquire::BrokenProxy="true" \
                   -o Acquire::http::No-Cache="true" \
                   -o Acquire::http::Pipeline-Depth="0" \
                   -y cmake

RUN export CXX=g++-10 \
    && python3 -m pip install --upgrade pip \
    && python3 -m pip install pytest pylint \
    && python3 -m pip install . \
    && cmake -B build -DJUNIT_OUTPUT=yes -DBUILD_TESTS=ON . \
    && export MAKEFLAGS="-j$(nproc)" \
    && make -C build