# Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
# BSD 3-Clause License, see COPYING

import unittest

import numpy as np

import libst


class LinearScalarSolverTC(unittest.TestCase):

    def setUp(self):

        self.dt = 0.1
        self.grid10 = libst.Grid(0, 10, 10)
        self.sol10 = libst.LinearScalarSolver(
            grid=self.grid10,
            time_increment=self.dt,
        )

        for elm in self.sol10.selms(odd_plane=False):
            r = elm.grid.xmin + elm.xctr / (elm.grid.xmax-elm.grid.xmin)
            v = np.sin(r * np.pi * 2)
            dv = np.cos(r * np.pi * 2) * 2 * np.pi
            elm.set_so0(0, 3) # FIXME: find a generic way to initialize.
            elm.set_so1(0, 0)

    def test_nvar(self):

        self.assertEqual(1, self.sol10.nvar)


# vim: set et sw=4 ts=4:
