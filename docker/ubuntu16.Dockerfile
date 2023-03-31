FROM ubuntu:16.04

COPY . /root/vrtgen

WORKDIR /root/

RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections \
    && apt update \
    && apt upgrade \
    && apt install -y make build-essential libssl-dev zlib1g-dev \
           libbz2-dev libreadline-dev libsqlite3-dev wget curl llvm \
           libncurses5-dev libncursesw5-dev xz-utils tk-dev \
           software-properties-common apt-transport-https \
    && wget https://www.python.org/ftp/python/3.8.10/Python-3.8.10.tgz \
    && tar xf Python-3.8.10.tgz \
    && cd Python-3.8.10 \
    && ./configure --enable-optimizations --with-ensurepip=install \
    && make -j$(nproc) \
    && make altinstall \
    && cd ../ \
    && rm -rf Python-3.8.10* \
    && wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null \
    && apt-add-repository -y 'deb https://apt.kitware.com/ubuntu/ xenial main' \
    && apt update \
    && apt install -y cmake \
    && apt install -y build-essential wget m4 flex bison \
    && wget https://ftpmirror.gnu.org/gcc/gcc-10.3.0/gcc-10.3.0.tar.xz \
    && tar xf gcc-10.3.0.tar.xz \
    && cd gcc-10.3.0 \
    && contrib/download_prerequisites \
    && cd ../ \
    && mkdir -p build && cd build \
    && ../gcc-10.3.0/configure -v --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu --prefix=/usr/local/gcc-10.3.0 --enable-checking=release --enable-languages=c,c++,fortran --disable-multilib --program-suffix=-10 \
    && make -j$(nproc) \
    && make install-strip \
    && cd ../ \
    && rm -rf gcc-10.3.0* \
    && rm -rf build
ENV CXX="g++-10"
ENV MAKEFLAGS="-j$(nproc)"
ENV PATH="/usr/local/gcc-10.3.0/bin:$PATH"
ENV LD_LIBRARY_PATH="/usr/local/gcc-10.3.0/lib64:$LD_LIBRARY_PATH"
WORKDIR /root/vrtgen
RUN python3.8 -m pip install --upgrade pip \
    && python3.8 -m pip install pytest pylint \
    && python3.8 -m pip install . \
    && cmake -B build -DJUNIT_OUTPUT=yes -DVRTGEN_BUILD_TESTS=ON . \
    && make -C build \
    && make -C build install