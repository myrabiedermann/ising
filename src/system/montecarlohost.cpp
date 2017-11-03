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
    
    energies.clear();
    magnetisations.clear();
    susceptibilities.clear();
    
    qDebug() << "initial: H = " << spinsystem.getHamiltonian();
    qDebug() << spinsystem.c_str();

}



void MonteCarloHost::randomiseSystem()
{
    qDebug() << __PRETTY_FUNCTION__ << '\n';

    Q_CHECK_PTR(parameters);

    spinsystem.randomise();

    energies.clear();
    magnetisations.clear();
    susceptibilities.clear();

    qDebug() << "initial: H =" << spinsystem.getHamiltonian();
    qDebug() << spinsystem.c_str();
}



void MonteCarloHost::run(const unsigned long& steps, const bool EQUILMODE)
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
    }
    
    if( !EQUILMODE )
    {
        energies.push_back(spinsystem.getHamiltonian());
        magnetisations.push_back(spinsystem.getMagnetisation());
        susceptibilities.push_back(spinsystem.getSusceptibility());
    }
}



void MonteCarloHost::print_data()
{
    // save to file:  step  J  temperature  B  H  M  chi
    
    Q_CHECK_PTR(parameters);
    
    std::ofstream FILE;
    if( ! enhance::fileExists("ising.data") )
    {
        // print header line
        FILE.open("ising.data");
        FILE << std::setw(8) << "# step"
        << std::setw(8) << "J"
        << std::setw(8) << "T"
        << std::setw(8) << "B"
        << std::setw(8) << "H"
        << std::setw(8) << "M"
        << std::setw(8) << "chi"
        << '\n';
    }
    else
    {
        FILE.open("ising.data", std::ios::app);
    }
    
    for(unsigned int i=0; i<energies.size(); ++i)
    {
        FILE << std::setw(8) << i*parameters->getPrintFreq()
             << std::setw(8) << parameters->getInteraction()
             << std::setw(8) << parameters->getTemperature()
             << std::setw(8) << parameters->getMagnetic()
             << std::setw(8) << energies[i]
             << std::setw(8) << magnetisations[i]
             << std::setw(8) << susceptibilities[i];
        
        FILE << '\n';
    }
    
    FILE.close();
}



void MonteCarloHost::print_correlation()
{
    // compute correlations of actual state and save to file

    Q_CHECK_PTR(parameters);
    
    std::ofstream FILE("ising.correlation");
    FILE << "# correlation <Si Sj>(r)\n";
    FILE << spinsystem.getCorrelation().formatted_string();
    FILE.close();
}



void MonteCarloHost::print_averages()
{
    // compute averages and save to file: <energy>  <magnetisation>  <susceptibility>

    Q_CHECK_PTR(parameters);

    std::ofstream FILE;
    if( ! enhance::fileExists("ising.averaged_data") )
    {
        // print header line
        FILE.open("ising.averaged_data");
        FILE << std::setw(10) << "J"
             << std::setw(10) << "T"
             << std::setw(10) << "B"
             << std::setw(12) << "<H>"
             << std::setw(12) << "<M>"
             << std::setw(12) << "<chi>"
             << std::setw(12) << "# of samples"
             << '\n';
    }
    else
    {
        FILE.open("ising.averaged_data", std::ios::app);
    }
    
    FILE << std::setw(10) << parameters->getInteraction()
         << std::setw(10) << parameters->getTemperature()
         << std::setw(10) << parameters->getMagnetic()
         << std::setw(12) << std::fixed << std::setprecision(6) << std::accumulate(std::begin(energies), std::end(energies), 0.0) / energies.size()
         << std::setw(12) << std::fixed << std::setprecision(6) << std::accumulate(std::begin(magnetisations), std::end(magnetisations), 0.0) / magnetisations.size()
         << std::setw(12) << std::fixed << std::setprecision(6) << std::accumulate(std::begin(susceptibilities), std::end(susceptibilities), 0.0) / susceptibilities.size()
         << std::setw(12) << ( energies.size() == magnetisations.size() and energies.size() == susceptibilities.size() ? energies.size() : -1 )
         << '\n';
    
    FILE.close();
}



const Spinsystem& MonteCarloHost::getSpinsystem() const
{
    return spinsystem;
}

