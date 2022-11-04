FROM rockylinux/rockylinux:8
ARG branch

COPY . /root/vrtgen

WORKDIR /root/vrtgen

RUN yum -y --nogpgcheck update \
    && yum install -y --nogpgcheck python38-pip gcc-toolset-10-gcc-c++ make cmake libarchive \
    # Enable updated packages
    && export PATH=/opt/rh/gcc-toolset-10/root/usr/bin:$PATH \
    && python3 -m pip install --upgrade pip \
    && python3 -m pip install pytest pylint \
    && python3 -m pip install . \
    && cmake -B build -DJUNIT_OUTPUT=yes . \
    && export MAKEFLAGS="-j$(nproc)" \
    && make -C build