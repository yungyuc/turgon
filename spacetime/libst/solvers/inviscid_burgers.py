# Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
# BSD 3-Clause License, see COPYING


"""
Core entities for :py:mod:`libst` library.
"""


import numpy as np

from libst import _core


class Solver:

    def __init__(self, grid, time_increment):

        self.grid = grid
        self.sol = _core.Solution(
            grid=grid, nvar=1, time_increment=time_increment)

    def init(self):

        self.sol.so0.fill(1)
        self.sol.so1.fill(1)
        for elm in _core.selm_in(self.sol, odd_plane=False):
            r = elm.grid.xmin + elm.xctr / (elm.grid.xmax-elm.grid.xmin)
            v = np.sin(r * np.pi * 2)
            dv = np.cos(r * np.pi * 2) * 2 * np.pi
            elm.set_so0(0, v)
            elm.set_so1(0, dv)

# vim: set et sw=4 ts=4:
