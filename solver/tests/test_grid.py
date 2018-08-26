# Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
# BSD 3-Clause License, see COPYING

import unittest

import libst


class GridTC(unittest.TestCase):

    def setUp(self):

        self.grid10 = libst.Grid(0, 10, 10)

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
            str(self.grid10.celm(0, odd_plane=True)),
        )

        self.assertEqual(
            "Celm(even, index=9, x=9.5, xneg=9, xpos=10)",
            str(self.grid10.celm(9, odd_plane=False)),
        )

        with self.assertRaisesRegexp(
            IndexError,
            "Grid::celm_at\(ielm=9, odd_plane=1\): xindex = 20 "
            "outside the interval \[1, 20\)",
        ):
            self.grid10.celm(9, odd_plane=True)

        with self.assertRaisesRegexp(
            IndexError,
            "Grid::celm_at\(ielm=10, odd_plane=0\): xindex = 21 "
            "outside the interval \[1, 20\)",
        ):
            self.grid10.celm(10)


class CelmTC(unittest.TestCase):

    def setUp(self):

        self.grid10 = libst.Grid(0, 10, 10)
        self.elm0 = self.grid10.celm(0)
        self.elm9 = self.grid10.celm(9)

    def test_str(self):

        self.assertEqual(
            "Celm(even, index=0, x=0.5, xneg=0, xpos=1)",
            str(self.elm0),
        )

    def test_grid(self):

        self.assertEqual(self.grid10, self.elm0.grid)

    def test_duplicate(self):

        golden = "Celm(even, index=0, x=0.5, xneg=0, xpos=1)"
        self.assertEqual(golden, str(self.elm0))
        self.assertEqual(golden, str(self.elm0.duplicate()))
        # "dup" attribute is a shorthand for duplicate() function.
        self.assertEqual(golden, str(self.elm0.dup))

        # Duplicated celm has standalone index.
        elm = self.elm0.dup
        elm.move(1)
        self.assertEqual(
            "Celm(odd, index=0, x=1, xneg=0.5, xpos=1.5)",
            str(elm),
        )
        self.assertEqual(
            "Celm(even, index=0, x=0.5, xneg=0, xpos=1)",
            str(self.elm0),
        )

    def test_index(self):

        self.assertEqual(0, self.elm0.index)
        # Moving a half grid makes it on odd plane.
        self.assertEqual(0, self.elm0.dup.move_pos().index)
        # Moving a full grid makes it back on even plane.
        self.assertEqual(1, self.elm0.dup.move_right().index)

    def test_on_even_plane(self):

        self.assertTrue(self.elm0.on_even_plane)
        self.assertFalse(self.elm0.dup.move_pos().on_even_plane)
        self.assertTrue(self.elm0.dup.move_right().on_even_plane)

    def test_coordinates(self):

        self.assertEqual(0.5, self.elm0.x)
        self.assertEqual(0, self.elm0.xneg)
        self.assertEqual(1, self.elm0.xpos)

    def test_move_is_inplace(self):

        # Movement is in-place.
        self.elm0.move(1)
        self.assertEqual(
            "Celm(odd, index=0, x=1, xneg=0.5, xpos=1.5)",
            str(self.elm0),
        )

    def test_move_all(self):

        # Move right/left for a full CE (compound CE).
        self.assertEqual(1.5, self.elm0.dup.move_right().x)
        self.assertEqual(0.5, self.elm0.dup.move_right().move_left().x)

        # Move pos(itive)/neg(itive) for a half CE (basic CE).
        self.assertEqual(1, self.elm0.dup.move_pos().x)
        self.assertEqual(0.5, self.elm0.dup.move_pos().move_neg().x)

       # Out of bound.
        elm0d = self.elm0.dup
        with self.assertRaisesRegexp(
            IndexError,
            "Celm\(xindex=1\)::move_at\(offset=-1\): xindex = 0 "
            "outside the interval \[1, 20\)",
        ):
            elm0d.move_neg()
        elm9d = self.elm9.dup
        with self.assertRaisesRegexp(
            IndexError,
            "Celm\(xindex=19\)::move_at\(offset=1\): xindex = 20 "
            "outside the interval \[1, 20\)",
        ):
            elm9d.move_pos()

class SelmTC(unittest.TestCase):

    def setUp(self):

        self.grid10 = libst.Grid(0, 10, 10)
        self.se0 = self.grid10.selm(0)
        self.se9 = self.grid10.selm(9)
        self.se10 = self.grid10.selm(10)

    def test_str(self):

        self.assertEqual(
            "Selm(index=0, x=0)",
            str(self.se0),
        )

# vim: set et sw=4 ts=4:
