subroutine solve_nuni_s2(&
   dnstep, dt, gam, dnx, x, xs, u, ux, nu, CFL_max)
implicit none
! arguments.
integer*4, intent(in)       :: dnstep  ! must be an odd number.
real   *8, intent(in)       :: dt, gam
integer*4, intent(in)       :: dnx
real   *8, intent(in),    &
           target,        &
           dimension(dnx)   :: x, xs
real   *8, intent(inout), &
           target,        &
           dimension(3,dnx) :: u, ux
real   *8, intent(inout), &
           target,        &
           dimension(dnx)   :: nu
real   *8, intent(out)      :: CFL_max

! local variables.
integer*4                   :: nx
real   *8, allocatable,   &
           dimension(:,:)   :: s, up
real   *8, pointer,       &
           dimension(:)     :: sidx, midx, sidxs, midxs, sidnu, midnu
real   *8, pointer,       &
           dimension(:,:)   :: sidu, midu, sidux, midux
! intermediate variables.
real   *8, dimension(3,3)   :: jaco
real   *8, dimension(3)     :: f, ut, ft
real   *8                   :: dx, dxl, dxr
real   *8                   :: s_v, s_c, anu
! scheme variables.
real   *8                   :: dxsl, dxsr, dxpl, dxpr, fnu
real   *8, dimension(3)     :: usl, usr, upl, upr, deno, vl, vr
real   *8, dimension(3)     :: avl, avr, sl, sr, smin, wl, wr
! iterators.
integer*4                   :: i, istep

allocate(s (3,dnx/2+1))
allocate(up(3,dnx/2+1))

! preset variables.
nu(:)  = 0.d0
jaco(1,:) = (/0.d0, 1.d0, 0.d0/)
jaco(2,3) = gam-1
CFL_max = 0.d0
march: do istep = 1, dnstep
   ! determine mesh and data bound.
   if (mod(istep,2) == 1) then
      sidx  => x (   :     :2)
      midx  => x (  2:dnx-1:2)
      sidxs => xs(   :     :2)
      midxs => xs(  2:dnx-1:2)
      sidu  => u (:, :     :2)
      midu  => u (:,2:dnx-1:2)
      sidux => ux(:, :     :2)
      midux => ux(:,2:dnx-1:2)
      sidnu => nu(   :     :2)
      midnu => nu(  2:dnx-1:2)
   else
      sidx  => x (  2:dnx-1:2)
      midx  => x (  3:dnx-2:2)
      sidxs => xs(  2:dnx-1:2)
      midxs => xs(  3:dnx-2:2)
      sidu  => u (:,2:dnx-1:2)
      midu  => u (:,3:dnx-2:2)
      sidux => ux(:,2:dnx-1:2)
      midux => ux(:,3:dnx-2:2)
      sidnu => nu(  2:dnx-1:2)
      midnu => nu(  3:dnx-2:2)
   end if
   ! determine nx.
   nx = size(midx)

   loop_s_up: do i = 1, nx+1
      ! jacobian.
      jaco(2,1) = (gam-3)/2*sidu(2,i)**2/sidu(1,i)**2
      jaco(2,2) = -(gam-3)*sidu(2,i)/sidu(1,i)
      jaco(3,1) = (gam-1)*sidu(2,i)**3/sidu(1,i)**3 - &
                  gam*sidu(2,i)*sidu(3,i)/sidu(1,i)**2
      jaco(3,2) = gam*sidu(3,i)/sidu(1,i) - &
                  3.d0/2*(gam-1)*sidu(2,i)**2/sidu(1,i)**2
      jaco(3,3) = gam*sidu(2,i)/sidu(1,i)
      ! f.
      f(1) = sidu(2,i)
      f(2) = (gam-1)*sidu(3,i) + (3.d0-gam)/2*sidu(2,i)**2/sidu(1,i)
      f(3) = gam*sidu(2,i)*sidu(3,i)/sidu(1,i) - &
             (gam-1)/2*sidu(2,i)**3/sidu(1,i)**2
      ! u_t; intermediate vector for f_t.
      ut(1) = -dot_product(jaco(1,:),sidux(:,i))
      ut(2) = -dot_product(jaco(2,:),sidux(:,i))
      ut(3) = -dot_product(jaco(3,:),sidux(:,i))
      ! f_t.
      ft(1) = dot_product(jaco(1,:),ut(:))
      ft(2) = dot_product(jaco(2,:),ut(:))
      ft(3) = dot_product(jaco(3,:),ut(:))
      ! s_m in (6.2).
      s (:,i) = f(:)*dt/2 + ft(:)*dt*dt/8
      ! u' in (6.5).
      up(:,i) = sidu(:,i) + ut(:)*dt/2
   end do loop_s_up

   ! advance u using (6.1) (non-uniform version of (3.9)).
   loop_u: do i = 1, nx
      dxl = midx(i  ) - sidx(i)
      dxr = sidx(i+1) - midx(i)
      dx  = dxl + dxr
      midu(:,i) = (dxl*(sidu(:,i  ) + sidux(:,i  )*dxl/2) + &
                   dxr*(sidu(:,i+1) - sidux(:,i+1)*dxr/2) + &
                   s(:,i) - s(:,i+1)) / dx
   end do loop_u

   ! calculate CFL/nu using (6.9).
   loop_CFL: do i = 1, nx
      ! TODO: dx calculation is simplified and not consistent with scheme.
      dxl = midx(i  ) - sidx(i)
      dxr = sidx(i+1) - midx(i)
      dx  = min(dxl, dxr)*2
      s_v = midu(2,i)/midu(1,i)
      s_c = dsqrt(gam*(gam-1) * &
            (midu(3,i)/midu(1,i)-(midu(2,i)/midu(1,i))**2/2))
      if (s_v < 0) then
         midnu(i) = (s_v-s_c)*dt/dx
      else 
         midnu(i) = (s_v+s_c)*dt/dx
      end if
      anu = dabs(midnu(i))
      if (anu > CFL_max) CFL_max = anu
   end do loop_CFL

   ! march u_x.
   loop_weight: do i = 1, nx
      anu     = dabs(midnu(i))
      dxl     = midx (i  ) - sidx(i  )
      dxr     = sidx (i+1) - midx(i  )
      dxsl    = sidxs(i  ) - sidx(i  )
      dxsr    = sidxs(i+1) - sidx(i+1)
      dxpl    = dxl/2 - dxsl
      dxpr    = dxr/2 + dxsr
      ! calculate value to be weighted (u_{x-}, u_{x+} in (6.6), (6.7)).
      usl (:) = up(:,i  ) + sidux(:,i  )*dxsl
      usr (:) = up(:,i+1) + sidux(:,i+1)*dxsr
      upl (:) = usl(:) + sidux(:,i  )*(1-anu)*dxpl
      upr (:) = usr(:) - sidux(:,i+1)*(1-anu)*dxpr
      vl  (:) = (midu(:,i) - upl (:  ))/(midxs(i)  -sidxs(i)-(1-anu)*dxpl)
      vr  (:) = (upr (:  ) - midu(:,i))/(sidxs(i+1)-midxs(i)-(1-anu)*dxpr)
      ! calculate weighting factor using Scheme-II (4.5).
      ! TODO not so sure about this scheme.
      avl (:) = dabs((midu(:,i) - usl (:  ))/2)
      avr (:) = dabs((usr (:  ) - midu(:,i))/2)
      smin(:) = min(avl(:), avr(:)) + 1.d-200
      sl  (:) = (avl(:)+1.d-200)/smin(:) - 1
      sr  (:) = (avr(:)+1.d-200)/smin(:) - 1
      fnu     = 5.d-1/anu
      deno(:) = fnu*(sl(:)+sr(:)) + 2
      wl  (:) = (fnu*sr(:) + 1)/deno(:)
      wr  (:) = (fnu*sl(:) + 1)/deno(:)
      ! weight to limit the result using (4.3).
      midux(:,i) = wl(:)*vl(:) + wr(:)*vr(:)
   end do loop_weight

   ! shift u back to mesh point.
   loop_shift_u: do i = 1, nx
      dx = midx(i) - (sidx(i)+sidx(i+1))/2
      midu(:,i) = midu(:,i) + midux(:,i)*dx
   end do loop_shift_u
end do march

deallocate(up)
deallocate(s )

end subroutine solve_nuni_s2
