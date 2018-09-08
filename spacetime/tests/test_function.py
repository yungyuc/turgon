# Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
# BSD 3-Clause License, see COPYING

import unittest

import libst


class FunctionTC(unittest.TestCase):

    def setUp(self):

        self.grid10 = libst.Grid(0, 10, 10)
        self.sol10 = libst.Solution(grid=self.grid10, nvar=1)

    def test_celm_in(self):

        for it, ce in enumerate(libst.celm_in(self.grid10)):
            self.assertEqual(str(self.grid10.celm(it)), str(ce))
        for it, ce in enumerate(libst.celm_in(self.grid10, odd_plane=True)):
            self.assertEqual(str(self.grid10.celm(it, odd_plane=True)), str(ce))

    def test_selm_in(self):

        for it, se in enumerate(libst.selm_in(self.sol10)):
            self.assertEqual(str(self.sol10.selm(it)), str(se))
        for it, se in enumerate(libst.selm_in(self.sol10, odd_plane=True)):
            self.assertEqual(str(self.sol10.selm(it, odd_plane=True)), str(se))

# vim: set et sw=4 ts=4:
