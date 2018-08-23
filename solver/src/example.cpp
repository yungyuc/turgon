#include <memory>
#include <iostream>

#include "spacetime.hpp"

int main(int argc, char ** argv)
{
    namespace st = spacetime;

    std::shared_ptr<st::Grid> grid=st::Grid::construct(0, 100, 100);
    std::cout << *grid << std::endl;

    st::Element ce0 = grid->element(0);
    st::Element ce99 = grid->element(99);
    std::cout << ce0 << " " << ce99 << std::endl;

    st::Element ce = grid->element(0);
    ce.move_pos();
    std::cout << "Moved: " << ce << std::endl;
    ce.move_pos();
    std::cout << "Moved: " << ce << std::endl;

    return 0;
}

/* vim: set et ts=4 sw=4: */
