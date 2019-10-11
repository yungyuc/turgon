#!/bin/bash
#
# Copyright (C) 2018 Yung-Yu Chen <yyc@solvcon.net>.
#
# Build and install xtensor-python.

INSTALL_PREFIX=${INSTALL_PREFIX:-/usr}

install() {

  githuborg=$1
  pkgname=$2
  pkgbranch=$3
  cmakeargs="${@:4}"
  pkgfull=$pkgname-$pkgbranch
  pkgrepo=https://github.com/$githuborg/$pkgname.git
  repotar=https://github.com/$githuborg/$pkgname/archive/$pkgbranch.tar.gz

  workdir=$(mktemp -d /tmp/build.XXXXXXXXX)
  echo "Work directory: $workdir"
  mkdir -p $workdir
  pushd $workdir
  echo "remote tarball: $repotar"
  curl -sSL -o $pkgfull.tar.gz $repotar
  rm -rf $pkgfull
  tar xf $pkgfull.tar.gz
  cd $pkgfull
  mkdir -p build
  cd build
  cmake $cmakeargs ..
  make install
  popd
  rm -rf $workdir

}

pybind11() {

  cmakeargs=("-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}")
  cmakeargs+=("-DCMAKE_BUILD_TYPE=Release")
  cmakeargs+=("-DPYTHON_EXECUTABLE:FILEPATH=`which python3`")
  cmakeargs+=("-DPYBIND11_TEST=OFF")
  install ${PYBIND_ORG:-pybind} pybind11 ${PYBIND_BRANCH:-master} \
    "${cmakeargs[@]}"

}

xtl() {

  cmakeargs=("-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}")
  cmakeargs+=("-DCMAKE_BUILD_TYPE=Release")
  install ${XTL_ORG:-QuantStack} xtl ${XTL_BRANCH:-master} \
    "${cmakeargs[@]}"

}

xsimd() {

  cmakeargs=("-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}")
  cmakeargs+=("-DCMAKE_BUILD_TYPE=Release")
  cmakeargs+=("-DBUILD_TESTS=OFF")
  install ${XSIMD_ORG:-QuantStack} xsimd ${XSIMD_BRANCH:-master} \
    "${cmakeargs[@]}"

}

xtensor() {

  cmakeargs=("-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}")
  cmakeargs+=("-DCMAKE_BUILD_TYPE=Release")
  install ${XTENSOR_ORG:-QuantStack} xtensor ${XTENSOR_BRANCH:-master} \
    "${cmakeargs[@]}"

}

xtensor_python() {

  cmakeargs=("-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}")
  cmakeargs+=("-DCMAKE_BUILD_TYPE=Release")
  install ${XTENSOR_PYTHON_ORG:-QuantStack} xtensor-python \
    ${XTENSOR_PYTHON_BRANCH:-master} "${cmakeargs[@]}"

}

if [ $1 == "pybind11" ]; then
  pybind11
elif [ $1 == "xtl" ]; then
  xtl
elif [ $1 == "xsimd" ]; then
  xsimd
elif [ $1 == "xtensor" ]; then
  xtensor
elif [ $1 == "xtensor-python" ]; then
  xtensor_python
elif [ $1 == "everything" ]; then
  pybind11
  xtl
  xsimd
  xtensor
  xtensor_python
fi
