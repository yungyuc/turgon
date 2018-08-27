# This dockerfile builds a docker image for building turgon with gitlab ci:
# https://gitlab.com/yungyuc/turgon/pipelines .
#
# To build the image locally, run:
#
# ```
# docker build . [-t registry.gitlab.com/yungyuc/turgon:latest]
# ```
#
# The public repository for this image is at:
# https://gitlab.com/yungyuc/turgon/container_registry .  You can pull it
# using:
#
# ```
# docker pull registry.gitlab.com/yungyuc/turgon
# ```


FROM ubuntu:18.04
MAINTAINER Yung-Yu Chen <yyc@solvcon.net>

ARG turgon_repo=https://gitlab.com/solvcon/turgon
ARG turgon_branch=master

# Install compiler and tools.
RUN \
  export DEBIAN_FRONTEND=noninteractive && \
  apt-get -qq update && \
  apt-get -qy install tzdata && \
  ln -fs /usr/share/zoneinfo/Asia/Taipei /etc/localtime && \
  dpkg-reconfigure --frontend noninteractive tzdata && \
  apt-get -qy install build-essential make cmake libc6-dev gcc-7 g++-7 curl git && \
  rm -rf /var/lib/apt/lists/*

# Install Python.
RUN \
  export DEBIAN_FRONTEND=noninteractive && \
  apt-get -qq update && \
  apt-get -qy install python3 cython3 python3-numpy python3-nose python3-pytest && \
  rm -rf /var/lib/apt/lists/*

# Install Latex.
RUN \
  export DEBIAN_FRONTEND=noninteractive && \
  apt-get -qq update && \
  apt-get -qy install texlive-base texlive-latex-base texlive-latex-extra texlive-pstricks && \
  rm -rf /var/lib/apt/lists/*

# Pybind11.
RUN \
  cd /tmp && \
  curl -sSL -o pybind11-master.tar.gz https://github.com/pybind/pybind11/archive/master.tar.gz && \
  rm -rf pybind11-master && \
  tar xf pybind11-master.tar.gz && \
  mkdir -p pybind11-master/build && \
  cd pybind11-master/build && \
  cmake -DPYBIND11_TEST=OFF -DCMAKE_BUILD_TYPE=Release -DPYTHON_EXECUTABLE:FILEPATH=`which python3` -DCMAKE_INSTALL_PREFIX=/usr .. && \
  make install && \
  rm -rf /tmp/pybind11*

# xtl.
RUN \
  cd /tmp && \
  curl -sSL -o xtl-master.tar.gz https://github.com/QuantStack/xtl/archive/master.tar.gz && \
  rm -rf xtl-master && \
  tar xf xtl-master.tar.gz && \
  mkdir -p xtl-master/build && \
  cd xtl-master/build && \
  cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr .. && \
  make install && \
  rm -rf /tmp/xtl*

# xsimd.
RUN \
  cd /tmp && \
  curl -sSL -o xsimd-master.tar.gz https://github.com/QuantStack/xsimd/archive/master.tar.gz && \
  rm -rf xsimd-master && \
  tar xf xsimd-master.tar.gz && \
  mkdir -p xsimd-master/build && \
  cd xsimd-master/build && \
  cmake -DBUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr .. && \
  make install && \
  rm -rf /tmp/xsimd*

# xtensor.
RUN \
  cd /tmp && \
  curl -sSL -o xtensor-master.tar.gz https://github.com/QuantStack/xtensor/archive/master.tar.gz && \
  rm -rf xtensor-master && \
  tar xf xtensor-master.tar.gz && \
  mkdir -p xtensor-master/build && \
  cd xtensor-master/build && \
  cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr .. && \
  make install && \
  rm -rf /tmp/xtensor*

# Set up user and environment.
RUN useradd -m turgon
USER turgon
ENV HOME="/home/turgon"
