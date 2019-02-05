# Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
# BSD 3-Clause License, see COPYING

import unittest

import numpy as np

import libst


class LinearScalarSolverTC(unittest.TestCase):

    def setUp(self):

        self.resolution = 8
        self.cycle = 10

        # Build grid.
        self.xcrd = np.arange(self.resolution+1) / self.resolution
        self.xcrd *= 2 * np.pi
        self.grid = libst.Grid(self.xcrd)
        self.dx = (self.grid.xmax - self.grid.xmin) / self.grid.ncelm

        # Build solver.
        self.time_stop = 2*np.pi
        cfl_max = 1.0
        dt_max = self.dx * cfl_max
        self.nstep = int(np.ceil(self.time_stop / dt_max))
        self.dt = self.time_stop / self.nstep
        self.cfl = self.dt / self.dx
        self.svr = libst.LinearScalarSolver(
            grid=self.grid,
            time_increment=self.dt,
        )

        # Initialize.
        for e in self.svr.selms(odd_plane=False):
            e.set_so0(0, np.sin(e.xctr))
            e.set_so1(0, np.cos(e.xctr))

    def test_nvar(self):

        self.assertEqual(1, self.svr.nvar)

    def test_initialized(self):

        v = [e.so0(0) for e in self.svr.selms(odd_plane=False)]
        self.assertEqual(v, np.sin(self.xcrd).tolist())

    def test_march(self):

        def _march():

            self.svr.march_half_so0(odd_plane=False)
            self.svr.march_half_so1(odd_plane=False)
            self.svr.treat_so0()
            self.svr.treat_so1()
            self.svr.march_half_so0(odd_plane=True)
            self.svr.march_half_so1(odd_plane=True)

        for it in range(self.nstep*self.cycle):
            _march()

        v = np.array([e.so0(0) for e in self.svr.selms(odd_plane=False)])
        np.testing.assert_allclose(v, np.sin(self.xcrd),
                                   rtol=1.e-14, atol=1.e-15)

# vim: set et sw=4 ts=4:
