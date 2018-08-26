#include <memory>
#include <iostream>

#include "spacetime.hpp"

int main(int argc, char ** argv)
{
    namespace st = spacetime;

    std::shared_ptr<st::Grid> grid=st::Grid::construct(0, 100, 100);
    std::cout << *grid << std::endl;

    st::Celm ce0 = grid->celm(0);
    st::Celm ce99 = grid->celm(99);
    std::cout << ce0 << " " << ce99 << std::endl;

    st::Celm ce = grid->celm(0);
    ce.move_pos();
    std::cout << "Moved: " << ce << std::endl;
    ce.move_pos();
    std::cout << "Moved: " << ce << std::endl;

    return 0;
}

/* vim: set et ts=4 sw=4: */
