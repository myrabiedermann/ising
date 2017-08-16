#pragma once

/*
 * struct Testimator
 */

#include "../global.hpp"


struct Testimator
{
  Testimator(Eigen::MatrixXi&);

  auto get_T()  const { return T; }
  auto get_pi() const { return pi; }

  void maximumLikelihood();


private:
  const Eigen::MatrixXi C;
  const Eigen::VectorXi Ci;

  Eigen::MatrixXd T;
  Eigen::VectorXd pi;

  void initial_probabilities();
  bool convergence(const Eigen::VectorXd&, const Eigen::VectorXd&);
  void calc_T();


};
