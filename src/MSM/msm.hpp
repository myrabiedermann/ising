#pragma once

/*
 * class MSM
 */

#include "../global.hpp"

#include "../input/parameters.hpp"
#include "../MC.hpp"
#include "../system/spinsys.hpp"
#include "Cextractor.hpp"
#include "Testimator.hpp"



class msm
{
    typedef std::vector<int> TRAJECTORY;

    std::vector<TRAJECTORY> trajs;

    std::vector<int> possibleStates;

    Eigen::VectorXd Hamiltonians;
    Eigen::MatrixXi countMatrix;
    Eigen::MatrixXd transitionMatrix;
    Eigen::VectorXd stationaryDistribution;
    Eigen::VectorXd entropies;


public:
    msm();

    void create_data();
    void read_data(const PATH&);

    void extract_countMatrix();
    void estimate_transitionMatrix();

    void calc_entropies();
    void save_data(const PATH&);

    void do_CK (const PATH&);
    void do_ITS(const PATH&);
    void do_EV (const PATH&);


};
