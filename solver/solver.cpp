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
    using array_type = xt::xarray<value_type, xt::layout_type::dynamic>;

private:

    class ctor_passkey {};

public:

    template<class ... Args>
    static std::shared_ptr<Grid> construct(Args&& ... args) {
        std::shared_ptr<Grid> instance = std::make_shared<Grid>(std::forward<Args>(args) ..., ctor_passkey());
        return instance;
    }

    Grid(real_type xmin, real_type xmax, size_t nelm, ctor_passkey const &)
      : m_xcoord(std::vector<size_t>{nelm+3}, xt::layout_type::row_major)
    {
        const size_t xsize = m_xcoord.size();
        const real_type xspace = (xmax - xmin) / nelm;
        for (size_t it=0; it<xsize; ++it) {
            const real_type xval = xspace * (static_cast<ssize_t>(it) - 1) + xmin;
            m_xcoord[it] = xval;
        }
    }

    Grid() = delete;
    Grid(Grid const & ) = delete;
    Grid(Grid       &&) = delete;
    Grid & operator=(Grid const & ) = delete;
    Grid & operator=(Grid       &&) = delete;

    size_t nelement() const { return m_xcoord.size() - 3; }

    ConservationElement element(size_t ielm);

    real_type x(size_t ielm) const { return m_xcoord[ielm+1]; }
    real_type xprev(size_t ielm) const { return m_xcoord[ielm]; }
    real_type xnext(size_t ielm) const { return m_xcoord[ielm+2]; }
    real_type xneg(size_t ielm) const { return (x(ielm) + xprev(ielm)) / 2; }
    real_type xpos(size_t ielm) const { return (x(ielm) + xnext(ielm)) / 2; }

private:

    array_type m_xcoord;

    friend ConservationElement;
    friend std::ostream& operator<<(std::ostream& os, const Grid & grid);

}; /* end class Grid */

std::ostream& operator<<(std::ostream& os, const Grid & grid)
{
    os << "Grid(xmin=" << grid.x(0) << ", xmax=" << grid.x(grid.nelement()) << ", nelement=" << grid.nelement() << ")";
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
      , m_index(index)
    {}

    std::shared_ptr<Grid> grid() const { return m_grid->shared_from_this(); }
    index_type index() const { return m_index; }

    real_type x() const { return m_grid->x(m_index); }
    real_type xneg() const { return m_grid->xneg(m_index); }
    real_type xpos() const { return m_grid->xpos(m_index); }

private:

    Grid * m_grid;
    size_t m_index;

}; /* end class ConservationElement */

inline ConservationElement Grid::element(size_t ielm)
{
    return ConservationElement(*this, ielm);
}

std::ostream& operator<<(std::ostream& os, const ConservationElement & elm)
{
    os << "ConservationElement(index=" << elm.index() << ", x=" << elm.x() << ", xneg=" << elm.xneg() << ", xpos=" << elm.xpos() << ")";
    return os;
}

} /* end namespace spacetime */

int main(int argc, char ** argv)
{

    namespace st = spacetime;

    std::shared_ptr<st::Grid> grid=st::Grid::construct(0, 100, 100);
    std::cout << *grid << std::endl;

    st::ConservationElement ce(*grid, 0);
    std::cout << ce << std::endl;

    return 0;
}

/* vim: set et ts=4 sw=4: */
