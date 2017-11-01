#include "montecarlohost.hpp"



MonteCarloHost::MonteCarloHost()
{}



MonteCarloHost::~MonteCarloHost()
{
    delete acceptance;
}



void MonteCarloHost::setAcceptance(AcceptanceAdaptor* acceptanceAdaptorDerived)
{
    acceptance = acceptanceAdaptorDerived;
}



void MonteCarloHost::setParameters(ParametersWidget* prms)
{
    Q_CHECK_PTR(prms);
    parameters = prms;
    Q_CHECK_PTR(parameters);
}


void MonteCarloHost::setup()
{
    qDebug() << __PRETTY_FUNCTION__ << '\n';
    
    Q_CHECK_PTR(parameters);
    
    spinsystem.setParameters(parameters);
    spinsystem.setup();
    
    trajectory.clear();
    trajectory.push_back(spinsystem.getHamiltonian());

    magnetisations.clear();
    susceptibilities.clear();
    correlations.clear();
    
    qDebug() << "initial: H = " << spinsystem.getHamiltonian();
    qDebug() << spinsystem.c_str();

}




void MonteCarloHost::run(const unsigned long& steps, const bool& EQUIL)
{
    Q_CHECK_PTR(parameters);
    assert(acceptance);
    
    double energy_old;
    double energy_new;
    
    
    for(unsigned int t=0; t<steps; ++t)   
    {
        // flip spin:
        energy_old = spinsystem.getHamiltonian();
        spinsystem.flip();
        energy_new = spinsystem.getHamiltonian();
        
        // check metropolis criterion:
        if( ! acceptance->valid(energy_old, energy_new, parameters->getTemperature()) )
        {
            spinsystem.flip_back(); 
            #ifndef QT_NO_DEBUG 
            qDebug() << "random = " << acceptance->latestRandomNumber() << " >= " << acceptance->latestConditionValue() << " = exp(-(energy_new-energy_old)/temperature)";
            qDebug() << "new H would have been: " << energy_new << "\n";
        }
        else
        {
            qDebug() << acceptance->latestRandomNumber() << " < " << acceptance->latestConditionValue();
            qDebug() << "new H: " << energy_new;
            qDebug() << spinsystem.c_str();
            #endif
        }
        
        if( !EQUIL )
        {
            // save to trajectory
            if( t % parameters->getPrintFreq() == 0 ) 
            {
                trajectory.push_back(spinsystem.getHamiltonian());
            }
            // save correlation
            if( t % parameters->getCorrFreq() == 0 )
            {
                correlations.push_back(spinsystem.getCorrelation());
            }
            // save data
            if( t % parameters->getSampleFreq() == 0 )
            {
                magnetisations.push_back(spinsystem.getMagnetisation());
                susceptibilities.push_back(spinsystem.getSusceptibility());
            }
        }
    }

}


void MonteCarloHost::print_trajectory()
{
    Q_CHECK_PTR(parameters);
    
    std::ofstream STREAM("ising.trajectory");
    STREAM << std::setw(10) << "# time" << std::setw(6) << "Hamiltonian\n";
    for(unsigned int t=0; t<trajectory.size(); ++t)
    {
        STREAM << std::setw(10) << t*parameters->getPrintFreq() << std::setw(6)  << trajectory[t] << '\n';
    }
    STREAM.close();
}


void MonteCarloHost::print_correlation()
{
    Q_CHECK_PTR(parameters);
    
    std::stringstream filename;
    filename << "ising.correlation-" << (correlations.size()-1)*parameters->getPrintFreq();
    std::ofstream FILE(filename.str());
    FILE << correlations.back().formatted_string();
    FILE.close();
}


void MonteCarloHost::print_data()
{
    // save to file: J temperature B <M> <chi>

    Q_CHECK_PTR(parameters);

    std::ofstream FILE;
    if( ! enhance::fileExists("ising.data") )
    {
        // print header line
        FILE.open("ising.data");
        FILE << std::setw(8) << "# J"
             << std::setw(8) << "T"
             << std::setw(8) << "B"
             << std::setw(8) << "<M>"
             << std::setw(8) << "<chi>"
             << '\n';
    }
    else
    {
        FILE.open("ising.data", std::ios::app);
    }

    FILE << std::setw(8) << parameters->getInteraction()
         << std::setw(8) << parameters->getTemperature()
         << std::setw(8) << parameters->getMagnetic()
         << std::setw(8) << std::accumulate(std::begin(magnetisations), std::end(magnetisations), 0) / magnetisations.size()
         << std::setw(8) << std::accumulate(std::begin(susceptibilities), std::end(susceptibilities), 0) / susceptibilities.size()
         << '\n';

    FILE.close();
}



auto MonteCarloHost::getTrajectory() const -> const decltype(MonteCarloHost::trajectory)&
{
    return trajectory;
}


auto MonteCarloHost::getCorrelations() const -> const decltype(MonteCarloHost::correlations)&
{
    return correlations;
}




const Spinsystem& MonteCarloHost::getSpinsystem() const
{
    return spinsystem;
}

