#include <sstream>
#include "solver.hpp"
#include "altproj.hpp"
#include "game.hpp"
#include "test.hpp"

int main(int argc, char *argv[])
{
    int random_seed;
    int size_game = 9;
    int nobs = 10;

    srand(random_seed); // set random seed

    // Welcome message
    std::cout << "===========================================" << std::endl;
    std::cout << "============ Welcome to Sudoku! ===========" << std::endl;
    std::cout << "===========================================" << std::endl;

    play_game(size_game, nobs);

    return 0;
}