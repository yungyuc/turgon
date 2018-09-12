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
  apt-get -qy install sudo build-essential make cmake libc6-dev gcc-7 g++-7 curl git && \
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

# Set up user and environment.
RUN \
  useradd -m turgon && \
  echo 'turgon	ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers
USER turgon
ENV HOME="/home/turgon"
