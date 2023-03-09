FROM centos:7

COPY . /root/vrtgen

WORKDIR /root/vrtgen

RUN find /etc/yum.repos.d/ -type f -name "CentOS*.repo" -exec sed -i.bak "s/mirrorlist=/#mirrorlist=/g" {} \;
RUN find /etc/yum.repos.d/ -type f -name "CentOS*.repo" -exec sed -i.bak "s/#baseurl=/baseurl=/g" {} \;
RUN yum -y clean all

RUN yum -y --nogpgcheck update \
    && yum install -y --nogpgcheck epel-release centos-release-scl \
    && yum -y --nogpgcheck update \
    && yum install -y --nogpgcheck make cmake3 devtoolset-11-gcc-c++ rh-python38-python-pip 

ENV PATH="/opt/rh/rh-python38/root/usr/local/bin:/opt/rh/rh-python38/root/usr/bin:/opt/rh/devtoolset-11/root/usr/bin:${PATH}"
RUN ln -s /opt/rh/rh-python38/root/usr/bin/python3.8 /usr/bin/python3.8

RUN python -m pip install --upgrade pip \
    && python -m pip install pytest pylint \
    && python -m pip install . \
    && cmake3 -B build -DJUNIT_OUTPUT=yes -DBUILD_TESTS=ON . \
    && export MAKEFLAGS="-j$(nproc)" \
    && make -C build