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
#include "spacetime/ElementBase_decl.hpp"

namespace spacetime
{

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
        return std::make_shared<Grid>(std::forward<Args>(args) ..., ctor_passkey());
    }

    Grid(real_type xmin, real_type xmax, size_t ncelm, ctor_passkey const &);

    Grid(array_type const & xloc, ctor_passkey const &) { init_from_array(xloc); }

    Grid() = delete;
    Grid(Grid const & ) = delete;
    Grid(Grid       &&) = delete;
    Grid & operator=(Grid const & ) = delete;
    Grid & operator=(Grid       &&) = delete;

    real_type xmin() const { return m_xmin; }
    real_type xmax() const { return m_xmax; }
    size_t ncelm() const { return m_ncelm; }
    size_t nselm() const { return m_ncelm+1; }

    Celm celm(size_t ielm);
    Celm celm(size_t ielm, bool odd_plane);
    Celm celm_at(size_t ielm);
    Celm celm_at(size_t ielm, bool odd_plane);

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
    size_t xindex_celm(size_t ielm) const { return 2 + (ielm << 1); }
    size_t xindex_celm(size_t ielm, bool odd_plane) const
    {
        size_t xindex = xindex_celm(ielm);
        if (odd_plane) { ++xindex; }
        return xindex;
    }

    /**
     * Convert selm index to coordinate index.
     */
    size_t xindex_selm(size_t ielm) const { return 1 + (ielm << 1); }
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

    void init_from_array(array_type const & xloc);

    real_type m_xmin;
    real_type m_xmax;
    size_t m_ncelm;

    array_type m_xcoord;

    template<class ET> friend class ElementBase;
    friend std::ostream& operator<<(std::ostream& os, const Grid & grid);

}; /* end class Grid */

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
