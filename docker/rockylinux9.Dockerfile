FROM rockylinux/rockylinux:9
ARG branch

COPY . /root/vrtgen

WORKDIR /root/vrtgen

RUN yum -y --nogpgcheck update \
    && yum install -y --nogpgcheck python3-pip gcc-c++ make cmake libarchive \
        libuuid-devel openssl-libs openssl-devel git

# nats.c install
RUN git clone https://github.com/nats-io/nats.c \
    && cd nats.c \
    && cmake -DNATS_BUILD_STREAMING=OFF -B build \
    && cmake --build build \
    && cmake --build build --target install \
    && cd .. \
    && rm -rf nats.c

# Enable updated packages
RUN python3 -m pip install --upgrade pip \
    && python3 -m pip install pytest pylint \
    && python3 -m pip install . \
    && cmake -B build -DJUNIT_OUTPUT=yes -DVRTGEN_BUILD_TESTS=ON . \
    && export MAKEFLAGS="-j$(nproc)" \
    && cmake --build build
