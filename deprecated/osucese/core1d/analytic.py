import sys
import warnings

from pylab import (plot, subplot, figure, savefig, show, 
        axis, grid, title, xticks, yticks, xlabel, ylabel)

class AnaShockTube(object):
    """
    Analytical solution for Sod's shock tube problem.

    @ivar gamma: Ratio of specific heat.
    @ivar nx: Number of mesh points for the solution in expansion wave.
    @ivar vn: numpy array of shape (4,4), corresponding to the value of 
        (position, density, pressure, speed) of region 1 to 4. About the 
        position, specifically, 
          - vn[0,0]: right margin of solution.
          - vn[1,0]: the shock wave between region 1 and 2.
          - vn[2,0]: contact discontinuity between region 2 and 3.
          - vn[3,0]: left margin of solution.
    @ivar ve: numpy array of shape (4,nx), corresponding to the value of 
        (position, density, pressure, speed) in the expansion wave between 
        region 3 and 4.
    """
    def __init__(self, gamma, v1, v4,
            xshift=0.0, nx=101):
        """
        @param gamma: Ratio of specific heat.
        @param x1: Left boundary position for region 1.
        @param r1: Density for region 1.
        @param p1: Pressure for region 1.
        @param u1: Speed for region 1.
        @param x4: Right boundary position for region 4.
        @param r4: Density for region 4.
        @param p4: Pressure for region 4.
        @param u4: Speed for region 4.

        @keyword xshift: Shift value of x coordinate.
        @keyword nx: Number of point in x coordinate.
        """
        from numpy import zeros
        self.gamma = gamma
        self.vn = zeros((4,4), dtype=float)  # xn, rn, pn, un
        self.ve = zeros((4,nx), dtype=float) # xe, re, pe, ue
        x1, r1, p1, u1 = v1
        x4, r4, p4, u4 = v4
        self.vn[0] = x1, r1, p1, u1
        self.vn[3] = x4, r4, p4, u4
        self.xshift = xshift
        self.nx = nx

    @staticmethod
    def strength(p41, gamma, a14):
        """
        Calculate the strength of the shock wave.

        @param p41: Ratio of pressure of region 4 over region 1.
        @param gamma: Ratio of specific heat.
        @param a14: Ratio of speed of sound of region 1 over region 4.
        """
        from numpy import sqrt
        def func(x):
            d1 = x-1
            d2 = gamma-1
            d3 = 2.0*gamma
            d4 = gamma+1
            nume = d2*a14*d1
            deno = sqrt(d3*(d3+d4*d1))
            return x*(1-nume/deno)**(-d3/d2)-p41
        from scipy.optimize import newton
        return newton(func, 1.0)

    @staticmethod
    def expwave(r4, p4, a4, u3, a3, gamma, time, nx):
        """
        Calculate the solution in the expansion wave.

        @param r4: Density for region 4.
        @param p4: Pressure for region 4.
        @param a4: Speed of sound for region 4.
        @param u3: Speed of right edge of expansion wave; 
        @param a3: Speed of sound for region 3.
        @param gamma: Ratio of specific heat.
        @param time: Specified time.
        @param nx: Number of mesh points in x direction.
        """
        xel = -a4 * time
        xer = (u3-a3)*time

        from numpy import arange
        xco = arange(nx, dtype=float)*(xer-xel)/(nx-1) + xel
        u = 2/(gamma+1) * (a4 + xco/time)
        r = (1 - (gamma-1)/2*(u/a4))**(2/(gamma-1)) * p4
        p = (1 - (gamma-1)/2*(u/a4))**(2*gamma/(gamma-1)) * p4
        return xco, r, p, u

    def calculate(self, time):
        """
        Calculate for solution at specified time.

        @param time: Specified time for solution.
        """
        from numpy import sqrt

        gamma = self.gamma
        xr, r1, p1, u1 = self.vn[0]
        xl, r4, p4, u4 = self.vn[3]

        a4 = sqrt(gamma*p4/r4)
        a1 = sqrt(gamma*p1/r1)

        # calculate region 2.
        p41 = p4/p1
        p21 = self.strength(p4/p1, gamma, a1/a4)
        r21 = (1+(gamma+1)/(gamma-1)*p21) / ((gamma+1)/(gamma-1)+p21)
        r2 = r21*r1
        u2 = a1/gamma * (p21-1) * sqrt( 
                2*gamma/(gamma+1) / (p21 + (gamma-1)/(gamma+1)) )
        p2 = p21*p1

        # calculate shock speed.
        us = a1 * sqrt( (gamma+1)/(2*gamma)*(p21-1) + 1 )
        self.vn[1] = us*time, r2, p2, u2

        # caluculate region 3.
        p34 = p21/p41
        r34 = p34**(1/gamma)
        r3 = r34*r4
        p3 = p34*p4
        u3 = u2
        a3 = sqrt(gamma*p3/r3)
        self.vn[2] = u2*time, r3, p3, u3

        # calculate expansion wave.
        self.ve[:] = self.expwave(
                r4, p4, a4, u3, a3, gamma, time, self.nx)

        self.ve[0]     += self.xshift
        self.vn[:,1:3] += self.xshift
