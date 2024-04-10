FROM centos:7

WORKDIR /build

RUN find /etc/yum.repos.d/ -type f -name "CentOS*.repo" -exec sed -i.bak "s/mirrorlist=/#mirrorlist=/g" {} \;
RUN find /etc/yum.repos.d/ -type f -name "CentOS*.repo" -exec sed -i.bak "s/#baseurl=/baseurl=/g" {} \;
RUN yum -y clean all

RUN yum -y --nogpgcheck update \
    && yum install -y --nogpgcheck epel-release centos-release-scl \
    && yum -y --nogpgcheck update \
    && yum install -y --nogpgcheck make cmake3 devtoolset-11-gcc-c++ rh-python38-python-pip \
        libuuid-devel openssl11-libs openssl11-devel git rh-python38-python-devel

ENV PATH="/opt/rh/rh-python38/root/usr/local/bin:/opt/rh/rh-python38/root/usr/bin:/opt/rh/devtoolset-11/root/usr/bin:${PATH}"
RUN ln -s /opt/rh/rh-python38/root/usr/bin/python3.8 /usr/bin/python3.8

# nats.c install
RUN git clone -b v3.8.0 https://github.com/nats-io/nats.c \
    && cd nats.c \
    && cmake3 -DOPENSSL_ROOT_DIR=/usr/lib64/openssl11 -DOPENSSL_INCLUDE_DIR=/usr/include/openssl11 -DNATS_BUILD_STREAMING=OFF -B build \
    && cmake3 --build build \
    && cmake3 --build build --target install \
    && cd .. \
    && rm -rf nats.c

COPY . /root/vrtgen
WORKDIR /root/vrtgen

RUN git submodule update --init --recursive

RUN python -m pip install --upgrade pip \
    && python -m pip install pytest pylint \
    && python -m pip install . \
    && cmake3 -B build -DJUNIT_OUTPUT=yes -DVRTGEN_BUILD_TESTS=ON . \
    && export MAKEFLAGS="-j$(nproc)" \
    && make -C build