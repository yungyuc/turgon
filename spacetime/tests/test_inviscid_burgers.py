# Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
# BSD 3-Clause License, see COPYING

import unittest

import numpy as np

import libst


class InviscidBurgersSolutionTC(unittest.TestCase):

    def setUp(self):

        self.dt = 0.1
        self.grid10 = libst.Grid(0, 10, 10)
        self.sol10 = libst.InviscidBurgersSolution(
            grid=self.grid10, time_increment=self.dt)

        self.sol10.so0.fill(1)
        self.sol10.so1.fill(1)
        for elm in libst.selm_in(self.sol10, odd_plane=False):
            r = elm.grid.xmin + elm.xctr / (elm.grid.xmax-elm.grid.xmin)
            v = np.sin(r * np.pi * 2)
            dv = np.cos(r * np.pi * 2) * 2 * np.pi
            elm.set_so0(0, 3) # FIXME: find a generic way to initialize.
            elm.set_so1(0, 0)

    def test_nvar(self):

        self.assertEqual(1, self.sol10.nvar)

    def test_march_half_so0(self):

        self.sol10.march_half_so0(odd_plane=False)
        res = [s.so0(0) for s in libst.selm_in(self.sol10, odd_plane=True)]
        self.assertEqual(10, len(res))
        self.assertEqual([3]*10, res)

    def test_march_half_so1(self):

        self.sol10.march_half_so1(odd_plane=False)
        res = [s.so0(0) for s in libst.selm_in(self.sol10, odd_plane=True)]
        self.assertEqual(10, len(res))
        self.assertEqual([3]*10, res)


class InviscidBurgersSelm(unittest.TestCase):

    def setUp(self):
        self.dt = 0.2
        self.grid10 = libst.Grid(0, 10, 10)
        self.sol10 = libst.InviscidBurgersSolution(
            grid=self.grid10, time_increment=self.dt)
        self.fe0 = self.sol10.felm(0)
        self.fe9 = self.sol10.felm(9)
        self.fe10 = self.sol10.felm(10)
        # Set trivial solution.
        self.sol10.so0.fill(1)
        self.sol10.so1.fill(0)

    def test_flux(self):

        self.assertEqual(0.5, self.fe0.xn(0))
        self.assertEqual(0.5, self.fe0.xp(0))
        self.assertEqual(0.05, self.fe0.tn(0))
        self.assertEqual(0.05, self.fe0.tp(0))

# vim: set et sw=4 ts=4:
