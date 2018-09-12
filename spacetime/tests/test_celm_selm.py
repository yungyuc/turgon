# Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
# BSD 3-Clause License, see COPYING

import unittest

import numpy as np

import libst


class CelmTC(unittest.TestCase):

    def setUp(self):

        self.grid10 = libst.Grid(0, 10, 10)
        self.ce0 = self.grid10.celm(ielm=0)
        self.ce9 = self.grid10.celm(ielm=9)

    def test_str(self):

        self.assertEqual(
            "Celm(even, index=0, x=0.5, xneg=0, xpos=1)",
            str(self.ce0),
        )

    def test_grid(self):

        self.assertEqual(self.grid10, self.ce0.grid)

    def test_duplicate(self):

        golden = "Celm(even, index=0, x=0.5, xneg=0, xpos=1)"
        self.assertEqual(golden, str(self.ce0))
        self.assertEqual(golden, str(self.ce0.duplicate()))
        # "dup" attribute is a shorthand for duplicate() function.
        self.assertEqual(golden, str(self.ce0.dup))

        # Duplicated celm has standalone index.
        elm = self.ce0.dup
        elm.move(1)
        self.assertEqual(
            "Celm(odd, index=0, x=1, xneg=0.5, xpos=1.5)",
            str(elm),
        )
        self.assertEqual(
            "Celm(even, index=0, x=0.5, xneg=0, xpos=1)",
            str(self.ce0),
        )

    def test_index(self):

        self.assertEqual(0, self.ce0.index)
        # Moving a half grid makes it on odd plane.
        self.assertEqual(0, self.ce0.dup.move_pos().index)
        # Moving a full grid makes it back on even plane.
        self.assertEqual(1, self.ce0.dup.move_right().index)

    def test_on_even_plane(self):

        self.assertTrue(self.ce0.on_even_plane)
        self.assertFalse(self.ce0.on_odd_plane)
        self.assertFalse(self.ce0.dup.move_pos().on_even_plane)
        self.assertTrue(self.ce0.dup.move_right().on_even_plane)

    def test_coordinates(self):

        self.assertEqual(0.5, self.ce0.x)
        self.assertEqual(0, self.ce0.xneg)
        self.assertEqual(1, self.ce0.xpos)

    def test_move_is_inplace(self):

        # Movement is in-place.
        self.ce0.move(1)
        self.assertEqual(
            "Celm(odd, index=0, x=1, xneg=0.5, xpos=1.5)",
            str(self.ce0),
        )

    def test_move_all(self):

        # Move right/left for a full CE (compound CE).
        self.assertEqual(1.5, self.ce0.dup.move_right().x)
        self.assertEqual(0.5, self.ce0.dup.move_right().move_left().x)

        # Move pos(itive)/neg(itive) for a half CE (basic CE).
        self.assertEqual(1, self.ce0.dup.move_pos().x)
        self.assertEqual(0.5, self.ce0.dup.move_pos().move_neg().x)

       # Out of bound.
        ce0d = self.ce0.dup
        with self.assertRaisesRegexp(
            IndexError,
            "Celm\(xindex=2\)::move_at\(offset=-1\): xindex = 1 "
            "outside the interval \[2, 21\)",
        ):
            ce0d.move_neg()
        ce9d = self.ce9.dup
        with self.assertRaisesRegexp(
            IndexError,
            "Celm\(xindex=20\)::move_at\(offset=1\): xindex = 21 "
            "outside the interval \[2, 21\)",
        ):
            ce9d.move_pos()


class SelmTC(unittest.TestCase):

    def setUp(self):

        self.grid10 = libst.Grid(0, 10, 10)
        self.sol10 = libst.Solution(grid=self.grid10, nvar=1,
                                    time_increment=0.2)
        self.se0 = self.sol10.selm(0)
        self.se9 = self.sol10.selm(9)
        self.se10 = self.sol10.selm(10)

    def test_str(self):

        self.assertEqual(
            "Selm(even, index=0, x=0, xneg=-0.5, xpos=0.5)",
            str(self.se0),
        )
        self.assertEqual(
            "Selm(even, index=9, x=9, xneg=8.5, xpos=9.5)",
            str(self.se9),
        )
        self.assertEqual(
            "Selm(even, index=10, x=10, xneg=9.5, xpos=10.5)",
            str(self.se10),
        )

    def test_grid(self):

        self.assertEqual(self.grid10, self.se0.grid)

    def test_duplicate(self):

        golden = "Selm(even, index=0, x=0, xneg=-0.5, xpos=0.5)"
        self.assertEqual(golden, str(self.se0))
        self.assertEqual(golden, str(self.se0.duplicate()))
        # "dup" attribute is a shorthand for duplicate() function.
        self.assertEqual(golden, str(self.se0.dup))

        # Duplicated selm has standalone index.
        se = self.se0.dup
        se.move(1)
        self.assertEqual(
            "Selm(odd, index=0, x=0.5, xneg=0, xpos=1)",
            str(se),
        )
        self.assertEqual(
            "Selm(even, index=0, x=0, xneg=-0.5, xpos=0.5)",
            str(self.se0),
        )

    def test_index(self):

        self.assertEqual(0, self.se0.index)
        # Moving a half grid makes it on odd plane.
        self.assertEqual(0, self.se0.dup.move_pos().index)
        # Moving a full grid makes it back on even plane.
        self.assertEqual(1, self.se0.dup.move_right().index)

    def test_on_even_plane(self):

        self.assertTrue(self.se0.on_even_plane)
        self.assertFalse(self.se0.on_odd_plane)
        self.assertFalse(self.se0.dup.move_pos().on_even_plane)
        self.assertTrue(self.se0.dup.move_right().on_even_plane)

        self.assertFalse(self.sol10.selm(5, odd_plane=True).on_even_plane)
        self.assertTrue(self.sol10.selm(5, odd_plane=False).on_even_plane)

    def test_coordinates(self):

        self.assertEqual(0, self.se0.x)
        self.assertEqual(-0.5, self.se0.xneg)
        self.assertEqual(0.5, self.se0.xpos)

    def test_move_is_inplace(self):

        # Movement is in-place.
        self.se0.move(1)
        self.assertEqual(
            "Selm(odd, index=0, x=0.5, xneg=0, xpos=1)",
            str(self.se0),
        )

    def test_move_all(self):

        # Move right/left for a full CE (compound CE).
        self.assertEqual(1, self.se0.dup.move_right().x)
        self.assertEqual(0, self.se0.dup.move_right().move_left().x)

        # Move pos(itive)/neg(itive) for a half CE (basic CE).
        self.assertEqual(0.5, self.se0.dup.move_pos().x)
        self.assertEqual(0, self.se0.dup.move_pos().move_neg().x)

       # Out of bound.
        se0d = self.se0.dup
        with self.assertRaisesRegexp(
            IndexError,
            "Selm\(xindex=1\)::move_at\(offset=-1\): xindex = 0 "
            "outside the interval \[1, 22\)",
        ):
            se0d.move_neg()
        se10d = self.se10.dup
        with self.assertRaisesRegexp(
            IndexError,
            "Selm\(xindex=21\)::move_at\(offset=1\): xindex = 22 "
            "outside the interval \[1, 22\)",
        ):
            se10d.move_pos()


class NonUniformTC(unittest.TestCase):

    def setUp(self):

        xloc = np.array([0, 1, 3, 10])
        self.grid3 = libst.Grid(xloc=xloc)
        self.ce0 = self.grid3.celm(ielm=0)
        self.ce1 = self.grid3.celm(ielm=1)
        self.ce2 = self.grid3.celm(ielm=2)
        self.sol3 = libst.Solution(grid=self.grid3, nvar=1, time_increment=0.1)
        self.se0 = self.sol3.selm(ielm=0)
        self.se1 = self.sol3.selm(ielm=1)
        self.se2 = self.sol3.selm(ielm=2)
        self.se3 = self.sol3.selm(ielm=3)

    def test_ce_xcoord(self):

        def get_xcoord(elm):
            return [elm.x, elm.xneg, elm.xctr, elm.xpos]

        for it in range(3):
            ce = getattr(self, 'ce%d'%it)
            self.assertEqual(ce.xctr, ce.x)

        self.assertEqual([0.5, 0, 0.5, 1], get_xcoord(self.ce0))
        self.assertEqual([2, 1, 2, 3], get_xcoord(self.ce1))
        self.assertEqual([6.5, 3, 6.5, 10], get_xcoord(self.ce2))

    def test_se_xcoord(self):

        def get_xcoord(elm):
            return [elm.x, elm.xneg, elm.xctr, elm.xpos]

        for it in [0, 3]:
            se = getattr(self, 'se%d'%it)
            self.assertEqual(se.xctr, se.x)
        for it in [1, 2]:
            se = getattr(self, 'se%d'%it)
            self.assertNotEqual(se.xctr, se.x)

        self.assertEqual([0.0, -0.5, 0.0, 0.5], get_xcoord(self.se0))
        self.assertEqual([1.0, 0.5, 1.25, 2.0], get_xcoord(self.se1))
        self.assertEqual([3.0, 2.0, 4.25, 6.5], get_xcoord(self.se2))
        self.assertEqual([10.0, 6.5, 10.0, 13.5], get_xcoord(self.se3))

# vim: set et sw=4 ts=4:
