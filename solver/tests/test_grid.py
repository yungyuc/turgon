# Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
# BSD 3-Clause License, see COPYING

import unittest

import libst


class GridTC(unittest.TestCase):

    def setUp(self):

        self.grid100 = libst.Grid(0, 100, 100)

    def test_str(self):

        self.assertEqual("Grid(xmin=0, xmax=100, nelement=100)",
                         str(self.grid100))

    def test_element(self):

        self.assertEqual(
            "Element(even, index=0, x=0.5, xneg=0, xpos=1)",
            str(self.grid100.element(0)),
        )


class ElementTC(unittest.TestCase):

    def setUp(self):

        self.grid100 = libst.Grid(0, 100, 100)
        self.elm0 = libst.Element(self.grid100, 0)

    def test_str(self):

        self.assertEqual(
            "Element(even, index=0, x=0.5, xneg=0, xpos=1)",
            str(self.elm0),
        )

    def test_grid(self):

        self.assertEqual(self.grid100, self.elm0.grid)

    def test_index(self):

        self.assertEqual(0, self.elm0.index)
        # FIXME: should be 1, 0, respectively
        self.assertEqual(0, self.elm0.duplicate().move_pos().index)
        self.assertEqual(-1, self.elm0.duplicate().move_neg().index)

    def test_on_even_plane(self):

        self.assertTrue(self.elm0.on_even_plane)
        self.assertFalse(self.elm0.duplicate().move_pos().on_even_plane)

    def test_coordinates(self):

        self.assertEqual(0.5, self.elm0.x)
        self.assertEqual(0, self.elm0.xneg)
        self.assertEqual(1, self.elm0.xpos)

    def test_move(self):

        # Movement is in-place.
        self.elm0.move(1)
        self.assertEqual(
            "Element(odd, index=0, x=1, xneg=0.5, xpos=1.5)",
            str(self.elm0),
        )

        # Duplicated element has standalone index.
        elm = self.elm0.duplicate()
        elm.move(1)
        self.assertEqual(
            "Element(even, index=1, x=1.5, xneg=1, xpos=2)",
            str(elm),
        )
        self.assertEqual(
            "Element(odd, index=0, x=1, xneg=0.5, xpos=1.5)",
            str(self.elm0),
        )

        # FIXME: test for out-of-bound.

    def test_move_more(self):

        self.assertEqual(1.5, self.elm0.duplicate().move_right().x)
        self.assertEqual(0.5, self.elm0.duplicate().move_right().move_left().x)
        self.assertEqual(1, self.elm0.duplicate().move_pos().x)
        self.assertEqual(0.5, self.elm0.duplicate().move_pos().move_neg().x)

# vim: set et sw=4 ts=4:
