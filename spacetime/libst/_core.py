# Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
# BSD 3-Clause License, see COPYING


"""
Core entities for :py:mod:`libst` library.
"""


from ._libst import (
    Grid,
    Solution,
    Celm,
    Selm,
    InviscidBurgersSolution,
    InviscidBurgersFelm,
    LinearScalarSolution,
    LinearScalarSelm,
)


__all__ = [
    'Grid',
    'Solution',
    'Celm',
    'Selm',
    'celm_in',
    'selm_in',
    'felm_in',
    'InviscidBurgersSolution',
    'InviscidBurgersFelm',
    'LinearScalarSolution',
    'LinearScalarSelm',
]


def celm_in(sol, odd_plane=False):
    grid = sol.grid
    ncelm = grid.ncelm-1 if odd_plane else grid.ncelm
    for it in range(ncelm):
        yield sol.celm(it, odd_plane=odd_plane)

def selm_in(sol, odd_plane=False):
    grid = sol.grid
    nselm = grid.nselm-1 if odd_plane else grid.nselm
    for it in range(nselm):
        yield sol.selm(it, odd_plane=odd_plane)

def felm_in(sol, odd_plane=False):
    grid = sol.grid
    nselm = grid.nselm-1 if odd_plane else grid.nselm
    for it in range(nselm):
        yield sol.felm(it, odd_plane=odd_plane)

# vim: set et sw=4 ts=4:
