subroutine solve_ctau2(&
   dnstep, dt, gam, dnx, x, u, ux, nu, CFL_max)
implicit none
! arguments.
integer*8, intent(in) :: dnstep  ! must be an odd number.
real*8, intent(in) :: dt, gam
integer, intent(in) :: dnx
real*8, intent(in), target, dimension(dnx) :: x
real*8, intent(inout), target, dimension(3,dnx) :: u, ux
real*8, intent(inout), target, dimension(dnx) :: nu
real*8, intent(out) :: CFL_max

! local variables.
integer :: nx
real*8, allocatable, dimension(:,:) :: s, up
real*8, pointer, dimension(:) :: sidx, midx, sidnu, midnu
real*8, pointer, dimension(:,:) :: sidu, midu, sidux, midux
! intermediate variables.
real*8, dimension(3,3) :: J
real*8, dimension(3) :: f, ut, ft
integer, dimension(1) :: ashape
real*8 :: dx, dx2, dx4, dt2, dtx, dttx4
real*8 :: s_v, s_c, anu
! scheme variables.
real*8 :: fnu
real*8, dimension(3) :: upl, upr, deno, vl, vr
real*8, dimension(3) :: avl, avr, sl, sr, smin, wl, wr
! iterators.
integer :: i, istep

allocate(s (3,dnx/2+1))
allocate(up(3,dnx/2+1))

! preset variables.
nu(:)  = 0.d0
J(1,:) = (/0.d0, 1.d0, 0.d0/)
J(2,3) = gam-1
dx     = dabs(x(3) - x(1))
dx2    = dx/2
dx4    = dx/4
dt2    = dt/2
dtx    = dt/dx
dttx4  = dt**2/(4*dx)
CFL_max = 0.d0
march: do istep = 1, dnstep
   ! determine mesh and data bound.
   if (mod(istep,2) == 1) then
      sidx  => x (   :     :2)
      midx  => x (  2:dnx-1:2)
      sidu  => u (:, :     :2)
      midu  => u (:,2:dnx-1:2)
      sidux => ux(:, :     :2)
      midux => ux(:,2:dnx-1:2)
      sidnu => nu(   :     :2)
      midnu => nu(  2:dnx-1:2)
   else
      sidx  => x (  2:dnx-1:2)
      midx  => x (  3:dnx-2:2)
      sidu  => u (:,2:dnx-1:2)
      midu  => u (:,3:dnx-2:2)
      sidux => ux(:,2:dnx-1:2)
      midux => ux(:,3:dnx-2:2)
      sidnu => nu(  2:dnx-1:2)
      midnu => nu(  3:dnx-2:2)
   end if
   ! determine nx.
   ashape = shape(midx)
   nx = ashape(1)

   loop_s_up: do i = 1, nx+1
      ! Jacobian.
      J(2,1) = (gam-3)/2*sidu(2,i)**2/sidu(1,i)**2
      J(2,2) = -(gam-3)*sidu(2,i)/sidu(1,i)
      J(3,1) = (gam-1)*sidu(2,i)**3/sidu(1,i)**3 - &
               gam*sidu(2,i)*sidu(3,i)/sidu(1,i)**2
      J(3,2) = gam*sidu(3,i)/sidu(1,i) - &
               3.d0/2*(gam-1)*sidu(2,i)**2/sidu(1,i)**2
      J(3,3) = gam*sidu(2,i)/sidu(1,i)
      ! f.
      f(1) = sidu(2,i)
      f(2) = (gam-1)*sidu(3,i) + (3.d0-gam)/2*sidu(2,i)**2/sidu(1,i)
      f(3) = gam*sidu(2,i)*sidu(3,i)/sidu(1,i) - &
             (gam-1)/2*sidu(2,i)**3/sidu(1,i)**2
      ! u_t; intermediate vector for f_t.
      ut(1) = -J(1,1)*sidux(1,i) - J(1,2)*sidux(2,i) - J(1,3)*sidux(3,i)
      ut(2) = -J(2,1)*sidux(1,i) - J(2,2)*sidux(2,i) - J(2,3)*sidux(3,i)
      ut(3) = -J(3,1)*sidux(1,i) - J(3,2)*sidux(2,i) - J(3,3)*sidux(3,i)
      ! f_t.
      ft(1) = J(1,1)*ut(1) + J(1,2)*ut(2) + J(1,3)*ut(3)
      ft(2) = J(2,1)*ut(1) + J(2,2)*ut(2) + J(2,3)*ut(3)
      ft(3) = J(3,1)*ut(1) + J(3,2)*ut(2) + J(3,3)*ut(3)
      ! s & u'.
      s (:,i) = sidux(:,i)*dx4 + f(:)*dtx + ft(:)*dttx4
      up(:,i) = sidu(:,i) + ut(:)*dt2
   end do loop_s_up

   ! march u.
   midu(:,:)  = (sidu(:,:nx) + sidu(:,2:nx) + s(:,:nx) - s(:,2:nx))/2

   ! calculate CFL/nu.
   loop_CFL: do i = 1, nx
      s_v = midu(2,i)/midu(1,i)
      s_c = dsqrt(gam*(gam-1) * &
                  (midu(3,i)/midu(1,i)-(midu(2,i)/midu(1,i))**2/2))
      if (s_v < 0) then
         midnu(i) = (s_v-s_c)*dtx
      else 
         midnu(i) = (s_v+s_c)*dtx
      end if
      anu = dabs(midnu(i))
      if (anu > CFL_max) CFL_max = anu
   end do loop_CFL

   ! march u_x.
   loop_weight: do i = 1, nx
      ! calculate value to be weighted.
      anu     = dabs(midnu(i))
      upl (:) = up(:,i  ) + (1-anu)*dx4*sidux(:,i  )
      upr (:) = up(:,i+1) - (1-anu)*dx4*sidux(:,i+1)
      vl  (:) = (midu(:,i) - upl (:  ))/((1+anu)*dx4)
      vr  (:) = (upr (:  ) - midu(:,i))/((1+anu)*dx4)
      ! calculate weighting function.
      avl (:) = dabs(vl(:)*dx4)
      avr (:) = dabs(vr(:)*dx4)
      smin(:) = min(avl(:), avr(:)) + 1.d-200
      sl  (:) = (avl(:)+1.d-200)/smin(:) - 1
      sr  (:) = (avr(:)+1.d-200)/smin(:) - 1
      fnu     = 5.d-1/(anu+1.d-200)
      deno(:) = fnu*(sl(:)+sr(:)) + 2
      wl  (:) = (fnu*sr(:) + 1)/deno(:)
      wr  (:) = (fnu*sl(:) + 1)/deno(:)
      ! weight to limit the result.
      midux(:,i) = wl(:)*vl(:) + wr(:)*vr(:)
   end do loop_weight
end do march

deallocate(up)
deallocate(s )

end subroutine solve_ctau2
