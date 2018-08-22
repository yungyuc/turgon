#include <memory>
#include <vector>
#include <iostream>

#include "xtensor/xarray.hpp"
#include "xtensor/xio.hpp"
#include "xtensor/xview.hpp"

namespace spacetime
{

using real_type = double;
using index_type = int32_t;

class ConservationElement;

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

    ConservationElement element(size_t ielm);

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

    friend ConservationElement;
    friend std::ostream& operator<<(std::ostream& os, const Grid & grid);

}; /* end class Grid */

std::ostream& operator<<(std::ostream& os, const Grid & grid)
{
    os << "Grid(xmin=" << grid.xmin() << ", xmax=" << grid.xmax() << ", nelement=" << grid.nelement() << ", ncoord=" << grid.m_xcoord.size() << ")";
    return os;
}

/**
 * A compound conservation element.
 */
class ConservationElement
{

public:

    ConservationElement(Grid & grid, size_t index)
      : m_grid(&grid)
      , m_icrd(grid.index_e2c(index, 0))
    {}

    std::shared_ptr<Grid> grid() const { return m_grid->shared_from_this(); }
    index_type index() const { return m_grid->index_c2e(m_icrd); }
    /**
     * Return true for even plane, false for odd plane (temporal).
     */
    bool even_plane() const { return !bool((m_icrd - 3) & 1); }

    real_type x() const { return m_grid->m_xcoord(m_icrd); }
    real_type xneg() const { return m_grid->m_xcoord(m_icrd-1); }
    real_type xpos() const { return m_grid->m_xcoord(m_icrd+1); }

    ConservationElement & move(ssize_t offset)
    {
        m_icrd += offset;
        return *this;
    }

    ConservationElement & move_left() { return move(-2); }
    ConservationElement & move_right() { return move(2); }
    ConservationElement & move_neg() { return move(-1); }
    ConservationElement & move_pos() { return move(1); }

private:

    Grid * m_grid;
    size_t m_icrd; //< Coordinate index.

}; /* end class ConservationElement */

inline ConservationElement Grid::element(size_t ielm)
{
    return ConservationElement(*this, ielm);
}

std::ostream& operator<<(std::ostream& os, const ConservationElement & elm)
{
    os << "ConservationElement(";
    if (elm.even_plane())
    {
        os << "even, ";
    }
    else
    {
        os << "odd, ";
    }
    os << "index=" << elm.index() << ", x=" << elm.x() << ", xneg=" << elm.xneg() << ", xpos=" << elm.xpos() << ")";
    return os;
}

} /* end namespace spacetime */

int main(int argc, char ** argv)
{

    namespace st = spacetime;

    std::shared_ptr<st::Grid> grid=st::Grid::construct(0, 100, 100);
    std::cout << *grid << std::endl;

    st::ConservationElement ce0 = grid->element(0);
    st::ConservationElement ce99 = grid->element(99);
    std::cout << ce0 << " " << ce99 << std::endl;

    st::ConservationElement ce = grid->element(0);
    ce.move_pos();
    std::cout << "Moved: " << ce << std::endl;
    ce.move_pos();
    std::cout << "Moved: " << ce << std::endl;

    return 0;
}

/* vim: set et ts=4 sw=4: */
