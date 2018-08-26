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

template<class ET> class ElementBase;
class Celm;
class Selm;

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

    Grid(real_type xmin, real_type xmax, size_t ncelm, ctor_passkey const &);

    Grid() = delete;
    Grid(Grid const & ) = delete;
    Grid(Grid       &&) = delete;
    Grid & operator=(Grid const & ) = delete;
    Grid & operator=(Grid       &&) = delete;

    real_type xmin() const { return m_xmin; }
    real_type xmax() const { return m_xmax; }
    size_t ncelm() const { return m_ncelm; }

    Celm celm(size_t ielm);
    Celm celm(size_t ielm, bool odd_plane);
    Celm celm_at(size_t ielm);
    Celm celm_at(size_t ielm, bool odd_plane);

    Selm selm(size_t ielm);
    Selm selm(size_t ielm, bool odd_plane);
    Selm selm_at(size_t ielm);
    Selm selm_at(size_t ielm, bool odd_plane);

    size_t xsize() const { return m_xcoord.size(); }

public:

    class CelmPK { private: CelmPK() = default; friend Celm; };

    /**
     * Get pointer to an coordinate value using conservation-element index.
     */
    real_type       * xptr_celm(size_t ielm, CelmPK const &)       { return xptr(xindex_celm(ielm)); }
    real_type const * xptr_celm(size_t ielm, CelmPK const &) const { return xptr(xindex_celm(ielm)); }
    real_type       * xptr_celm(size_t ielm, bool odd_plane, CelmPK const &)       { return xptr(xindex_celm(ielm, odd_plane)); }
    real_type const * xptr_celm(size_t ielm, bool odd_plane, CelmPK const &) const { return xptr(xindex_celm(ielm, odd_plane)); }

public:

    class SelmPK { private: SelmPK() = default; friend Selm; };

    /**
     * Get pointer to an coordinate value using conservation-element index.
     */
    real_type       * xptr_selm(size_t ielm, SelmPK const &)       { return xptr(xindex_selm(ielm)); }
    real_type const * xptr_selm(size_t ielm, SelmPK const &) const { return xptr(xindex_selm(ielm)); }
    real_type       * xptr_selm(size_t ielm, bool odd_plane, SelmPK const &)       { return xptr(xindex_selm(ielm, odd_plane)); }
    real_type const * xptr_selm(size_t ielm, bool odd_plane, SelmPK const &) const { return xptr(xindex_selm(ielm, odd_plane)); }

private:

    /**
     * Convert celm index to coordinate index.
     */
    size_t xindex_celm(size_t ielm) const { return 1 + ielm*2; }
    size_t xindex_celm(size_t ielm, bool odd_plane) const
    {
        size_t xindex = xindex_celm(ielm);
        if (odd_plane) { ++xindex; }
        return xindex;
    }

    /**
     * Convert selm index to coordinate index.
     */
    size_t xindex_selm(size_t ielm) const { return ielm*2; }
    size_t xindex_selm(size_t ielm, bool odd_plane) const
    {
        size_t xindex = xindex_selm(ielm);
        if (odd_plane) { ++xindex; }
        return xindex;
    }

    /**
     * Get pointer to an coordinate value using coordinate index.
     */
    real_type       * xptr()       { return m_xcoord.data(); }
    real_type const * xptr() const { return m_xcoord.data(); }
    real_type       * xptr(size_t xindex)       { return m_xcoord.data() + xindex; }
    real_type const * xptr(size_t xindex) const { return m_xcoord.data() + xindex; }

    real_type m_xmin;
    real_type m_xmax;
    size_t m_ncelm;

    array_type m_xcoord;

    template<class ET> friend class ElementBase;
    friend std::ostream& operator<<(std::ostream& os, const Grid & grid);

}; /* end class Grid */

inline
Grid::Grid(real_type xmin, real_type xmax, size_t ncelm, ctor_passkey const &)
  : m_xmin(xmin), m_xmax(xmax), m_ncelm(ncelm)
{
    // Mark the boundary of conservation celms.
    const real_type xspace = (xmax - xmin) / ncelm;
    m_xcoord = array_type(std::vector<size_t>{ncelm*2+1});
    // Fill x-coordinates at CE boundary.
    for (size_t it=0; it<ncelm; ++it)
    {
        m_xcoord[it*2] = xmin + xspace * it;
    }
    m_xcoord[m_xcoord.size()-1] = xmax;
    // Fill x-coordinates at CE center.
    for (size_t it=0; it<ncelm; ++it)
    {
        m_xcoord[it*2+1] = (m_xcoord[it*2] + m_xcoord[it*2+2])/2;
    }
}

template< class ET >
class ElementBase
{

public:

    using base_type = ElementBase;
    using element_type = ET;

    ElementBase(Grid & grid, real_type * xptr)
      : m_grid(&grid), m_xptr(xptr)
    {}

    element_type duplicate() { return *static_cast<element_type *>(this); }

    Grid const & grid() const { return *m_grid; }

    real_type x() const { return *m_xptr; }
    real_type xneg() const { return *(m_xptr-1); }
    real_type xpos() const { return *(m_xptr+1); }

protected:

    size_t xindex() const { return m_xptr - m_grid->xptr(); }

    Grid * m_grid;
    real_type * m_xptr;

    friend Grid;

}; /* end class ElementBase */

/**
 * A compound conservation celm.
 */
class Celm
  : public ElementBase<Celm>
{

public:

    Celm(Grid & grid, size_t index)
      : base_type(
            grid
          , grid.xptr_celm(index, Grid::CelmPK())
        )
    {}

    Celm(Grid & grid, size_t index, bool odd_plane)
      : base_type(
            grid
          , grid.xptr_celm(index, odd_plane, Grid::CelmPK())
        )
    {}

    /**
     * Celm index.
     */
    index_type index() const { return (xindex() - 1) >> 1; }

    /**
     * Return true for even plane, false for odd plane (temporal).
     */
    bool on_even_plane() const { return !bool((xindex() - 1) & 1); }

    Celm & move(ssize_t offset)
    {
        m_xptr += offset;
        return *static_cast<Celm *>(this);
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

}; /* end class Celm */

Celm & Celm::move_at(ssize_t offset)
{
    const ssize_t xindex = this->xindex() + offset;
    if (xindex < 1 || xindex >= m_grid->xsize()-1) {
        throw std::out_of_range(Formatter()
            << "Celm(xindex=" << this->xindex() << ")::move_at(offset=" << offset
            << "): xindex = " << xindex
            << " outside the interval [1, " << m_grid->xsize()-1 << ")"
        );
    }
    return move(offset);
}

inline Celm Grid::celm(size_t ielm)
{
    return Celm(*this, ielm);
}

inline Celm Grid::celm(size_t ielm, bool odd_plane)
{
    return Celm(*this, ielm, odd_plane);
}

inline Celm Grid::celm_at(size_t ielm)
{
    const Celm elm = celm(ielm);
    if (elm.xindex() < 1 || elm.xindex() >= this->xsize()-1) {
        throw std::out_of_range(Formatter()
            << "Grid::celm_at(ielm=" << ielm << "): xindex = " << elm.xindex()
            << " outside the interval [1, " << this->xsize()-1 << ")"
        );
    }
    return elm;
}

inline Celm Grid::celm_at(size_t ielm, bool odd_plane)
{
    const Celm elm = celm(ielm, odd_plane);
    if (elm.xindex() < 1 || elm.xindex() >= this->xsize()-1) {
        throw std::out_of_range(Formatter()
            << "Grid::celm_at(ielm=" << ielm << ", odd_plane=" << odd_plane
            << "): xindex = " << elm.xindex()
            << " outside the interval [1, " << this->xsize()-1 << ")"
        );
    }
    return elm;
}

/**
 * A solution element.
 */
class Selm
  : public ElementBase<Celm>
{

public:

    Selm(Grid & grid, size_t index)
      : base_type(
            grid
          , grid.xptr_selm(index, Grid::SelmPK())
        )
    {}

    Selm(Grid & grid, size_t index, bool odd_plane)
      : base_type(
            grid
          , grid.xptr_selm(index, odd_plane, Grid::SelmPK())
        )
    {}

    /**
     * Selm index.
     */
    index_type index() const { return xindex() >> 1; }

    /**
     * Return true for even plane, false for odd plane (temporal).
     */
    bool on_even_plane() const { return !bool(xindex() & 1); }

}; /* end class Selm */

inline Selm Grid::selm(size_t ielm)
{
    return Selm(*this, ielm);
}

inline Selm Grid::selm(size_t ielm, bool odd_plane)
{
    return Selm(*this, ielm, odd_plane);
}

inline Selm Grid::selm_at(size_t ielm)
{
    const Selm elm = selm(ielm);
    if (static_cast<ssize_t>(elm.xindex()) < 0 || elm.xindex() >= this->xsize()) {
        throw std::out_of_range(Formatter()
            << "Grid::selm_at(ielm=" << ielm << "): xindex = " << elm.xindex()
            << " outside the interval [0, " << this->xsize() << ")"
        );
    }
    return elm;
}

inline Selm Grid::selm_at(size_t ielm, bool odd_plane)
{
    const Selm elm = selm(ielm, odd_plane);
    if (static_cast<ssize_t>(elm.xindex()) < 0 || elm.xindex() >= this->xsize()) {
        throw std::out_of_range(Formatter()
            << "Grid::selm_at(ielm=" << ielm << ", odd_plane=" << odd_plane
            << "): xindex = " << elm.xindex()
            << " outside the interval [0, " << this->xsize() << ")"
        );
    }
    return elm;
}

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
