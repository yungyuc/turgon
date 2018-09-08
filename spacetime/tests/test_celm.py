# Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
# BSD 3-Clause License, see COPYING

import unittest

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

# vim: set et sw=4 ts=4:
