# This dockerfile builds a docker image for building turgon with gitlab ci:
# https://gitlab.com/yungyuc/turgon/pipelines .
#
# To build the image locally, run:
#
# ```
# docker build  .
# ```
#
# The public repository for this image is at:
# https://hub.docker.com/r/yungyuc/turgon/ .  You can pull it down
# using:
#
# ```
# docker pull yungyuc/turgon
# ```


FROM ubuntu:18.04
MAINTAINER Yung-Yu Chen <yyc@solvcon.net>

ARG turgon_repo=https://gitlab.com/solvcon/turgon
ARG turgon_branch=master

# Install compiler and tools.
RUN \
  export DEBIAN_FRONTEND=noninteractive && \
  apt-get -qq update && \
  apt-get -qqy install tzdata && \
  ln -fs /usr/share/zoneinfo/Asia/Taipei /etc/localtime && \
  dpkg-reconfigure --frontend noninteractive tzdata && \
  apt-get -qqy install build-essential g++ liblapack-dev curl git unzip python3 && \
  rm -rf /var/lib/apt/lists/*

# Install Latex.
RUN \
  apt-get -qq update && \
  apt-get -qqy install texlive-base texlive-latex-base texlive-latex-extra texlive-pstricks && \
  rm -rf /var/lib/apt/lists/*

# Set up user and environment.
RUN useradd -m turgon
USER turgon
ENV HOME="/home/turgon"
