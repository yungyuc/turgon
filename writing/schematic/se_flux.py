import sys
import os

import numpy as np

import libst


def draw():

    xloc = np.array([-1, 0, 2, 3.5])
    grid = libst.Grid(xloc=xloc)
    sol = libst.Solver(grid=grid, nvar=1, time_increment=1)

    dt = sol.dt
    hdt = dt / 2
    qdt = hdt / 2
    odt = qdt / 2

    c = libst.PstCanvas(unit='5cm', padding=[0.75, 0.25, 0.7, 0.25])
    c.set(linewidth='1pt')
    c.set(linecolor='black')

    ce = sol.celm(ielm=1, odd_plane=False)
    linewidth = '0.5pt'

    # SEs.
    sexn = ce.selm_xn
    c.line((sexn.x,0), (sexn.x,hdt), linewidth=linewidth, linecolor='red')
    c.line((sexn.x,0), (sexn.xpos,0), linewidth=linewidth, linecolor='red')

    c.dots((sexn.x,0), dotstyle='o')
    c.uput(0.05, 'dl', (sexn.x,0), r'$x_{j-\frac{1}{2}}$')
    c.dots((sexn.xctr,0), dotstyle='triangle')
    c.uput(0.05, 'u', (sexn.xctr,0), r'$x^s_{j-\frac{1}{2}}$')

    sexp = ce.selm_xp
    c.line((sexp.x,0), (sexp.x,hdt), linewidth=linewidth, linecolor='blue')
    c.line((sexp.xneg,0), (sexp.x,0), linewidth=linewidth, linecolor='blue')

    c.dots((sexp.x,0), dotstyle='o')
    c.uput(0.05, 'dr', (sexp.x,0), r'$x_{j+\frac{1}{2}}$')
    c.dots((sexp.xctr,0), dotstyle='triangle')
    c.uput(0.05, 'u', (sexp.xctr,0), r'$x^s_{j+\frac{1}{2}}$')

    c.dots((sexn.xpos,0), dotstyle='square')
    assert sexn.xpos == sexp.xneg
    c.uput(0.05, 'u', (sexn.xpos,0),
           r'$x^+_{j-\frac{1}{2}} = x^-_{j+\frac{1}{2}}$')

    setp = ce.selm_tp
    c.line((setp.xneg,hdt), (setp.x,hdt),
           linewidth=linewidth, linecolor='orange')
    c.line((setp.x,hdt), (setp.xpos,hdt),
           linewidth=linewidth, linecolor='orange')

    c.dots((setp.x,hdt), dotstyle='o')
    c.dots((setp.xctr,setp.hdt), dotstyle='triangle')
    c.uput(0.05, 'd', (setp.x,hdt), r'$x_j=x^s_j$')

    # Fluxes.
    vlen = hdt/6
    sep = 0.015
    # \Delta x^+_{j-\frac{1}{2}}
    c.line(((sexn.x+sexn.xpos)/2, 0), ((sexn.x+sexn.xpos)/2, -vlen),
           arrows='->', linecolor='red')
    c.uput(0, 'd', ((sexn.x+sexn.xpos)/2, -vlen),
           r'{\color{red}$(\mathbf{h}^*)^n_{j-\frac{1}{2},+}'
           r'\cdot(0, -\Delta x^+_{j-\frac{1}{2}})$}')
    # \Delta x^-_{j+\frac{1}{2}}
    c.line(((sexp.xneg+sexp.x)/2, 0), ((sexp.xneg+sexp.x)/2, -vlen),
           arrows='->', linecolor='blue')
    c.uput(0, 'd', ((sexp.xneg+sexp.x)/2, -vlen),
           r'{\color{blue}$(\mathbf{h}^*)^n_{j+\frac{1}{2},-}'
           r'\cdot(0, -\Delta x^-_{j+\frac{1}{2}})$}')
    # \Delta x_j
    c.line((setp.xctr, hdt+sep), (setp.xctr, hdt+2*vlen),
           arrows='->', linecolor='orange')
    c.uput(0, 'u', (setp.xctr, hdt+2*vlen),
           r'{\color{orange}$\mathbf{h}^{n+\frac{1}{2}}_{j}'
           r'\cdot(0, \Delta x_j)$}')
    # \Delta t^n_{j-\frac{1}{2}}
    c.line((sexn.x, qdt), (sexn.x-vlen, qdt), arrows='->', linecolor='red')
    c.uput(0, 'ul', (sexn.x-vlen, qdt),
           r'{\color{red}$(\mathbf{h}^*)^{n,+}_{j-\frac{1}{2}}'
           r'\cdot(-\frac{\Delta t}{2}, 0)$}')
    # \Delta t^n_{j+\frac{1}{2}}
    c.line((sexp.x, qdt), (sexp.x+vlen, qdt), arrows='->', linecolor='blue')
    c.uput(0, 'ur', (sexp.x+vlen, qdt),
           r'{\color{blue}$(\mathbf{h}^*)^{n,+}_{j+\frac{1}{2}}'
           r'\cdot(\frac{\Delta t}{2}, 0)$}')

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
