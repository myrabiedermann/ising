#include "msm.hpp"

/*
 * functions in class msm
 */


msm::msm()
{
    // get possible H/J values for ergodicity check:
    spinsys spinsystem(prms::systemsize, prms::J, prms::CONSTRAINED);
    possibleStates = spinsystem.possibleHwithoutJ();
}

/***************************************************************************/

void msm::create_data()
{
    // do MC simulations to get data for Markov state modelling

    std::cout << "\n--> creating MC data for Markov state modelling ...\n";

    PATH dir_path(prms::initialPath);
    dir_path /= prms::output_key+"-trjs";
    if( prms::SAVETRJ )
    {
        if( boost::filesystem::exists(dir_path) and boost::filesystem::is_directory(dir_path) )
        {
            boost::filesystem::remove_all(dir_path);
#ifndef NDEBUG
            std::cout << "WARNING: removed all contents in trajectory " << dir_path << "\n";
#endif
        }
        boost::filesystem::create_directory(dir_path);
    }

    unsigned int nr_runs = prms::msm_runs * (prms::msm_scaling + 1) / 2;

    for(unsigned int R = 0; R<nr_runs; ++R){
        unsigned int nr_mcsteps;
        if( R < prms::msm_runs/2 ) 	nr_mcsteps = prms::msm_maxMCsteps;
        else 				                nr_mcsteps = prms::msm_maxMCsteps / prms::msm_scaling;

        mc MC;
        MC.do_metropolis(nr_mcsteps, 1);
        trajs.push_back( MC.get_HwithoutJ_trj() );

        // save trj:
        if( prms::SAVETRJ ){
            boost::filesystem::path outpath(dir_path);
            outpath /= std::to_string(R) + ".trj";
            MC.save_trj( outpath );
        }
    }

    std::cout << "   ... done\n";

}

/***************************************************************************/

void msm::read_data(const PATH& _directory_path)
{
    // read MC data from files

    SIZE stepper = 0;
    PATH filepath = _directory_path;
    filepath /= "0.trj";
    while( boost::filesystem::exists(filepath) )
    {
        // create new trajectory:
        trajs.resize(trajs.size() + 1);

        // read file:
        IFSTREAM FILE;
        FILE.open(filepath);
        while( FILE.good() )
        {
            std::string line;
            std::getline(FILE, line, '\n');
            std::stringstream linestream(line);
            if( linestream.peek() != '#')
            {
                SIZE timestamp;
                SIZE orderparam;
                linestream >> timestamp >> orderparam;
                trajs.back().emplace_back(orderparam);
            }
        }

        // adjust filepath:
        ++ stepper;
        filepath = _directory_path;
        std::stringstream newfile;
        newfile << stepper << ".trj";
        filepath /= newfile.str();
    }

}

/***************************************************************************/

void msm::extract_countMatrix()
{
    // extract count Matrix from trajectory data

    Cextractor Cgetter;
    Cgetter.extract( trajs, prms::msm_tau, possibleStates );
    countMatrix = Cgetter.get_C();
    Hamiltonians = prms::J * Cgetter.get_energies();

#ifndef NDEBUG
    std::cout << "count matrix:\n";
    std::cout << countMatrix << "\n";
    std::cout << "for H/J values:\n";
    std::cout << Cgetter.get_energies() << "\n";
#endif

}

/***************************************************************************/

void msm::estimate_transitionMatrix()
{
    // estimate transition probability matrix from count matrix

    Testimator Tgetter(countMatrix);
    Tgetter.maximumLikelihood();
    stationaryDistribution = Tgetter.get_pi();
    transitionMatrix = Tgetter.get_T();

#ifndef NDEBUG
    std::cout << "transition matrix:\n";
    std::cout << transitionMatrix << "\n";
    std::cout << "final probabilities:\n ";
    std::cout << stationaryDistribution << "\n";
#endif
}

/***************************************************************************/

void msm::calc_entropies()
{
    // calc entropies from stationary distribution : S(N) = log(pi(N)) + H(N)

    entropies.resize(stationaryDistribution.rows());
    entropies = stationaryDistribution.array().log();
    entropies += Hamiltonians;

#ifndef NDEBUG
    std::cout << "Hamiltonians:\n";
    std::cout << Hamiltonians << "\n";
    std::cout << "entropies:\n";
    std::cout << entropies << "\n";
#endif
}

/***************************************************************************/

void msm::save_data(const PATH& _outpath)
{
    // save estimated data (stationary distribution, entropies) to _outpath

    OFSTREAM FILE;
    FILE.open(_outpath);
    // header line:
    FILE << std::setw(8) << "# H"
         << std::setw(14) << "p"
         << std::setw(14) << "S" << "\n";
    // data lines:
    for(unsigned int i = 0; i < Hamiltonians.rows(); ++i){
        FILE << std::setw(8) << Hamiltonians(i)
             << std::setw(14) << stationaryDistribution(i)
             << std::setw(14) << entropies(i)
             << "\n";
    }

    FILE.close();
}

/***************************************************************************/

void msm::do_CK(const PATH& _outpath)
{
    // perform a Chapman-Kolmogorov test

    Eigen::MatrixXd data_MSM;
    Eigen::MatrixXd data_MC;

    for( unsigned int k = 1; k <= 10; ++ k )
    {
        // extract count Matrix
        Cextractor Cgetter;
        Cgetter.extract(trajs, k*prms::msm_tau, possibleStates);
        auto Cmatrix = Cgetter.get_C();
        auto states = Cgetter.get_translator();

        // estimate MSM
        Testimator Tgetter(Cmatrix);
        Tgetter.maximumLikelihood();
        auto Tmatrix = Tgetter.get_T();

        // resize matrices
        data_MSM.conservativeResizeLike( Eigen::MatrixXd::Zero(data_MSM.rows() + 1, prms::msm_cktransitions.size() + 1) );
        data_MC.conservativeResizeLike( Eigen::MatrixXd::Zero(data_MC.rows() + 1, 2*prms::msm_cktransitions.size()+1) );
        int current_row = data_MSM.rows() - 1;

        data_MSM(current_row, 0) = k*prms::msm_tau;
        data_MC(current_row, 0)  = k*prms::msm_tau;

        // get probabilities and error for all transitions
        for(unsigned int i = 0; i < prms::msm_cktransitions.size(); ++i) {

            // find position of trans.first and trans.second in states:
            int index1, index2;

            auto pos1 = std::find( std::begin(states), std::end(states), prms::msm_cktransitions[i].first );
            if (pos1 != std::end(states)) {
                index1 = std::distance( std::begin(states), pos1 );
            } else {
                continue;
            }

            auto pos2 = std::find( std::begin(states), std::end(states), prms::msm_cktransitions[i].second );
            if (pos2 != std::end(states)) {
                index2 = std::distance( std::begin(states), pos2 );
            } else {
                continue;
            }

            // save probabilities etc:
            double Ci = static_cast<double>(Cmatrix.row(index1).sum());
            double pdata = static_cast<double>(Cmatrix(index1, index2)) / Ci;
            data_MSM(current_row, i+1)      = Tmatrix(index1, index2);
            data_MC(current_row, 2*(i+1)-1) = pdata;
            data_MC(current_row, 2*(i+1))   = std::sqrt( static_cast<double>(k) * (pdata-std::pow(pdata,2)) / Ci );
        }

    }

    // save data to files
    OFSTREAM FILE;
    PATH originalpath = _outpath;
    PATH filepath = originalpath.replace_extension("");
    filepath += "-transitions.data";
    FILE.open(filepath);
    FILE << "# requested transitions for Chapman Kolmogorov test\n";
    for(const auto& T: prms::msm_cktransitions) FILE << T.first << "-" << T.second << "\t";
    FILE << "\n";
    FILE.close();

    filepath = originalpath.replace_extension("");
    filepath += "-msm.data";
    FILE.open(filepath);
    FILE << "# MSM probabilities in Chapman Kolmogorov test for requested transitions, first col: k*tau \n";
    FILE << std::setw(12) << "#";
    for(const auto& T: prms::msm_cktransitions) FILE << T.first << "-" << T.second << std::setw(6) << " ";
    FILE << "\n";
    FILE << data_MSM;
    FILE << "\n";
    FILE.close();

    filepath = originalpath.replace_extension("");
    filepath += "-mc.data";
    FILE.open(filepath);
    FILE << "# MC probabilities and respective errors in Chapman Kolmogorov test for requested transitions, first col: k*tau \n";
    FILE << std::setw(13) << "#";
    for(const auto& T: prms::msm_cktransitions) FILE << T.first << "-" << T.second << std::setw(20) << " ";
    FILE << "\n";
    FILE << data_MC;
    FILE << "\n";
    FILE.close();
}

/***************************************************************************/

void msm::do_ITS(const PATH& _outpath)
{
    // compute implied timescales

    Eigen::MatrixXd allITS;
    Eigen::MatrixXi Cmatrix;
    Eigen::MatrixXd Tmatrix;
    Eigen::VectorXd eigenvalues;
    Eigen::VectorXd timescales;

    for(unsigned int tau = 1; tau < 100; ++tau)
    {
        // get possible H/J values for ergodicity check:
        spinsys spinsystem(prms::systemsize, prms::J, prms::CONSTRAINED);

        // extract count Matrix
        Cextractor Cgetter;
        Cgetter.extract( trajs, tau , spinsystem.possibleHwithoutJ() );
        Cmatrix = Cgetter.get_C();

        // estimate MSM
        Testimator Tgetter(Cmatrix);
        Tgetter.maximumLikelihood();
        Tmatrix = Tgetter.get_T();

        // get eigenvalues
        eigenvalues = Tmatrix.eigenvalues().real();
        std::sort(eigenvalues.data(), eigenvalues.data()+eigenvalues.rows(),
            [](const auto& a, const auto& b){ return b < a; });

        // get implied timescales
        timescales.resize(eigenvalues.rows() - 1);  // no its for t1!
        timescales = eigenvalues.segment(1, eigenvalues.rows()-1).array().log();
        timescales = timescales.array().inverse();
        timescales *= -1.0*tau;

        // save timplied timescales
        if( allITS.cols() == 0 )
        {
            allITS.conservativeResize(timescales.rows(), 1);
            allITS.setZero();
            allITS.col(0) = timescales;
        }
        else{
            allITS.conservativeResizeLike( Eigen::MatrixXd::Zero(allITS.rows(), allITS.cols() + 1) );
            allITS.topRightCorner(timescales.rows(), 1) = timescales;
        }
    }

    // save to file
    OFSTREAM FILE;
    FILE.open(_outpath);
    // header line:
    FILE << "# implied timescales in range tau = 1 ... 99 \n";
    FILE << allITS;
    FILE << "\n";
    FILE.close();
}

/***************************************************************************/

void msm::do_EV(const PATH& _outpath)
{
    // compute eigenvalues and eigenvectors of T

    std::cout << __PRETTY_FUNCTION__ << " : computation of EV & EVecs not yet implemented \n";

}
