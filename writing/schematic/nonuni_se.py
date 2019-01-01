import sys
import os

import numpy as np

import libst


def draw():

    xloc = np.array([-1, 0, 2, 3.5])
    grid = libst.Grid(xloc=xloc)
    sol = libst.Solution(grid=grid, nvar=1, time_increment=6/4)

    dx = (grid.xmax-grid.xmin)/grid.ncelm
    dt = dx
    hdt = dt / 2

    c = libst.PstCanvas(unit='2cm', padding=0.5)
    c.set(linewidth='1pt')
    c.set(linecolor='black')

    # CE grids.
    linewidth = '0.5pt'
    for se in libst.selm_in(sol):
        c.line((se.x, 0), (se.x, dt), linewidth=linewidth)
    c.line((sol.selm(0).x, 0), (sol.selm(grid.ncelm).x, 0),
           linewidth=linewidth)
    c.line((sol.selm(0).x, hdt), (sol.selm(grid.ncelm).x, hdt),
           linewidth=linewidth)
    c.line((sol.selm(0).x, dt), (sol.selm(grid.ncelm).x, dt),
           linewidth=linewidth)

    # x-axis.
    sep = 0.05
    c.line((sol.selm(0).x, -hdt*1.2), (sol.selm(grid.ncelm).x, -hdt*1.2),
           linewidth=linewidth)
    c.uput(sep, 'l', (grid.xmin-sep,-hdt*1.2), r'$j$')
    c.uput(sep, 'r', (grid.xmax+sep,-hdt*1.2), r'$x$')
    for se in libst.selm_in(sol):
        c.line((se.x, -hdt*1.2-sep), (se.x, -hdt*1.2+sep))
        c.uput(sep, 'd', (se.x,-hdt*1.2-sep), r'$%d$'%se.index)
    for se in libst.selm_in(sol, odd_plane=True):
        c.line((se.x, -hdt*1.2-sep/2), (se.x, -hdt*1.2+sep/2))
        c.uput(sep, 'd', (se.x,-hdt*1.2-sep), r'$\frac{%d}{2}$'%(se.index*2+1))

    # t-axis.
    x = grid.xmin - dx*0.6
    c.line((x, 0), (x, dt), linewidth=linewidth)
    c.uput(sep, 'd', (x, 0-sep), r'$n$')
    c.uput(sep, 'u', (x, dt+sep), r'$t$')
    c.line((x-sep, 0), (x+sep, 0))
    c.uput(sep, 'l', (x-sep, 0), r'$0$')
    c.line((x-sep/2, hdt), (x+sep/2, hdt))
    c.uput(sep, 'l', (x-sep, hdt), r'$\frac{1}{2}$')
    c.line((x-sep, dt), (x+sep, dt))
    c.uput(sep, 'l', (x-sep, dt), r'$1$')

    # SE and solution propagation.
    sep = 0.05
    sepx = dx*0.1
    sept = dt*0.1
    for se in libst.selm_in(sol):
        c.selm(se, 0,
               sep=sep, linestyle='dotted', dotsep='1pt', linecolor='red')
    for se in libst.selm_in(sol, odd_plane=True):
        c.selm(se, hdt,
               sep=sep, linestyle='dotted', dotsep='1pt', linecolor='blue')

    return c

def draw_cce():

    c = libst.PstCanvas(unit='2cm', extent=(-2.4, -0.5, 2.4, 1.5))
    c.set(linewidth='1pt')
    c.set(linecolor='black')
    c.frame((-1,0), (1,1))
    c.line((0,0), (0,1), linestyle='dashed')
    c.frame((-0.95,0.05), (-0.05,0.95), linestyle='dotted', linecolor='red')
    c.rput('tr', (-0.1,0.9), r'$\mathrm{CE}_-$')
    c.frame((0.05,0.05), (0.95,0.95), linestyle='dotted', linecolor='red')
    c.rput('tr', (0.9,0.9), r'$\mathrm{CE}_+$')
    c.frame((-1.05,-0.05), (1.05,1.05), linestyle='dotted', linecolor='red')
    c.rput('tr', (-1.1,0.9), r'$\mathrm{CE}$')
    c.dots((0,1), (-1,1), (-1,0), (0,0), (1,0), (1,1), (0,1), dotstyle='*')
    c.uput(0.1, 'u', (0,1), r'A $(x_j,t^n)$')
    c.uput(0.1, 'ul', (-1,1), r'B')
    c.uput(0.1, 'dl', (-1,0), r'$(x_{j-\frac{1}{2}},t^{n-\frac{1}{2}})$ C')
    c.uput(0.1, 'd', (0,0), r'D')
    c.uput(0.1, 'dr', (1,0), r'E $(x_{j+\frac{1}{2}},t^{n-\frac{1}{2}})$')
    c.uput(0.1, 'ur', (1,1), r'F')
    c.line((0,0), (0,1), linestyle='dashed')

    return c

def main():

    filename = os.path.splitext(os.path.basename(sys.argv[0]))[0] + '.eps'
    if len(sys.argv) > 1: filename = sys.argv[1]

    drawn = draw()
    drawn.to_filename(filename)

    if len(sys.argv) > 2:
        filename = sys.argv[2]
        with open(filename, 'w') as fobj:
            fobj.write(str(drawn))
        print("Wrote tex file to", filename)

if __name__ == '__main__':
    main()

# vim: set et sw=4 ts=4:
