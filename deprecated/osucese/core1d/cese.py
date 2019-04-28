class CESE(object):
    """
    1D CESE solver for shock tube problem.

    @ivar params: Array of parameters; the only one is the ratio of 
                  specific heat.
    @ivar x: Mesh points, staggered.
    @ivar xs: Center of solution elements, staggered.
    @ivar u: Mesh variables.
    @ivar ux: The derivative of mesh variables to space x.
    @ivar CFL: Maximum CFL number.
    """
    def __init__(self, params, x, u, ux):
        nx          = len(x)
        self.params = params.copy()
        self.x      = x     .copy()
        self.xs     = x     .copy()
        self.xs[2:-2:2] = (x[1:-2:2] + x[3::2])/2
        self.xs[1:-1:2] = (x[0:-1:2] + x[2::2])/2
        assert (self.xs[1::2] == self.x[1::2]).all()
        from numpy import zeros
        self.u      = zeros((3,nx), dtype=float, order='fortran')
        self.ux     = zeros((3,nx), dtype=float, order='fortran')
        self.u [:]  = u [:]
        self.ux[:]  = ux[:]
        self.CFL    = zeros(    nx, dtype=float)

    def march(self, dt):
        return self.solve(2, dt, self.params[0], 
            self.x, self.u, self.ux, self.CFL)

class HalfEpsilon(CESE):
    from core1d._core import solve_halfe as solve

class Ctau2(CESE):
    from core1d._core import solve_ctau2 as solve

class CESE2(CESE):
    def march(self, dt):
        return self.solve(2, dt, self.params, 
            self.x, self.u, self.ux, self.CFL)

class W1(CESE2):
    from core1d._core import solve_w1 as solve

class W2(CESE2):
    from core1d._core import solve_w2 as solve

class CESEnUni(CESE):
    def march(self, dt):
        return self.solve(2, dt, self.params[0], 
            self.x, self.xs, self.u, self.ux, self.CFL)

class NuniS2(CESEnUni):
    from core1d._core import solve_nuni_s2 as solve

class CESEnUni2(CESEnUni):
    def march(self, dt):
        return self.solve(2, dt, self.params, 
            self.x, self.xs, self.u, self.ux, self.CFL)

class NuniW1(CESEnUni2):
    from core1d._core import solve_nuni_w1 as solve

class NuniW2(CESEnUni2):
    from core1d._core import solve_nuni_w2 as solve

class NuniW3(CESEnUni2):
    from core1d._core import solve_nuni_w3 as solve

class NuniW4(CESEnUni2):
    from core1d._core import solve_nuni_w4 as solve

class CESEDsnUni2(CESEnUni2):
    def __init__(self, nx, dt, xratio):
        super(CESEnUni2, self).__init__(nx, dt, xratio)
        self.un  = zeros((3,nx), dtype=float, order='Fortran')
        self.unx = zeros((3,nx), dtype=float, order='Fortran')

    from core1d._core import solve_ds_nuni_w2 as solve
    def march(self):
        for i in range(2):
            res = self.solve(self.dt, self.params, self.x, self.xs, 
                self.u, self.ux, self.un, self.unx, self.CFL)
        return res
