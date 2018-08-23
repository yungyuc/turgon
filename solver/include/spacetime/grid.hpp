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

class Element;

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

    Grid(real_type xmin, real_type xmax, size_t nelm, ctor_passkey const &)
      : m_xmin(xmin), m_xmax(xmax), m_nelement(nelm)
    {
        initialize_arrays();
    }

    Grid() = delete;
    Grid(Grid const & ) = delete;
    Grid(Grid       &&) = delete;
    Grid & operator=(Grid const & ) = delete;
    Grid & operator=(Grid       &&) = delete;

    real_type xmin() const { return m_xmin; }
    real_type xmax() const { return m_xmax; }
    size_t nelement() const { return m_nelement; }

    Element element(size_t ielm);
    Element element_at(size_t ielm);

    real_type x(size_t ielm) const { return m_xcoord[index_e2c(ielm,0)]; }
    real_type xleft(size_t ielm) const { return m_xcoord[index_e2c(ielm,-2)]; }
    real_type xright(size_t ielm) const { return m_xcoord[index_e2c(ielm,2)]; }
    real_type xneg(size_t ielm) const { return m_xcoord[index_e2c(ielm,-1)]; }
    real_type xpos(size_t ielm) const { return m_xcoord[index_e2c(ielm,1)]; }

private:

    void initialize_arrays()
    {
        // Mark the boundary of conservation elements.
        const real_type xspace = (m_xmax - m_xmin) / m_nelement;
        const array_type mark = xt::arange(m_xmin-xspace, m_xmax+xspace*2, xspace);
        // Fill x-coordinates.
        auto left = xt::view(mark, xt::range(0, mark.size()-1));
        auto right = xt::view(mark, xt::range(1, mark.size()));
        const array_type middle = (left + right) / 2;
        m_xcoord = array_type(std::vector<size_t>{mark.size() + middle.size()});
        for (size_t it=0; it<middle.size(); ++it) {
            m_xcoord[it*2] = mark[it];
            m_xcoord[it*2+1] = middle[it];
        }
        m_xcoord[m_xcoord.size()-1] = mark[mark.size()-1];
    }

    /**
     * Convert element index to coordinate index.
     */
    size_t index_e2c(size_t ielm, ssize_t offset) const { return 3 + ielm*2 + offset; }

    /**
     * Convert coordinate index to element index.
     */
    size_t index_c2e(size_t icrd) const { return (icrd - 3) >> 1; }

    real_type m_xmin;
    real_type m_xmax;
    size_t m_nelement;

    array_type m_xcoord;

    friend Element;
    friend std::ostream& operator<<(std::ostream& os, const Grid & grid);

}; /* end class Grid */

/**
 * A compound conservation element.
 */
class Element
{

public:

    Element(Grid & grid, size_t index)
      : m_grid(&grid)
      , m_icrd(grid.index_e2c(index, 0))
    {}

    Element duplicate() const { return *this; }
    std::shared_ptr<Grid> grid() const { return m_grid->shared_from_this(); }
    index_type index() const { return m_grid->index_c2e(m_icrd); }
    /**
     * Return true for even plane, false for odd plane (temporal).
     */
    bool on_even_plane() const { return !bool((m_icrd - 3) & 1); }

    real_type x() const { return m_grid->m_xcoord(m_icrd); }
    real_type xneg() const { return m_grid->m_xcoord(m_icrd-1); }
    real_type xpos() const { return m_grid->m_xcoord(m_icrd+1); }

    Element & move(ssize_t offset)
    {
        m_icrd += offset;
        return *this;
    }

    Element & move_at(ssize_t offset)
    {
        const ssize_t icrd = m_icrd + offset;
        if (icrd < 0 || icrd >= m_grid->m_xcoord.size()) {
            throw std::out_of_range(Formatter()
                << "Element::move_at() m_icrd = " << m_icrd
                << ", offset = " << offset << ", icrd = " << icrd
                << " outside the interval [0, " << m_grid->m_xcoord.size() << ")"
            );
        }
        m_icrd = icrd;
        return *this;
    }

    Element & move_left() { return move(-2); }
    Element & move_right() { return move(2); }
    Element & move_neg() { return move(-1); }
    Element & move_pos() { return move(1); }

    Element & move_left_at() { return move_at(-2); }
    Element & move_right_at() { return move_at(2); }
    Element & move_neg_at() { return move_at(-1); }
    Element & move_pos_at() { return move_at(1); }

private:

    Grid * m_grid;
    size_t m_icrd; //< Coordinate index.

}; /* end class Element */

inline Element Grid::element(size_t ielm)
{
    return Element(*this, ielm);
}

inline Element Grid::element_at(size_t ielm)
{
    const ssize_t icrd = this->index_e2c(ielm, 0);
    if (icrd < 0 || icrd >= this->m_xcoord.size()) {
        throw std::out_of_range(Formatter()
            << "Grid::element_at() icrd = " << icrd
            << " outside the interval [0, " << this->m_xcoord.size() << ")"
        );
    }
    return Element(*this, ielm);
}

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
