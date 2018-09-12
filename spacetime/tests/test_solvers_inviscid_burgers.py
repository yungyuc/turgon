# Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
# BSD 3-Clause License, see COPYING

import unittest

import numpy as np

import libst
from libst.solvers import inviscid_burgers as iburg

class SingleStepTC(unittest.TestCase):

    def setUp(self):

        self.grid = libst.Grid(0, 10, 10)
        self.solver = iburg.Solver(self.grid, 0.1)

    def test_null(self):

        self.assertEqual(np.arange(-0.5, 10.6, 0.5).tolist(),
                         self.grid.xcoord.tolist())

        self.solver.init()

# vim: set et sw=4 ts=4:
