#include <memory>
#include <iostream>

#include "spacetime.hpp"

int main(int argc, char ** argv)
{
    namespace st = spacetime;

    std::shared_ptr<st::Grid> grid=st::Grid::construct(0, 100, 100);
    std::shared_ptr<st::Solution> sol=st::Solution::construct(grid, 1, 1);
    std::cout << *grid << std::endl;
    std::cout << *sol << std::endl;

    st::Celm ce0 = sol->celm(0, false);
    st::Celm ce99 = sol->celm(99, false);
    std::cout << (ce0 == ce99) << std::endl;
    std::cout << (ce0 != ce99) << std::endl;
    std::cout << (ce0 <  ce99) << std::endl;
    std::cout << (ce0 <= ce99) << std::endl;
    std::cout << (ce0 >  ce99) << std::endl;
    std::cout << (ce0 >= ce99) << std::endl;
    std::cout << ce0 << " " << ce99 << std::endl;

    st::Celm ce = sol->celm(0, false);
    ce.move_pos();
    std::cout << "Moved: " << ce << std::endl;
    ce.move_pos();
    std::cout << "Moved: " << ce << std::endl;

    return 0;
}

/* vim: set et ts=4 sw=4: */
