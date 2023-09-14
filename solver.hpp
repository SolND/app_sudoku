#ifndef _SOLVER_HPP
#define _SOLVER_HPP
#include <cassert>
#include <cstdlib>
#include <iostream>

class Board
{
public:
    Board(){}
    Board(int size) : size_table(size)
    {
        // ma tran theo doi cac gia tri
        grid = new int *[size_table];
        immutable = new bool*[size_table];
        infeasible = new bool*[size_table];

        for(int i = 0; i < size_table; i++)
        {
            grid[i] = new int[size_table];
            immutable[i] = new bool[size_table];
            infeasible[i] = new bool[size_table];
        }


        for(int i = 0; i < size_table; i++)
        {
            for(int j = 0; j < size_table; j++)
            {
                grid[i][j] = 0;
                infeasible[i][j] = false;
                immutable[i][j] = true;
            }
        }
    }

    // ham huy board
    ~Board()
    {
        for(int i = 0; i < size_table; i++)
        {
            delete [] grid[i];
        }
        delete [] grid;

        for(int i = 0; i < size_table; i++)
        {
            delete [] immutable[i];
        }
        delete [] immutable;

        for(int i = 0; i < size_table; i++)
        {
            delete [] infeasible[i];
        }
        delete [] infeasible;
    }

    int& operator() (int x, int y)
    {
        assert(x < size_table && y < size_table);
        return grid[x][y];
    }
    void print_puzzle();
    void clear_puzzle();
    bool check_puzzle();
    bool in_bounds(int val);
    bool feasible_user(int row, int col, int val);

    void assign_value(int x, int y, int val);
    void assign_immutable(int x, int y, bool val);
    void set_from_array(int a[4][4]);
    bool check_immutable(int x, int y);
    bool is_problem(int x, int y);
    int get_size();

private:
    int size_table;
    int **grid; //luu tru cac gia tri trong bang 
    bool **immutable; // theo doi cac o khong thay doi
    bool **infeasible; // theo doi cac o gay ra loi
};

bool check_feasible(Board &board, int row, int col, int val);
bool solve(Board &board, int row, int col);
int* gen_perm(int size);
Board generate_puzzle(int size, int nobs);

#endif