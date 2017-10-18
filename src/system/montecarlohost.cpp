#include "montecarlohost.hpp"



MonteCarloHost::MonteCarloHost()
{
    
}



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
    Q_CHECK_PTR(parameters);
    
    spinsystem.setParameters(parameters);
    spinsystem.setup();
    
    trajectory.clear();
    trajectory.push_back(spinsystem.getHamiltonian());
    
    qDebug() << "initial: H = " << spinsystem.getHamiltonian();
    qDebug() << spinsystem.c_str();
}




void MonteCarloHost::run(const unsigned long& steps)
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
        
        
        // save to trajectory
        if( t % parameters->getPrintFreq() == 0 ) 
        {
            trajectory.push_back(spinsystem.getHamiltonian());
        }
    }
}


void MonteCarloHost::print_trajectory()
{
    Q_CHECK_PTR(parameters);
    
    std::ofstream STREAM("trajectory");
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
    
    std::ofstream FILE("correlation");
    FILE << std::setw(10) << "# time" << std::setw(10) << "< s_i s_j >\n";
    for(unsigned int t=0; t<correlation.size(); ++t)
    {
        FILE << std::setw(10) << t*parameters->getPrintFreq() << std::setw(10) << correlation[t] << '\n';
    }
    FILE.close();
}



auto MonteCarloHost::getTrajectory() const -> const decltype(MonteCarloHost::trajectory)&
{
    return trajectory;
}


auto MonteCarloHost::getCorrelation() const -> const decltype(MonteCarloHost::correlation)&
{
    return correlation;
}




const Spinsystem& MonteCarloHost::getSpinsystem() const
{
    return spinsystem;
}

