#ifndef _ALTPROJ_HPP
#define _ALTPROJ_HPP
#include <armadillo>
#include "solver.hpp"

#define TOL 1e-7
#define MAXITS 1e4

arma::cube RC1(const arma::cube &Q);
arma::cube RC2(const arma::cube &Q);
arma::cube RC3(const arma::cube &Q);
arma::cube RC4(const arma::cube &Q);
arma::cube RC5(const arma::cube &Q,  Board &board);
Board cube2Board(const arma::cube &Q);
Board DR(Board &board);

#endif