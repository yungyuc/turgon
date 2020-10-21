''' 
Compare linear solver's solution wtih exact solution, because solutions of computation field are not a scalar,
therefore compared the norm of difference bewteen solver's solution and exact solution.

By comparing norm of difference bewteen solver's solution and exact solution we can check if the linear solver is work properly
or check if the 
'''

import numpy as np
import unittest
import libst


def exact_solution(svr, iter_num, gfun):
    ''' Calculate exact solution of the 1D advection equation
        Args:
            svr:
              linear wave solver
            iter_num: 
              how many step will be executed
            gfun:
              system function
        Return:
            A ndarray which content current time step solution
    '''
    v = np.array([])
    for e in svr.selms(odd_plane = False):
        x = (e.xctr - iter_num * svr.dt) % svr.grid.xmax  # dealt with boundary treatment
        if x < 2 * np.pi or x > 4 * np.pi:
            v = np.append(v, 0)
        else:
            v = np.append(v, gfun(x))
    return v


class NormTC(unittest.TestCase):
    ''' Create linear solver object for norm test
        Args:
            resolution:
              grid number
        Return:
            A well set linear solver object
    '''
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
    
    
    @staticmethod
    def norm(vec, ord=1):
        ''' Calculate the vector norm
            Args:
                vec:
                  vector for norm calculation
                ord:
                  norm order
            Return:
                A double number, vector norm
        '''
        res = 0 
        for ele in vec:
            res += abs(ele) ** ord
        return res ** (1 / ord)


    def setUp(self):

        self.resolution = 256
        self.svr = NormTC._build_solver(self.resolution)
        self.cycle = range(1001)


    def test_norm_error(self):
        for i in self.cycle:
            self.svr.march_alpha2(1)
            error_norm = NormTC.norm(exact_solution(self.svr, i + 1, np.sin) - self.svr.get_so0(0).ndarray, 1)
            self.assertLessEqual(error_norm, 1e-11) # FIX ME: I don't know how to determine assert condition

