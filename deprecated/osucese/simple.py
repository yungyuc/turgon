"""
This is the simulation for a simple shock tube, with various scheme of 
CESE method.

The entry point of this script is main().

Read simulate() function for detail, in which the main logic of simulation 
is implemented.
"""
import sys
log = sys.stdout.write

import matplotlib
matplotlib.use('TkAgg')

def draw(x, val, anap, idx, 
        color, theylabel, thetitle="", thexlabel=""):
    """
    Draw simulated data and analytical solution on the same figure.
    """
    from numpy import arange
    from pylab import (plot, 
            axis, grid, title, xticks, yticks, xlabel, ylabel)

    v1, v2, v3, v4 = anap.vn
    ve = anap.ve
    # analytical solution.
    plot([v4[0]   , ve[0,0]], [v4[idx], v4[idx]], color=color)
    plot( ve[0]             ,  ve[idx]          , color=color)
    plot([ve[0,-1], v3[0]  ], [v3[idx], v3[idx]], color=color)
    plot([v3[0]   , v3[0]  ], [v3[idx], v2[idx]], color=color)
    plot([v3[0]   , v2[0]  ], [v2[idx], v2[idx]], color=color)
    plot([v2[0]   , v2[0]  ], [v2[idx], v1[idx]], color=color)
    plot([v2[0]   , v1[0]  ], [v1[idx], v1[idx]], color=color)
    # ce/se solution.
    plot(x[::2]             , val[::2]          , '.', color=color)
    title(thetitle)
    xlabel(thexlabel)
    ylabel(theylabel)
    axis([-0.5, 0.5, -0.1, 1.1])
    xticks([-0.5, -0.25, 0.0, 0.25, 0.5])
    yticks(arange(11,dtype=float)/10.)
    grid(True)

def initialize(solver, gamma, r1, v1, p1, r2, v2, p2, nx, xratio):
    """
    Initiailize CESE solver with simple shock tube problem.

    @param solver: CESE solver class.
    @type solver: core1d.cese.CESE class
    @param gamma: Ratio of specific heat.
    @type gamma: float
    @param r1, v1, p1: Density, velocity and pressure of region 1.
    @type r1, v1, p1: float
    @param r2, v2, p2: Density, velocity and pressure of region 2.
    @type r2, v2, p2: float
    @param nx: Number of staggered x mesh.
    @type nx: int
    @param xratio: ratio of adjacent x mesh.
    @type xratio: float

    @return: Solver instance.
    @rtype: core1d.cese.CESE instance
    """
    from numpy import zeros, array, arange
    from core1d.cese import CESEnUni

    if not issubclass(solver, CESEnUni):
        xratio = 1.0
    assert nx%2 != 0

    # data arrays.
    x  = zeros(    nx, dtype=float)
    u  = zeros((3,nx), dtype=float, order='Fortran')
    ux = zeros((3,nx), dtype=float, order='Fortran')

    # initialize.
    dx2 = 1.0/(nx-1)
    dx  = dx2*2
    nx1 = (nx-1)//2 + 1
    nx2 = (nx-1)//2
    x[ ::2] = arange(nx1, dtype=float)*dx - 0.5
    x[1::2] = arange(nx2, dtype=float)*dx + dx2 - 0.5
    L = (x[-1]-x[0])/2
    n = nx//2//2
    if xratio != 1.0:
        dx = (xratio-1)/(xratio**n-1)*L
    else:
        dx = L/n
    for i in range(n):
        x[nx//2+2*(i+1)] =  x[nx//2+2*i] + xratio**i*dx
        x[nx//2+2*i+1  ] = (x[nx//2+2*i] + x[nx//2+2*(i+1)]) / 2
        x[nx//2-2*(i+1)] = -x[nx//2+2*(i+1)]
        x[nx//2-2*i-1  ] = -x[nx//2+2*i+1  ]
    u[0,:nx1] = r1
    u[0,nx1:] = r2
    u[1,:nx1] = r1*v1
    u[1,nx1:] = r2*v2
    u[2,:nx1] = p1/(gamma-1) + r1*v1**2/2
    u[2,nx1:] = p2/(gamma-1) + r2*v2**2/2

    return solver(array([gamma], dtype=float), x, u, ux)

def simulate(nx, xratio, dt, nstep, solver, casename=None):
    """
    Main logic for the simulation.

    @param nx: Number of total staggerred spatial mesh.
    @type nx: int
    @param xratio: Ratio of adjascent spatial mesh point.
    @type xratio: float
    @param dt: Full time step.
    @type dt: float
    @param nstep: Simulation length in number of time steps.
    @type nstep: int
    @param solver: Solver class.
    @type solver: core1d.cese.CESE
    @keyword casename: Specified casename.
    @tyep casename: str
    """
    ################################################################
    # Setup runtime environment.
    ################################################################
    import os
    from numpy import isnan, concatenate
    from pylab import figure, subplot, savefig
    from core1d.analytic import AnaShockTube
    from core1d.utilities import format_float

    casename = casename or os.path.splitext(os.path.split(__file__)[-1])[0]
    outputdir = "result_"+casename
    basefn = "%s_%s_%s_%s" % (casename, solver.__name__, 
        format_float(dt), xratio)
    log('\n'+"#"*64+'\n')
    log("# All output goes to \"%s\" directory.\n" % outputdir)
    log("# Base filename is \"%s\".\n" % basefn)
    log("#"*64+'\n')

    ################################################################
    # State conditions.
    ################################################################
    gamma = 1.4
    r1    = 1.0
    v1    = 0.0
    p1    = 1.0
    r2    = 0.125
    v2    = 0.0
    p2    = 0.1

    ################################################################
    # Initialize.
    ################################################################
    cese = initialize(solver, gamma, r1, v1, p1, r2, v2, p2, nx, xratio)
    log("Total steps          = %d.\n" % nstep)
    log("Final time           = %g seconds.\n" % (nstep*dt))
    log("Spatial mesh ratio   = %f .\n" % xratio)
    log("Maximum mesh ratio   = %f .\n" % ((cese.x[-1]-cese.x[-2]) / \
        (cese.x[len(cese.x)//2+1]-cese.x[len(cese.x)//2])))

    ################################################################
    # Time marching.
    ################################################################
    istep = 0
    aCFL  = 0.0
    pstep = 5000//(nx//200 or 1)
    while istep < nstep:
        if istep > 0 and istep%(pstep*60) == 0:
            log(" %d\n"%istep)
        if istep%pstep == 0:
            log("#")
        if isnan(cese.u).any() or isnan(cese.ux).any():
            log("\nnan occurs at step: %d\n" % istep)
            for i in range(cese.u.shape[1]):
                if isnan(cese.u[:,i]).any():
                    log("on u at %d\n" % i)
                if isnan(cese.ux[:,i]).any():
                    log("on ux at %d\n" % i)
            break
        CFL = cese.march(dt)
        aCFL += CFL
        if CFL >= 1.0:
            log("\nCFL >= 1 at step: %d\n"%istep)
            break
        sys.stdout.flush()
        istep += 1
    aCFL /= (istep+1)
    log("\nAveraged maximum CFL = %f .\n" % aCFL)

    ################################################################
    # Analytic solution.
    ################################################################
    xshift = cese.x[len(cese.x)//2+1] - cese.x[len(cese.x)//2]
    anap = AnaShockTube(gamma, (0.5, r2, p2, v2), (-0.5, r1, p1, v1), 
        xshift=xshift)
    anap.calculate(istep*dt)

    ################################################################
    # Post-processing.
    ################################################################
    u  = cese.u
    ux = cese.ux
    x  = cese.x
    figure()
    subplot(221)
    draw(x, u[0], anap, idx=1, 
            color="b", theylabel="$\\rho$", 
            thetitle="solver: %s, r: %s"%(solver.__name__, xratio))
    subplot(222)
    draw(x, (gamma-1)*(u[2]-u[1]**2/(2*u[0])), anap, idx=2, 
            color="g", theylabel="$p$", thexlabel="$x$", 
            thetitle="$t=%4.2f, CFL_a=%6.4f$"%(istep*dt, aCFL))
    subplot(223)
    draw(x, u[1]/u[0], anap, idx=3, 
            color="r", theylabel="$u$", thexlabel="$x$")
    if not os.path.exists(outputdir):
        os.mkdir(outputdir)
    savefig(os.path.join(outputdir, "%s.eps"%basefn))
    savefig(os.path.join(outputdir, "%s.png"%basefn))
    concatenate([x,u[0],u[1],u[2],ux[0],ux[1],ux[2]]).tofile(
            os.path.join(outputdir, "%s.dat"%basefn), sep="\n")

def main():
    """
    Entry point of the simulation.
    """
    from numpy import arange
    from core1d import cese
    final_time = 0.2    # seconds.
    for xratio in (1.0,1.1,):
        for dt in 4*10.**arange(-6,-7,-1):
            nstep = int(round(final_time/dt))
            for solver in ['NuniW%d'%i for i in range(1,5)]:
                simulate(101, xratio, dt, nstep, getattr(cese, solver))

if __name__ == '__main__':
    from core1d.distutils import build
    if len(sys.argv) > 1 and sys.argv[1] in ('build', 'clean'):
        build()
    else:
        main()
