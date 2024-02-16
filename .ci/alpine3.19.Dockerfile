FROM alpine:3.19
ARG branch

WORKDIR /build

RUN apk update && apk upgrade
RUN apk add \
    build-base \
    cmake \
    py3-pip \
    openssl-dev \
    git

# nats.c install
RUN git clone https://github.com/nats-io/nats.c \
    && cd nats.c \
    && cmake -DNATS_BUILD_STREAMING=OFF -B build \
    && cmake --build build \
    && cmake --build build --target install \
    && cd .. \
    && rm -rf nats.c

COPY . /root/vrtgen

WORKDIR /root/vrtgen

RUN python3 -m venv /opt/venv \
    && . /opt/venv/bin/activate \
    && python3 -m pip install pytest pylint \
    && python3 -m pip install . \
    && cmake -B build -DJUNIT_OUTPUT=yes -DVRTGEN_BUILD_TESTS=ON . \
    && export MAKEFLAGS="-j$(nproc)" \
    && make -C build \
    && deactivate