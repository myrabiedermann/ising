#pragma once

/*
 * count matrix extractor
 */

#include "../global.hpp"

struct Cextractor
{
  typedef std::vector<SIGNED> TRAJECTORY;

  void extract(const std::vector<TRAJECTORY>&, const SIZE&, const std::vector<SIGNED>&);

  auto& get_C()        const { return countMatrix_ergodic; }
  auto& get_energies() const { return energies; }
  auto& get_translator() const { return translator_ergodic; }

private:
  Eigen::MatrixXi     countMatrix;  // access elements via (row, col)
  Eigen::MatrixXi     countMatrix_sorted;
  Eigen::MatrixXi     countMatrix_ergodic;
  std::vector<SIGNED> translator;   // contains values to translate indices of countMatrix (= indices in this vector) to their respective order parameter (=entry in trajectory --> energy / order parameter)
  std::vector<SIGNED> translator_sorted;
  std::vector<SIGNED> translator_ergodic;
  Eigen::VectorXd     energies;
  std::vector<SIGNED> possibleOrders;

  void sortC();
  void checkErgodicity();

};
