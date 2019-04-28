subroutine solve_w1(&
   dnstep, dt, np, pa, dnx, x, neq, u, ux, nu, CFL_max)
implicit none
! arguments.
integer*4, intent(in)         :: dnstep  ! must be an odd number.
real   *8, intent(in)         :: dt
integer*4, intent(in)         :: np
real   *8, intent(in),       &
           dimension(np)      :: pa
integer*4, intent(in)         :: dnx
real   *8, intent(in),       &
           target,           &
           dimension(dnx)     :: x
integer*4, intent(in)         :: neq
real   *8, intent(inout),    &
           target,           &
           dimension(neq,dnx) :: u, ux
real   *8, intent(inout),    &
           target,           &
           dimension(dnx)     :: nu
real   *8, intent(out)        :: CFL_max

! local variables.
integer*4                     :: nx
real   *8, allocatable,      &
           dimension(:,:)     :: s, up
real   *8, pointer,          &
           dimension(:)       :: sidx, midx, sidnu, midnu
real   *8, pointer,          &
           dimension(:,:)     :: sidu, midu, sidux, midux
! intermediate variables.
real   *8                     :: dx, dx2, dx4, dt2, dtx, dttx4
real   *8, dimension(neq,neq) :: jaco
real   *8, dimension(neq)     :: f, fx, ft, ut
real   *8                     :: s_v, s_c, tau
! scheme variables.
real   *8, dimension(neq)     :: upl, upr, deno, vl, vr
real   *8, dimension(neq)     :: avl, avr, wl, wr
! iterators.
integer :: i, j, istep

allocate(s (neq,dnx/2+1))
allocate(up(neq,dnx/2+1))

! preset variables.
nu(:)  = 0.d0
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
    nx = size(midx)

    loop_s_up: do i = 1, nx+1
        ! f, jacobian.
        call eqn_euler(np, pa, neq, sidu(:,i), f, jaco, s_v, s_c)
        ! f_x, u_t, f_t.
        do j = 1, neq
            fx(j) = dot_product(jaco(j,:), sidux(:,i))
        end do
        ut(:) = -fx(:)
        do j = 1, neq
            ft(j) = dot_product(jaco(j,:), ut   (:)  )
        end do
        ! s & u'.
        s (:,i) = f(:)*dtx + sidux(:,i)*dx4 + ft(:)*dttx4
        up(:,i) = sidu(:,i) + ut(:)*dt2
    end do loop_s_up

   	! march u.
    midu(:,:) = (sidu(:,:nx) + sidu(:,2:nx) + s(:,:nx) - s(:,2:nx))/2

    ! calculate CFL/nu.
    loop_CFL: do i = 1, nx
        call eqn_euler(np, pa, neq, midu(:,i), f, jaco, s_v, s_c)
        if (s_v < 0) then
           midnu(i) = (s_v-s_c)*dtx
        else 
           midnu(i) = (s_v+s_c)*dtx
        end if
        tau = dabs(midnu(i))
        if (tau > CFL_max) CFL_max = tau
    end do loop_CFL

    ! march u_x.
    loop_weight: do i = 1, nx
        ! calculate value to be weighted.
        tau     = dabs(midnu(i))
        upl (:) = up(:,i  ) + (1-tau)*dx4*sidux(:,i  )
        upr (:) = up(:,i+1) - (1-tau)*dx4*sidux(:,i+1)
        vl  (:) = (midu(:,i) - upl (:  ))/((1+tau)*dx4)
        vr  (:) = (upr (:  ) - midu(:,i))/((1+tau)*dx4)
        ! calculate weighting function.
        avl (:) = dabs(vl(:))
        avr (:) = dabs(vr(:))
        deno(:) = avl(:) + avr(:) + 1.d-200
        wl  (:) = avr(:)/deno(:)
        wr  (:) = avl(:)/deno(:)
        ! weight to limit the result.
        midux(:,i) = wl(:)*vl(:) + wr(:)*vr(:)
    end do loop_weight
end do march

deallocate(up)
deallocate(s )

end subroutine solve_w1
! vim: set ai et nu ts=4 sw=4 tw=79:
