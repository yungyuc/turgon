# [Choice] Ubuntu version: bionic, focal
ARG VARIANT=focal
FROM mcr.microsoft.com/vscode/devcontainers/base:${VARIANT}

# Install additional OS packages.
RUN \
  apt-get -qqy update && \
  export DEBIAN_FRONTEND=noninteractive && \
  apt-get -qy install --no-install-recommends \
    sudo curl git build-essential make cmake libc6-dev gcc-9 g++-9 \
    python3 python3-dev python3-venv

# Install pip3 pacakges.
RUN \
  pip3 install setuptools && \
  pip3 install numpy pytest flake8

# vim: set ff=unix ft=dockerfile et sw=2 ts=2 sts=2:
