import numpy as np
import unittest
import libst

''' Calculate the vector norm
    vec: vector for norm calculation
    ord: norm order
'''
def norm(vec, ord = 1):
    res = 0 
    for ele in vec:
        res += abs(ele) ** ord
    return res ** (1 / ord)


''' Calculate exact solution of the 1D advection equation
    svr: linear wave solver
    iter_num: how many step will be executed
    gfun: system function
'''
def ExactSolution(svr, iter_num, gfun):
    v = np.array([])
    for e in svr.selms(odd_plane = False):
        x = (e.xctr - iter_num * svr.dt) % svr.grid.xmax  # dealt with boundary treatment
        if x < 2 * np.pi or x > 4 * np.pi:
            v = np.append(v, 0)
        else:
            v = np.append(v, gfun(x))
    return v

class NormTC(unittest.TestCase):

    @staticmethod
    def _build_solver(resolution):

        grid = libst.Grid(0, 4 * 2 * np.pi, resolution)
        cfl = 1
        dx = (grid.xmax - grid.xmin) / grid.ncelm
        dt = dx * cfl 
        svr = libst.LinearScalarSolver(grid = grid, time_increment = dt)
        
        # Initialize
        for e in svr.selms(odd_plane = False):
            if e.xctr < 2 * np.pi or e.xctr > 4 * np.pi:
                v = 0
                dv = 0
            else:
                v = np.sin(e.xctr)
                dv = np.cos(e.xctr)
            e.set_so0(0, v)
            e.set_so1(0, dv)
        
        svr.setup_march()

        return svr

    def setUp(self):

        self.resolution = 256
        self.svr = self._build_solver(self.resolution)
        self.cycle = range(1001)

    def test_norm_error(self):
        for i in self.cycle:
            self.svr.march_alpha2(1)
            error_norm = norm(ExactSolution(self.svr, i + 1, np.sin) - self.svr.get_so0(0).ndarray, 1)
            self.assertLessEqual(error_norm, 1e-11) # FIX ME: I don't know how to determine assert condition

