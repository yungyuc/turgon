! ** Define 1D Euler equation.
subroutine eqn_euler(np, pa, neq, u, f, jaco, v, c)
implicit none
! arguments.
integer*4, intent(in)         :: np
real   *8, intent(in),       &
           dimension(np)      :: pa
integer*4, intent(in)         :: neq
real   *8, intent(in),       &
           dimension(neq)     :: u
real   *8, intent(inout),    &
           dimension(neq)     :: f
real   *8, intent(inout),    &
           dimension(neq,neq) :: jaco
real   *8, intent(inout)      :: v, c
! local vars.
real   *8                     :: gam
 
! parse parameter values.
gam = pa(1)
! calculate f.
f(1) = u(2)
f(2) = (gam-1)*u(3) + (3.d0-gam)/2*u(2)**2/u(1)
f(3) = gam*u(2)*u(3)/u(1) - (gam-1)/2*u(2)**3/u(1)**2
! calculate jacobian using (3.14).
jaco(1,:) = (/0.d0, 1.d0, 0.d0/)
jaco(2,1) = (gam-3)/2*u(2)**2/u(1)**2
jaco(2,2) = -(gam-3)*u(2)/u(1)
jaco(2,3) = gam-1
jaco(3,1) = (gam-1)*u(2)**3/u(1)**3 - gam*u(2)*u(3)/u(1)**2
jaco(3,2) = gam*u(3)/u(1) - 3.d0/2*(gam-1)*u(2)**2/u(1)**2
jaco(3,3) = gam*u(2)/u(1)
! calculate speed (eigenvalues) using (3.15).
v = u(2)/u(1)
c = dsqrt(gam*(gam-1) * (u(3)/u(1)-(u(2)/u(1))**2/2))
end subroutine eqn_euler
! vim: set ai et nu ts=4 sw=4 tw=79:
