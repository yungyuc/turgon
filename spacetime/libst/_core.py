# Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
# BSD 3-Clause License, see COPYING


"""
Core entities for :py:mod:`libst` library.
"""


from ._libst import (
    Grid,
    Celm,
    Selm,
    Kernel,
    Solver,
    InviscidBurgersSolver,
    LinearScalarSolver,
)


__all__ = [
    'Grid',
    'Celm',
    'Selm',
    'Kernel',
    'Solver',
    'SolverProxy',
    'InviscidBurgersSolver',
    'LinearScalarSolver',
]


class SolverProxy():

    def __init__(self, *args, **kw):

        self.svr = Solver(*args, **kw)
        self.svr.kernel.xp_calc = self._xp_calc
        self.svr.kernel.xn_calc = self._xn_calc
        self.svr.kernel.tp_calc = self._tp_calc
        self.svr.kernel.tn_calc = self._tn_calc
        self.svr.kernel.so0p_calc = self._so0p_calc
        self.svr.kernel.cfl_updater = self._cfl_updater

    def _xp_calc(self, se, iv):
        return 1.0

    def _xn_calc(self, se, iv):
        return 1.0

    def _tp_calc(self, se, iv):
        return 1.0

    def _tn_calc(self, se, iv):
        return 1.0

    def _so0p_calc(self, se, iv):
        return se.get_so(iv)

    def _cfl_updater(self, se):
        se.set_cfl(1.0)

    def __getattr__(self, name):
        return getattr(self.svr, name)

# vim: set et sw=4 ts=4:
