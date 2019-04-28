subroutine solve_ds_nuni_w2(&
   dt, np, pa, dnx, x, xs, u, ux, un, unx, nu, CFL_max)
implicit none
! arguments.
real   *8, intent(in)       :: dt
integer*4, intent(in)       :: np
real   *8, intent(in),     &
           dimension(np)    :: pa
integer*4, intent(in)       :: dnx
real   *8, intent(in),     &
           target,         &
           dimension(dnx)   :: x, xs
real   *8, intent(inout),  &
           target,         &
           dimension(3,dnx) :: u, ux, un, unx
real   *8, intent(inout),  &
           target,         &
           dimension(dnx)   :: nu
real   *8, intent(out)      :: CFL_max

! local variables.
integer*4 :: nx
real   *8, allocatable,    &
           dimension(:,:)   :: s, up
real   *8, pointer,        &
           dimension(:)     :: sidx, midx, sidxs, midxs, sidnu, midnu
real   *8, pointer,        &
           dimension(:,:)   :: sidu , midu , sidux , midux , &
                               sidun, midun, sidunx, midunx
! intermediate variables.
real   *8, dimension(3,3)   :: jaco
real   *8, dimension(3)     :: f, ut, ft
real   *8                   :: dx, dxl, dxr
real   *8                   :: s_v, s_c, anu
! scheme variables.
real   *8                   :: dxsl, dxsr, dxpl, dxpr
real   *8, dimension(3)     :: usl, usr, upl, upr, deno, vl, vr
real   *8, dimension(3)     :: avl, avr, wl, wr
! iterators.
integer*4                   :: i, j, istep

allocate(s (3,dnx/2+1))
allocate(up(3,dnx/2+1))

! preset variables.
nu(:)      = 0.d0
CFL_max    = 0.d0
march: do istep = 1, 2
   ! determine mesh and data bound.
   if (mod(istep,2) == 1) then
      sidx   => x  (   :     :2)
      midx   => x  (  2:dnx-1:2)
      sidxs  => xs (   :     :2)
      midxs  => xs (  2:dnx-1:2)
      sidu   => u  (:, :     :2)
      midu   => u  (:,2:dnx-1:2)
      sidux  => ux (:, :     :2)
      midux  => ux (:,2:dnx-1:2)
      sidun  => un (:, :     :2)
      midun  => un (:,2:dnx-1:2)
      sidunx => unx(:, :     :2)
      midunx => unx(:,2:dnx-1:2)
      sidnu  => nu (   :     :2)
      midnu  => nu (  2:dnx-1:2)
   else
      sidx   => x  (  2:dnx-1:2)
      midx   => x  (  3:dnx-2:2)
      sidxs  => xs (  2:dnx-1:2)
      midxs  => xs (  3:dnx-2:2)
      sidu   => u  (:,2:dnx-1:2)
      midu   => u  (:,3:dnx-2:2)
      sidux  => ux (:,2:dnx-1:2)
      midux  => ux (:,3:dnx-2:2)
      sidun  => un (:,2:dnx-1:2)
      midun  => un (:,3:dnx-2:2)
      sidunx => unx(:,2:dnx-1:2)
      midunx => unx(:,3:dnx-2:2)
      sidnu  => nu (  2:dnx-1:2)
      midnu  => nu (  3:dnx-2:2)
   end if
   ! determine nx.
   nx = size(midx)

   ! calculate u_t, f_t as well as s, u' out of defined Jacobian.
   loop_s_up: do i = 1, nx+1
      ! f, jacobian.
      call eqn_euler(np, pa, size(sidu,dim=1), sidu(:,i), &
         f, jaco, s_v, s_c)
      ! u_t, f_t.
      do j = 1, size(jaco,dim=1)
         ut(j) = -dot_product(jaco(j,:), sidux(:,i))
      end do
      do j = 1, size(jaco,dim=1)
         ft(j) =  dot_product(jaco(j,:), ut   (:)  )
      end do
      ! s & u'.
      s (:,i) = f(:)*dt/2 + ft(:)*dt*dt/8
      up(:,i) = sidu(:,i) + ut(:)*dt/2
   end do loop_s_up

   ! march u.
   loop_u: do i = 1, nx
      dxl = midx(i  ) - sidx(i)
      dxr = sidx(i+1) - midx(i)
      dx  = dxl + dxr
      midun(:,i) = (dxl*(sidu(:,i  ) + sidux(:,i  )*dxl/2) + &
                    dxr*(sidu(:,i+1) - sidux(:,i+1)*dxr/2) + &
                    s(:,i) - s(:,i+1)) / dx
   end do loop_u

   ! calculate CFL/nu.
   loop_CFL: do i = 1, nx
      ! TODO: dx calculation is simplified and not consistent with scheme.
      dxl = midx(i  ) - sidx(i)
      dxr = sidx(i+1) - midx(i)
      dx  = min(dxl, dxr)*2
      call eqn_euler(np, pa, size(midun,dim=1), midun(:,i), &
         f, jaco, s_v, s_c)
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
      ! calculate value to be weighted.
      usl (:) = up(:,i  ) + sidux(:,i  )*dxsl
      usr (:) = up(:,i+1) + sidux(:,i+1)*dxsr
      upl (:) = usl(:) + sidux(:,i  )*(1-anu)*dxpl
      upr (:) = usr(:) - sidux(:,i+1)*(1-anu)*dxpr
      vl  (:) = (midun(:,i) - upl  (:  ))/(midxs(i)  -sidxs(i)-(1-anu)*dxpl)
      vr  (:) = (upr  (:  ) - midun(:,i))/(sidxs(i+1)-midxs(i)-(1-anu)*dxpr)
      ! calculate weighting function.
      avl (:) = dabs((midun(:,i) - usl  (:  ))/(dxl-dxsl))
      avr (:) = dabs((usr  (:  ) - midun(:,i))/(dxr+dxsr))
      deno(:) = avl(:) + avr(:) + 1.d-200
      wl  (:) = avr(:)/deno(:)
      wr  (:) = avl(:)/deno(:)
      ! weight to limit the result.
      midunx(:,i) = wl(:)*vl(:) + wr(:)*vr(:)
   end do loop_weight

   ! shift u back to mesh point.
   loop_shift_u: do i = 1, nx
      dx = midx(i) - (sidx(i)+sidx(i+1))/2
      midu(:,i) = midu(:,i) + midux(:,i)*dx
   end do loop_shift_u
end do march

deallocate(up)
deallocate(s )

end subroutine solve_ds_nuni_w2
! vim: set ai et nu ts=3 sw=3:
