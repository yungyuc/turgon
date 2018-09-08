# Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
# BSD 3-Clause License, see COPYING

import unittest

import libst


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

# vim: set et sw=4 ts=4:
