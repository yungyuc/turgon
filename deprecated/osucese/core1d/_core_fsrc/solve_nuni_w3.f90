subroutine solve_nuni_w3(&
   dnstep, dt, np, pa, dnx, x, xs, u, ux, nu, CFL_max)
implicit none
! arguments.
integer*4, intent(in)       :: dnstep  ! must be an odd number.
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
           dimension(3,dnx) :: u, ux
real   *8, intent(inout),  &
           target,         &
           dimension(dnx)   :: nu
real   *8, intent(out)      :: CFL_max

! local variables.
integer*4                   :: nx
real   *8, allocatable,    &
           dimension(:,:)   :: s, up
real   *8, pointer,        &
           dimension(:)     :: sidx, midx, sidxs, midxs, sidnu, midnu
real   *8, pointer,        &
           dimension(:,:)   :: sidu, midu, sidux, midux
! intermediate variables.
real   *8, dimension(3,3)   :: jaco
real   *8, dimension(3)     :: f, fx, ft, ut
real   *8, dimension(3)     :: ul, ur
real   *8                   :: dx, dxl, dxr
real   *8                   :: s_v, s_c, tau
! scheme variables.
real   *8                   :: dxpl, dxpr, xpl, xpr
real   *8, dimension(3)     :: upl, upr, deno, vl, vr
real   *8, dimension(3)     :: avl, avr, wl, wr, dlt
! iterators.
integer*4                   :: i, j, istep

allocate(s (3,dnx/2+1))
allocate(up(3,dnx/2+1))

! shift u from mesh point to solution point.
shift_forward: do i = 1, dnx
    u(:,i) = u(:,i) + ux(:,i)*(xs(i)-x(i))
end do shift_forward

! preset variables.
nu(:)   = 0.d0
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

    ! calculate u_t, f_t as well as s, u' out of defined Jacobian.
    loop_s_up: do i = 1, nx+1
        ! f, jacobian.
        call eqn_euler(np, pa, size(sidu,dim=1), sidu(:,i), &
            f, jaco, s_v, s_c)
        ! f_x, u_t, f_t.
        do j = 1, size(jaco,dim=1)
            fx(j) = dot_product(jaco(j,:), sidux(:,i))
        end do
        ut(:) = -fx(:)
        do j = 1, size(jaco,dim=1)
            ft(j) = dot_product(jaco(j,:), ut   (:)  )
        end do
        ! s_m in (6.2).
        s (:,i) = f(:) + fx(:)*(sidx(i)-sidxs(i)) + ft(:)*dt/4
        ! u' in (6.5).
        up(:,i) = sidu(:,i) + ut(:)*dt/2
    end do loop_s_up

    ! advance u using (6.1) (non-uniform version of (3.9)).
    loop_u: do i = 1, nx
        dxl   = midx(i  ) - sidx(i)
        dxr   = sidx(i+1) - midx(i)
        dx    = dxl + dxr
        ul(:) = sidu(:,i  )+sidux(:,i  )*((midx(i)+sidx(i  ))/2-sidxs(i  ))
        ur(:) = sidu(:,i+1)+sidux(:,i+1)*((midx(i)+sidx(i+1))/2-sidxs(i+1))
        midu(:,i) = (ul(:)*dxl + ur(:)*dxr + (s(:,i) - s(:,i+1))*dt/2) / dx
    end do loop_u

    ! calculate CFL/nu using (6.9).
    loop_CFL: do i = 1, nx
        ! NOTE: change of meaning of dxl, dxr in this loop!!
        if (i == 1) then
            dxl = (midxs(i) - sidxs(i))*2 
        else
            dxl = midxs(i) - midxs(i-1)
        end if
        if (i == nx) then
            dxr = (sidxs(i+1) - midxs(i))*2
        else
            dxr = midxs(i+1) - midxs(i)
        end if
        call eqn_euler(np, pa, size(midu,dim=1), midu(:,i), &
            f, jaco, s_v, s_c)
        midnu(i) = sign((s_c+dabs(s_v)) * dmax1(dt/dxl,dt/dxr), s_v)
        tau      = dabs(midnu(i))
        if (tau > CFL_max) CFL_max = tau
    end do loop_CFL

    ! advance u_x.
    loop_weight: do i = 1, nx
        tau     = dabs(midnu(i))
        dxpl    = (1-tau) * ((midx(i)+sidx(i))/2 - sidxs(i))
        dxpr    = (1-tau) * (sidxs(i+1) - (midx(i)+sidx(i+1))/2)
        xpl     = sidxs(i  ) + dxpl
        xpr     = sidxs(i+1) - dxpr
        ! calculate value to be weighted (u_{x-}, u_{x+} in (6.6), (6.7)).
        upl (:) = up(:,i  ) + sidux(:,i  )*dxpl
        upr (:) = up(:,i+1) - sidux(:,i+1)*dxpr
        vl  (:) = (midu(:,i) - upl (:  ))/(midxs(i)-xpl)
        vr  (:) = (upr (:  ) - midu(:,i))/(xpr-midxs(i))
        ! calculate weighting factor using W-3 (4.10).
        avl (:) = dabs(vl(:))
        avr (:) = dabs(vr(:))
        deno(:) = avl(:) + avr(:) + 1.d-200
        wl  (:) = avr(:)/deno(:)
        wr  (:) = avl(:)/deno(:)
        dlt (:) = dabs(wl(:) - 0.5d0)
        dlt (:) = dlt(:) * dmin1(0.5d0/(dlt(:)+1.d-200), 1/tau)
        wl  (:) = 0.5d0 + sign(dlt(:), wl(:)-0.5d0)
        wr  (:) = 0.5d0 + sign(dlt(:), wr(:)-0.5d0)
        ! weight to limit the result using (4.3).
        midux(:,i) = wl(:)*vl(:) + wr(:)*vr(:)
    end do loop_weight
end do march

! shift u back to mesh point.
shift_backward: do i = 1, dnx
    u(:,i) = u(:,i) + ux(:,i)*(x(i)-xs(i))
end do shift_backward

deallocate(up)
deallocate(s )

end subroutine solve_nuni_w3
! vim: set ai et nu ts=4 sw=4 tw=79:
