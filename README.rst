===========================================================
The conservation element and solution element (CESE) method
===========================================================

.. image:: https://mybinder.org/badge_logo.svg
  :target: https://mybinder.org/v2/gh/yungyuc/turgon/master?filepath=notebook

Build Turgon on OS X
====================

Clone from https://github.com/yungyuc/turgon.git

.. code-block:: bash

  $ git clone https://github.com/yungyuc/turgon.git

Turgon needs following packages:

1. C++14
2. Python3.7
3. Cmake 3.15.4+
4. numpy
5. **pybind11** (recommended build and install locally, otherwise cmake may not find pybind11)
6. `modmesh <https://github.com/solvcon/modmesh>`_

Make sure all dependencies have been installed.

Now change the current directory to `turgon/spacetime`

.. code-block:: bash

  $ cd turgon/spacetime

Run make:

.. code-block:: bash

  $ make CMAKE_ARGS=-Dmodmesh_INCLUDE_DIR=path_to/modmesh/include

Test
====

Using gtest to test if turgon is installed properly.

.. code-block:: bash

  $ make test

How to get development note
===========================

Turgon have some development note, in order to get these note, the `Latex <http://www.tug.org/>`_ is needed to install.

If you use **Mac OS**, you can check the `MacTEX <http://www.tug.org/mactex/>`_ or:

.. code-block:: bash

  $ brew cask install mactex

If you use **Linux**, you can check the `texlive <https://www.tug.org/texlive/>`_ or:

.. code-block:: bash

  $ sudo apt-get install texlive-full

Now change you current directory to ``writing``

.. code-block:: bash

  $ cd writing

then build the note:

.. code-block:: bash

  $ make note

