#The conservation element and solution element (CESE) method


[![image](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/yungyuc/turgon/master?filepath=notebookrr)

##Build Turgon on OS X

###Requirements
* C++14
* Python3.7
* Cmake 3.15.4+
* numpy
* **pybind11**(recommended build and install locally, otherwise cmake may not find pybind11)
* [modmesh](https://github.com/solvcon/modmesh.git)

###Build
1. Clone from [Turgon](https://github.com/yungyuc/turgon.git)
`$git clone https://github.com/yungyuc/turgon.git`

2. `$cd turgon/spacetime`

3. Make sure all dependencies have been installed.
4. '$make CMAKE_ARGS=-Dmodmesh_INCLUDE_DIR=path_to/modmesh/include'

###Test
Using gtest to test if turgon is installed properly.
`$make test`