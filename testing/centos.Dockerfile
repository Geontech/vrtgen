FROM centos:7
ARG branch

COPY . /root/vrtgen

WORKDIR /root/vrtgen

RUN yum -y update \
    && yum install -y epel-release centos-release-scl \
    && yum install -y make cmake3 devtoolset-10-gcc-c++ rh-python38-python-pip which 

ENV PATH="/opt/rh/rh-python38/root/usr/local/bin:/opt/rh/rh-python38/root/usr/bin:/opt/rh/devtoolset-10/root/usr/bin:${PATH}"

RUN python -m pip install --upgrade pip \
    && python -m pip install pytest pylint \
    && python -m pip install . \
    && cmake3 -B build -DJUNIT_OUTPUT=yes . \
    && export MAKEFLAGS="-j$(nproc)" \
    && make -C build