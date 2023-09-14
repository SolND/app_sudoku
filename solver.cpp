#include "solver.hpp"
#include <cassert>
#include <math.h>
#include <ctime>
#include <cstdlib>

void Board::assign_value(int x, int y, int val)
{
    (*this)(x,y) = val;
}

void Board::assign_immutable(int x, int y, bool val)
{
    immutable[x][y] = val;
}

bool Board::check_immutable(int x, int y)
{
    return immutable[x][y];
}

bool Board::is_problem(int x, int y)
{
    return infeasible[x][y];
}

int Board::get_size()
{
    return size_table;
}

void Board::set_from_array(int arr[4][4])
{
    for(int i = 0; i < size_table; i++)
        for(int j = 0; i < size_table; j++)
            grid[i][j] = arr[i][j];
}

//print the board
void Board::print_puzzle()
{
    int block_size = (int)sqrt(size_table);
    int extra_space = size_table / 10;
    
    for(int i = 0; i < size_table; i++)
    {
        std::cout << "=---";
        for(int j = 0; i < extra_space; j++)
        {
            std::cout << "-";
        }
    }

    std::cout << "=\n";

    for(int i = 0; i <size_table; i++)
    {
        std::cout << "| ";
        for(int j = 0; j < size_table; j++)
        {
            if((*this)(i,j) == 0)
            {
                std::cout << ".";
                for(int n = 0; n <extra_space; n++)
                {
                    std::cout << " ";
                }
            }
            else
            {
                if(!is_problem(i,j))
                {
                    std::cout << (*this)(i,j);
                }
                else
                {
                    std::cout << "\033[31m" << (*this)(i,j) << "\033[39m";
                }
                
                int num = (*this)(i,j)/10;

                while(num < extra_space)
                {
                    std::cout << " ";
                    num++;
                }
            }

            if((j + 1) % block_size == 0)
            {
                std::cout << " | ";
            }
            else
            {
                std::cout << "   ";
            }
        }
        std::cout << "\n";

        if((i + 1) %block_size == 0)
        {
            for(int j = 0; j < size_table;j++)
            {
                std::cout << "=---";
                for(int n = 0; n < extra_space; n++)
                {
                    std::cout << "-";
                }
            }
            std::cout << "=\n";
        }
    }
}

// check cau do da hoan thanh chua
bool Board::check_puzzle()
{
    int val = 0;

    for(int i = 0; i < size_table; i++)
    {
        for(int j = 0; j < size_table; j++)
        {
            val = (*this)(i,j);
            (*this)(i,j) = 0;

            if(!check_feasible(*this, i, j, val))
            {
                (*this)(i,j) = val;
                return false;
            }
            (*this)(i,j) = val;
        }
    }
    return true;
}

// lam trong toan bo cac o khong thay doi 
void Board::clear_puzzle()
{
    for(int i = 0; i < size_table; i++)
        for(int j = 0; i < size_table; j++)
            if(!check_immutable(i,j))
                (*this)(i,j) = 0;
}

// pham vi tu 1 den 9 tren ma tan 9x9
bool Board::in_bounds(int val){
    if ((val > 0) && (val <= size_table))
        return true;
    return false;
}

bool Board::feasible_user(int row, int col, int val)
{
    int block_size = (int)sqrt(size_table);

    if(row >= size_table)
    {
        std::cout << "Ban khong the choi ngoai bang tro choi!\n";
        return false;
    }

    if(col >= size_table)
    {
        std::cout << "Ban khong the choi ngoai bang tro choi!\n";
        return false;
    }

    bool is_feasible = true;

    for(int i = 0; i < size_table; i++)
    {
        if((*this)(row, i) == val)
        {
            infeasible[row][i] = true;
            is_feasible = false;
        }
        else
        {
            infeasible[row][i] = false;
        }
    }

    for(int i = 0; i < size_table; i++)
    {
        if((*this)(col, i) == val)
        {
            infeasible[col][i] = true;
            is_feasible = false;
        }
        else
        {
            infeasible[col][i] = false;
        }
    }

    int block_row = block_size*(row/block_size);
    int block_col = block_size*(col/block_size);

    for(int i = 0; i < block_size; i++)
    {
        for(int j = 0; j < block_size; j++)
        {
            if((*this)(block_row + i, block_col +j) == val)
            {
                infeasible[block_row + i] [block_col +j] = true;
                is_feasible = false;
            }
            else
            {
                infeasible[block_row + i][block_col + j] = false;
            }
        }
    }
    return is_feasible;
}

bool check_feasible(Board &board, int row, int col, int val)
{
    int size = board.get_size();
    assert(row < size);
    assert(col < size);
    int block_size = (int)sqrt(size);
    for(int i = 0; i < size; i++)
    {
        if(board(row, i) == val)
            return false;
    }

    for(int i = 0; i < size; i++)
    {
        if(board(i, col) == val)
            return false;
    }

    int block_row = block_size*(row/block_size);
    int block_col = block_size*(col/block_size);

    for(int i = 0; i < block_size; i++)
    {
        for(int j = 0; j < block_size; j++)
        {
            if(board(block_row + i, block_col +j) == val)
                return false;
        }
    }
    return true;
}

bool solve(Board &board, int row, int col)
{
    int size = board.get_size();
    assert(size == pow(sqrt(size), 2));

    if(row == size)
        return true;
    
    if(board(row, col) != 0)
    {
        if(col == (size -1))
        {
            if(solve(board, row + 1, 0))
                return true;
        }
        else 
        {
            if(solve(board, row, col + 1))
                return true;
        }
        return false;
    }
    for(int val = 1; val <= size; val++){
        if(check_feasible(board, row, col, val)){
            board(row,col) = val;
            if(col == (size-1)){
                if(solve(board, row+1, 0)) 
                    return true;
            } else {
                if(solve(board, row, col+1)) 
                    return true;
            }
        }
    }
    board(row, col) = 0;
    return false;
}

Board generate_puzzle(int size, int nobs)
{
    assert(nobs <= size*size);
    Board board(size);

    int *perm = gen_perm(size);
    
    for(int i = 0; i < size; i++)
    {
        board(i, i) = perm[i];
    }
    delete [] perm;

    bool issolved = solve(board, 0, 0);
    assert(issolved);

    perm = gen_perm(size*size);
    int x, y;

    for(int i = 0;i < (size*size - nobs); i++)
    {
        x = (perm[i] - 1)/size;
        y = (perm[i] - 1)%size;
        board(x,y) = 0;
        board.assign_immutable(x, y, false);
    }

    delete [] perm;
    return board;
}

int* gen_perm(int size){

    // initialize array [1,...,N]
    int *x = new int[size];
    for(int i = 0; i < size; i++)
        x[i] = i+1;

    // generate random permutation of [1,...,N]
    int rindex;
    int temp;
    for(int i = (size-1); i > 0; i--)
    {
        rindex = rand()%(i+1);
        temp = x[i];
        x[i] = x[rindex];
        x[rindex] = temp;
    }

    return x;

}