FROM rockylinux/rockylinux:8
ARG branch

COPY . /root/vrtgen

WORKDIR /root/vrtgen

RUN yum -y --nogpgcheck update \
    && yum install -y --nogpgcheck python38-pip gcc-toolset-11-gcc-c++ make cmake libarchive \
        libuuid-devel openssl-libs openssl-devel git

# nats.c install
RUN export PATH=/opt/rh/gcc-toolset-11/root/usr/bin:$PATH \
    && git clone https://github.com/nats-io/nats.c \
    && cd nats.c \
    && cmake -DNATS_BUILD_STREAMING=OFF -B build \
    && cmake --build build \
    && cmake --build build --target install \
    && cd .. \
    && rm -rf nats.c

# Enable updated packages
RUN export PATH=/opt/rh/gcc-toolset-11/root/usr/bin:$PATH \
    && python3 -m pip install --upgrade pip \
    && python3 -m pip install pytest pylint \
    && python3 -m pip install . \
    && cmake -B build -DJUNIT_OUTPUT=yes -DBUILD_TESTS=ON . \
    && export MAKEFLAGS="-j$(nproc)" \
    && cmake --build build