# Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
# BSD 3-Clause License, see COPYING

import unittest

import libst


class GridTC(unittest.TestCase):

    def setUp(self):

        self.grid10 = libst.Grid(0, 10, 10)

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
            str(self.grid10.celm(0, odd_plane=True)),
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


    def test_selm(self):

        with self.assertRaises(TypeError):
            self.grid10.selm(-1)

        self.assertEqual(
            "Selm(even, index=0, x=0, xneg=-0.5, xpos=0.5)",
            str(self.grid10.selm(0)),
        )

        self.assertEqual(
            "Selm(odd, index=0, x=0.5, xneg=0, xpos=1)",
            str(self.grid10.selm(0, odd_plane=True)),
        )

        self.assertEqual(
            "Selm(even, index=10, x=10, xneg=9.5, xpos=10.5)",
            str(self.grid10.selm(10, odd_plane=False)),
        )

        with self.assertRaisesRegexp(
            IndexError,
            "Grid::selm_at\(ielm=10, odd_plane=1\): xindex = 22 "
            "outside the interval \[1, 22\)",
        ):
            self.grid10.selm(10, odd_plane=True)

        with self.assertRaisesRegexp(
            IndexError,
            "Grid::selm_at\(ielm=11, odd_plane=0\): xindex = 23 "
            "outside the interval \[1, 22\)",
        ):
            self.grid10.selm(11)

    def test_celm_in(self):

        for it, ce in enumerate(libst.celm_in(self.grid10)):
            self.assertEqual(str(self.grid10.celm(it)), str(ce))
        for it, ce in enumerate(libst.celm_in(self.grid10, odd_plane=True)):
            self.assertEqual(str(self.grid10.celm(it, odd_plane=True)), str(ce))

        for it, se in enumerate(libst.selm_in(self.grid10)):
            self.assertEqual(str(self.grid10.selm(it)), str(se))
        for it, se in enumerate(libst.selm_in(self.grid10, odd_plane=True)):
            self.assertEqual(str(self.grid10.selm(it, odd_plane=True)), str(se))


class CelmTC(unittest.TestCase):

    def setUp(self):

        self.grid10 = libst.Grid(0, 10, 10)
        self.ce0 = self.grid10.celm(0)
        self.ce9 = self.grid10.celm(9)

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
        self.se0 = self.grid10.selm(0)
        self.se9 = self.grid10.selm(9)
        self.se10 = self.grid10.selm(10)

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
        self.assertFalse(self.se0.dup.move_pos().on_even_plane)
        self.assertTrue(self.se0.dup.move_right().on_even_plane)

        self.assertFalse(self.grid10.selm(5, odd_plane=True).on_even_plane)
        self.assertTrue(self.grid10.selm(5, odd_plane=False).on_even_plane)

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
