# Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
# BSD 3-Clause License, see COPYING

import unittest

import libst


class SolutionTC(unittest.TestCase):

    def setUp(self):

        self.grid10 = libst.Grid(0, 10, 10)
        self.sol10 = libst.Solution(grid=self.grid10, nvar=1,
                                    time_increment=0.2)

    def test_str(self):

        self.assertEqual("Solution(grid=Grid(xmin=0, xmax=10, ncelm=10))",
                         str(self.sol10))

    def test_grid(self):

        self.assertEqual(self.grid10, self.sol10.grid)

    def test_nvar(self):

        self.assertEqual(1, self.sol10.nvar)

    def test_time_increment(self):

        self.assertEqual(0.2, self.sol10.time_increment)
        self.sol10.time_increment = 42
        self.assertEqual(42, self.sol10.time_increment)

    def test_selm(self):

        with self.assertRaises(TypeError):
            self.sol10.selm(-1)

        self.assertEqual(
            "Selm(even, index=0, x=0, xneg=-0.5, xpos=0.5)",
            str(self.sol10.selm(0)),
        )

        self.assertEqual(
            "Selm(odd, index=0, x=0.5, xneg=0, xpos=1)",
            str(self.sol10.selm(0, odd_plane=True)),
        )

        self.assertEqual(
            "Selm(even, index=10, x=10, xneg=9.5, xpos=10.5)",
            str(self.sol10.selm(10, odd_plane=False)),
        )

        with self.assertRaisesRegexp(
            IndexError,
            "Grid::selm_at\(ielm=10, odd_plane=1\): xindex = 22 "
            "outside the interval \[1, 22\)",
        ):
            self.sol10.selm(10, odd_plane=True)

        with self.assertRaisesRegexp(
            IndexError,
            "Grid::selm_at\(ielm=11, odd_plane=0\): xindex = 23 "
            "outside the interval \[1, 22\)",
        ):
            self.sol10.selm(11)

# vim: set et sw=4 ts=4:
