FROM ubuntu:22.04
ARG branch

COPY . /root/vrtgen

WORKDIR /root/vrtgen

RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections \
        && apt update 

RUN apt install -f -m \
                -o Acquire::BrokenProxy="true" \
                -o Acquire::http::No-Cache="true" \
                -o Acquire::http::Pipeline-Depth="0" \
                -y g++-11 python3-pip software-properties-common uuid-dev libssl-dev git cmake

# nats.c install
RUN export CXX=g++-11 \
    && git clone -b v3.8.0 https://github.com/nats-io/nats.c \
    && cd nats.c \
    && cmake -DNATS_BUILD_STREAMING=OFF -B build \
    && cmake --build build \
    && cmake --build build --target install \
    && cd .. \
    && rm -rf nats.c

RUN export CXX=g++-11 \
    && python3 -m pip install --upgrade pip \
    && python3 -m pip install pytest pylint \
    && python3 -m pip install . \
    && cmake -B build -DJUNIT_OUTPUT=yes -DVRTGEN_BUILD_TESTS=ON . \
    && export MAKEFLAGS="-j$(nproc)" \
    && make -C build