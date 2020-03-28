#!/bin/bash
#

apt-get -qy install \
curl git build-essential make cmake libc6-dev gcc-7 g++-7 \
python3.7 python3.7-dev python3.7-venv
rm /usr/bin/python3
ln -s python3.7 /usr/bin/python3
apt -y install python3-pip
pip3 install -U pip
pip3 install -U numpy pytest setuptools cmake

