#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include <memory>
#include <vector>

#include "xtensor/xarray.hpp"
#include "xtensor/xio.hpp"
#include "xtensor/xview.hpp"

#include "spacetime/system.hpp"
#include "spacetime/type.hpp"

namespace spacetime
{

class Celm;

class Grid
  : public std::enable_shared_from_this<Grid>
{

public:

    using value_type = real_type;
    using array_type = xt::xarray<value_type, xt::layout_type::row_major>;

private:

    class ctor_passkey {};

public:

    template<class ... Args>
    static std::shared_ptr<Grid> construct(Args&& ... args)
    {
        std::shared_ptr<Grid> instance = std::make_shared<Grid>(std::forward<Args>(args) ..., ctor_passkey());
        return instance;
    }

    Grid(real_type xmin, real_type xmax, size_t nelement, ctor_passkey const &);

    Grid() = delete;
    Grid(Grid const & ) = delete;
    Grid(Grid       &&) = delete;
    Grid & operator=(Grid const & ) = delete;
    Grid & operator=(Grid       &&) = delete;

    real_type xmin() const { return m_xmin; }
    real_type xmax() const { return m_xmax; }
    size_t nelement() const { return m_nelement; }

    Celm element(size_t ielm);
    Celm element(size_t ielm, bool odd_plane);
    Celm element_at(size_t ielm);
    Celm element_at(size_t ielm, bool odd_plane);

    real_type x(size_t ielm) const { return m_xcoord[index_e2c(ielm,0)]; }
    real_type xleft(size_t ielm) const { return m_xcoord[index_e2c(ielm,-2)]; }
    real_type xright(size_t ielm) const { return m_xcoord[index_e2c(ielm,2)]; }
    real_type xneg(size_t ielm) const { return m_xcoord[index_e2c(ielm,-1)]; }
    real_type xpos(size_t ielm) const { return m_xcoord[index_e2c(ielm,1)]; }

private:

    /**
     * Convert element index to coordinate index.
     */
    size_t index_e2c(size_t ielm, ssize_t offset) const { return 1 + ielm*2 + offset; }
    size_t index_e2c(size_t ielm, ssize_t offset, bool odd_plane) const
    {
        size_t icrd = 1 + ielm*2 + offset;
        if (odd_plane) { ++icrd; }
        return icrd;
    }

    real_type       * xptr()       { return m_xcoord.data(); }
    real_type const * xptr() const { return m_xcoord.data(); }
    real_type       * xptr(size_t icrd)       { return m_xcoord.data() + icrd; }
    real_type const * xptr(size_t icrd) const { return m_xcoord.data() + icrd; }
    real_type       * xptr(size_t ielm, ssize_t offset)       { return m_xcoord.data() + index_e2c(ielm, offset); }
    real_type const * xptr(size_t ielm, ssize_t offset) const { return m_xcoord.data() + index_e2c(ielm, offset); }
    real_type       * xptr(size_t ielm, ssize_t offset, bool odd_plane)       { return m_xcoord.data() + index_e2c(ielm, offset, odd_plane); }
    real_type const * xptr(size_t ielm, ssize_t offset, bool odd_plane) const { return m_xcoord.data() + index_e2c(ielm, offset, odd_plane); }

    /**
     * Convert coordinate index to element index.
     */
    size_t index_c2e(size_t icrd) const { return (icrd - 1) >> 1; }

    real_type m_xmin;
    real_type m_xmax;
    size_t m_nelement;

    array_type m_xcoord;

    friend Celm;
    friend std::ostream& operator<<(std::ostream& os, const Grid & grid);

}; /* end class Grid */

inline
Grid::Grid(real_type xmin, real_type xmax, size_t nelement, ctor_passkey const &)
  : m_xmin(xmin), m_xmax(xmax), m_nelement(nelement)
{
    // Mark the boundary of conservation elements.
    const real_type xspace = (xmax - xmin) / nelement;
    m_xcoord = array_type(std::vector<size_t>{nelement*2+1});
    // Fill x-coordinates at CE boundary.
    for (size_t it=0; it<nelement; ++it)
    {
        m_xcoord[it*2] = xmin + xspace * it;
    }
    m_xcoord[m_xcoord.size()-1] = xmax;
    // Fill x-coordinates at CE center.
    for (size_t it=0; it<nelement; ++it)
    {
        m_xcoord[it*2+1] = (m_xcoord[it*2] + m_xcoord[it*2+2])/2;
    }
}

/**
 * A compound conservation element.
 */
class Celm
{

public:

    Celm(Grid & grid, size_t index)
      : m_grid(&grid)
      , m_xptr(grid.xptr(index, 0))
    {}

    Celm(Grid & grid, size_t index, bool odd_plane)
      : m_grid(&grid)
      , m_xptr(grid.xptr(index, 0, odd_plane))
    {}
 
    Celm duplicate() const { return *this; }
    std::shared_ptr<Grid> grid() const { return m_grid->shared_from_this(); }
    index_type index() const { return m_grid->index_c2e(coord_index()); }
    /**
     * Return true for even plane, false for odd plane (temporal).
     */
    bool on_even_plane() const { return !bool((coord_index() - 1) & 1); }

    real_type x() const { return m_grid->m_xcoord(coord_index()); }
    real_type xneg() const { return m_grid->m_xcoord(coord_index()-1); }
    real_type xpos() const { return m_grid->m_xcoord(coord_index()+1); }

    Celm & move(ssize_t offset)
    {
        m_xptr += offset;
        return *this;
    }

    Celm & move_at(ssize_t offset);

    Celm & move_left() { return move(-2); }
    Celm & move_right() { return move(2); }
    Celm & move_neg() { return move(-1); }
    Celm & move_pos() { return move(1); }

    Celm & move_left_at() { return move_at(-2); }
    Celm & move_right_at() { return move_at(2); }
    Celm & move_neg_at() { return move_at(-1); }
    Celm & move_pos_at() { return move_at(1); }

private:

    size_t coord_index() const { return m_xptr - m_grid->xptr(); }

    Grid * m_grid;
    real_type * m_xptr;

    friend Grid;

}; /* end class Celm */

inline Celm Grid::element(size_t ielm)
{
    return Celm(*this, ielm);
}

inline Celm Grid::element(size_t ielm, bool odd_plane)
{
    return Celm(*this, ielm, odd_plane);
}
 
inline Celm Grid::element_at(size_t ielm)
{
    const Celm elm = element(ielm);
    if (elm.coord_index() < 1 || elm.coord_index() >= this->m_xcoord.size()-1) {
        throw std::out_of_range(Formatter()
            << "Grid::element_at(ielm=" << ielm << ") icrd = " << elm.coord_index()
            << " outside the interval [1, " << this->m_xcoord.size()-1 << ")"
        );
    }
    return elm;
}

inline Celm Grid::element_at(size_t ielm, bool odd_plane)
{
    const Celm elm = element(ielm, odd_plane);
    if (elm.coord_index() < 1 || elm.coord_index() >= this->m_xcoord.size()-1) {
        throw std::out_of_range(Formatter()
            << "Grid::element_at(ielm=" << ielm << ", odd_plane=" << odd_plane
            << ") icrd = " << elm.coord_index()
            << " outside the interval [1, " << this->m_xcoord.size()-1 << ")"
        );
    }
    return elm;
}

inline Celm & Celm::move_at(ssize_t offset)
{
    const ssize_t icrd = coord_index() + offset;
    if (icrd < 1 || icrd >= m_grid->m_xcoord.size()-1) {
        throw std::out_of_range(Formatter()
            << "Celm::move_at() (coord_index = " << coord_index()
            << ", offset = " << offset << ") icrd = " << icrd
            << " outside the interval [1, " << m_grid->m_xcoord.size()-1 << ")"
        );
    }
    return move(offset);
}

class SolutionElement
{
}; /* end class SolutionElement */

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
