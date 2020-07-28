===========================================================
The conservation element and solution element (CESE) method
===========================================================

.. image:: https://mybinder.org/badge_logo.svg
  :target: https://mybinder.org/v2/gh/yungyuc/turgon/master?filepath=notebook

Build Turgon on OS X
====================
Clone from https://github.com/yungyuc/turgon.git
  ``$ git clone https://github.com/yungyuc/turgon.git``

Turgon needs following packages::
 1. C++14
 2. Python3.7
 3. Cmake 3.15.4+
 4. numpy
 5. **pybind11** (recommended build and install locally, otherwise cmake may not find pybind11)
 6. `modmesh <https://github.com/solvcon/modmesh>`_

Make sure all dependencies have been installed.

Now change the current directory to `turgon/spacetime`
 ``$ cd turgon/spacetime``

Run make:
 ``$ make CMAKE_ARGS=-Dmodmesh_INCLUDE_DIR=path_to/modmesh/include``

Test
====
Using gtest to test if turgon is installed properly
 ``$ make test``