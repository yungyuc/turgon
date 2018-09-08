# Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
# BSD 3-Clause License, see COPYING

import unittest

import libst


class GridTC(unittest.TestCase):

    def setUp(self):

        self.grid10 = libst.Grid(xmin=0.0, xmax=10.0, nelm=10)

    def test_construction(self):

        with self.assertRaisesRegexp(
            ValueError,
            "Grid::Grid\(xmin=0, xmax=10, ncelm=0\) invalid argument: "
            "ncelm smaller than 1",
        ):
            libst.Grid(0, 10, 0)

        with self.assertRaisesRegexp(
            ValueError,
            "Grid::Grid\(xmin=10, xmax=10, ncelm=10\) invalid arguments: "
            "xmin >= xmax",
        ):
            libst.Grid(10, 10, 10)

        with self.assertRaisesRegexp(
            ValueError,
            "Grid::Grid\(xmin=11, xmax=10, ncelm=10\) invalid arguments: "
            "xmin >= xmax",
        ):
            libst.Grid(11, 10, 10)

    def test_number(self):

        self.assertEqual(10, self.grid10.ncelm)
        self.assertEqual(11, self.grid10.nselm)

    def test_str(self):

        self.assertEqual("Grid(xmin=0, xmax=10, ncelm=10)",
                         str(self.grid10))

    def test_celm(self):

        with self.assertRaises(TypeError):
            self.grid10.celm(-1)

        self.assertEqual(
            "Celm(even, index=0, x=0.5, xneg=0, xpos=1)",
            str(self.grid10.celm(0)),
        )

        self.assertEqual(
            "Celm(odd, index=0, x=1, xneg=0.5, xpos=1.5)",
            str(self.grid10.celm(ielm=0, odd_plane=True)),
        )

        self.assertEqual(
            "Celm(even, index=9, x=9.5, xneg=9, xpos=10)",
            str(self.grid10.celm(9, odd_plane=False)),
        )

        with self.assertRaisesRegexp(
            IndexError,
            "Grid::celm_at\(ielm=9, odd_plane=1\): xindex = 21 "
            "outside the interval \[2, 21\)",
        ):
            self.grid10.celm(9, odd_plane=True)

        with self.assertRaisesRegexp(
            IndexError,
            "Grid::celm_at\(ielm=10, odd_plane=0\): xindex = 22 "
            "outside the interval \[2, 21\)",
        ):
            self.grid10.celm(10)

# vim: set et sw=4 ts=4:
