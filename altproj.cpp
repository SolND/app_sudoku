#include "altproj.hpp"
#include <algorithm>

arma::cube RC1(const arma::cube &Q)
{
    int n = Q.n_rows;
    arma::cube P(n, n, n, arma::fill::zeros);
    arma::uvec seq = arma::linspace<arma::uvec>(0, n-1, n), ix(n), order(n);
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            ix = j*n*n +n *seq + i;
            order = arma::sort_index(Q.elem(ix), "descend");
            P(i, order(0), j) = 1;
        }
    }
    return 2*P - Q;
}

arma::cube RC2(const arma::cube &Q)
{
    int n = Q.n_rows;
    arma::cube P(n, n, n, arma::fill::zeros);
    arma::uvec seq = arma::linspace<arma::uvec>(0, n-1, n), ix(n), order(n);
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            ix = j*n*n +n *seq + i;
            order = arma::sort_index(Q.elem(ix), "descend");
            P(order(0), i, j) = 1;
        }
    }
    return 2*P - Q;
}

arma::cube RC3(const arma::cube &Q)
{
    int n = Q.n_rows;
    arma::cube P(n, n, n, arma::fill::zeros);
    int block_size = sqrt(n);
    arma::uvec mask(block_size*block_size);
    arma::uvec ix(block_size*block_size), order;

    for(int i = 0; i < block_size; i++)
    for(int j = 0; j < block_size; j++)
      mask(i+block_size*j) = i + n*j;


    for(int k = 0; k < n; k++)
        for(int i = 0; i < block_size; i++)
            for(int j = 0; j < block_size; j++){
            ix = mask + (i*block_size + j*n*block_size);
            order = arma::sort_index(Q.elem(ix+k*n*n), "descend");
            P(ix(order(0)) + k*n*n) = 1;
        }
    return 2*P - Q;
}

arma::cube RC4(const arma::cube &Q)
{
    int n = Q.n_rows;
    arma::cube P(n, n, n, arma::fill::zeros);
    arma::uvec seq = arma::linspace<arma::uvec>(0, n-1, n), ix(n), order(n);

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            ix = j*n*n +n *seq + i;
            order = arma::sort_index(Q.elem(ix), "descend");
            P(i,j,order(0)) = 1;
        }
    }
    return 2*P - Q;
}

arma::cube RC5(const arma::cube &Q, Board &board)
{
    int n = Q.n_rows;
    arma::cube P = Q;

    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            if(board(i,j) != 0)
                P(i,j,board(i,j)-1) = 1;
    return 2*P - Q;
}

Board cube2Board(const arma::cube &Q)
{
    int n = Q.n_rows;
    Board solved_board(n);
    arma::uword ix;
    arma::vec A;

    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
        {
            A = Q.tube(i,j);
            A.max(ix);
            solved_board(i,j) = (int)ix + 1;
        }
    return solved_board;
}

Board DR(Board& board)
{
    int n = board.get_size();
    double err;
    arma::cube X1(n,n,n,arma::fill::zeros),
        X2(n,n,n,arma::fill::zeros),
        X3(n,n,n,arma::fill::zeros),
        X4(n,n,n,arma::fill::zeros),
        X5(n,n,n,arma::fill::zeros),
        Z(n,n,n,arma::fill::zeros);
        
    for(int i =0; i < MAXITS; i++)
    {
        Z = (X1+X2+X3+X4+X5)/5;
        if( i%50 == 0 )
        {
            err = accu(abs(Z - RC1(Z))) + accu(abs(Z - RC2(Z))) +
                accu(abs(Z - RC3(Z))) + accu(abs(Z - RC4(Z))) +
                accu(abs(Z - RC5(Z, board)));
            if(err < TOL)
                return cube2Board(Z);
        }
        X1 = X1/2 + RC1(2*Z-X1)/2;
        X2 = X2/2 + RC2(2*Z-X2)/2;
        X3 = X3/2 + RC3(2*Z-X3)/2;
        X4 = X4/2 + RC4(2*Z-X4)/2;
        X5 = X5/2 + RC5(2*Z-X5,board)/2;
    }
    return cube2Board(Z);
}