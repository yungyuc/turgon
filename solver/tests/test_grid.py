# Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
# BSD 3-Clause License, see COPYING

import unittest

import libst


class GridTC(unittest.TestCase):

    def setUp(self):

        self.grid10 = libst.Grid(0, 10, 10)

    def test_str(self):

        self.assertEqual("Grid(xmin=0, xmax=10, nelement=10)",
                         str(self.grid10))

    def test_element(self):

        with self.assertRaises(TypeError):
            self.grid10.element(-1)

        self.assertEqual(
            "Element(even, index=0, x=0.5, xneg=0, xpos=1)",
            str(self.grid10.element(0)),
        )

        self.assertEqual(
            "Element(odd, index=0, x=1, xneg=0.5, xpos=1.5)",
            str(self.grid10.element(0, odd_plane=True)),
        )

        self.assertEqual(
            "Element(even, index=9, x=9.5, xneg=9, xpos=10)",
            str(self.grid10.element(9, odd_plane=False)),
        )

        with self.assertRaisesRegexp(
            IndexError,
            "Grid::element_at\(ielm=9, odd_plane=1\) icrd = 20 "
            "outside the interval \[1, 20\)",
        ):
            self.grid10.element(9, odd_plane=True)

        with self.assertRaisesRegexp(
            IndexError,
            "Grid::element_at\(ielm=10, odd_plane=0\) icrd = 21 "
            "outside the interval \[1, 20\)",
        ):
            self.grid10.element(10)


class ElementTC(unittest.TestCase):

    def setUp(self):

        self.grid10 = libst.Grid(0, 10, 10)
        self.elm0 = self.grid10.element(0)
        self.elm9 = self.grid10.element(9)

    def test_str(self):

        self.assertEqual(
            "Element(even, index=0, x=0.5, xneg=0, xpos=1)",
            str(self.elm0),
        )

    def test_grid(self):

        self.assertEqual(self.grid10, self.elm0.grid)

    def test_duplicate(self):

        golden = "Element(even, index=0, x=0.5, xneg=0, xpos=1)"
        self.assertEqual(golden, str(self.elm0))
        self.assertEqual(golden, str(self.elm0.duplicate()))
        # "dup" attribute is a shorthand for duplicate() function.
        self.assertEqual(golden, str(self.elm0.dup))

        # Duplicated element has standalone index.
        elm = self.elm0.dup
        elm.move(1)
        self.assertEqual(
            "Element(odd, index=0, x=1, xneg=0.5, xpos=1.5)",
            str(elm),
        )
        self.assertEqual(
            "Element(even, index=0, x=0.5, xneg=0, xpos=1)",
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
            "Element(odd, index=0, x=1, xneg=0.5, xpos=1.5)",
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
            "Element::move_at\(\) \(coord_index = 1, offset = -1\) icrd = 0 "
            "outside the interval \[1, 20\)",
        ):
            elm0d.move_neg()
        elm9d = self.elm9.dup
        with self.assertRaisesRegexp(
            IndexError,
            "Element::move_at\(\) \(coord_index = 19, offset = 1\) icrd = 20 "
            "outside the interval \[1, 20\)",
        ):
            elm9d.move_pos()

# vim: set et sw=4 ts=4:
