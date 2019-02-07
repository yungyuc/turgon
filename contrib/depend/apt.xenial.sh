# Install compiler and tools.
apt-get -qy install sudo build-essential make cmake libc6-dev gcc-5 g++-5 curl git
# Install Python.
apt-get -qy install python3 cython3 python3-numpy python3-nose python3-pytest
# Install Latex.
apt-get -qy install texlive-base texlive-latex-base texlive-latex-extra \
    texlive-fonts-recommended texlive-pstricks texlive-extra-utils