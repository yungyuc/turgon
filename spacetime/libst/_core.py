# Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
# BSD 3-Clause License, see COPYING


"""
Core entities for :py:mod:`libst` library.
"""


from ._libst import (
    Grid,
    Celm,
    Selm,
    Solution,
)


__all__ = [
    'Grid',
    'Celm',
    'Selm',
    'Solution',
    'celm_in',
    'selm_in',
]


def celm_in(grid, odd_plane=False):
    ncelm = grid.ncelm-1 if odd_plane else grid.ncelm
    for it in range(ncelm):
        yield grid.celm(it, odd_plane=odd_plane)

def selm_in(sol, odd_plane=False):
    grid = sol.grid
    nselm = grid.nselm-1 if odd_plane else grid.nselm
    for it in range(nselm):
        yield sol.selm(it, odd_plane=odd_plane)

# vim: set et sw=4 ts=4:
