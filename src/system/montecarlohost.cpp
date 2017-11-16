#include "montecarlohost.hpp"



MonteCarloHost::MonteCarloHost()
{
    qDebug() << __PRETTY_FUNCTION__;
}



MonteCarloHost::~MonteCarloHost()
{
    qDebug() << __PRETTY_FUNCTION__;

    delete acceptance;
}



void MonteCarloHost::setAcceptance(AcceptanceAdaptor* acceptanceAdaptorDerived)
{
    qDebug() << __PRETTY_FUNCTION__;

    acceptance = acceptanceAdaptorDerived;
}



void MonteCarloHost::setParameters(BaseParametersWidget* prms)
{
    qDebug() << __PRETTY_FUNCTION__;

    Q_CHECK_PTR(prms);
    parameters = prms;
    Q_CHECK_PTR(parameters);
}



void MonteCarloHost::setup()
{
    qDebug() << __PRETTY_FUNCTION__;
    
    Q_CHECK_PTR(parameters);
    
    spinsystem.setParameters(parameters);
    spinsystem.setup();
    
    clearRecords();
    
    Logger::getInstance().debug("[mc]", "initial: H = ", spinsystem.getHamiltonian());
    Logger::getInstance().debug(spinsystem.c_str());

}



void MonteCarloHost::resetSpins()
{
    qDebug() << __PRETTY_FUNCTION__;

    Q_CHECK_PTR(parameters);
    
    if( parameters->getWavelengthPattern() )
    {
        spinsystem.resetSpinsCosinus(parameters->getWavelength());
    }
    else
    {
        spinsystem.resetSpins();
    }
    
    Logger::getInstance().debug("[mc] initial: H =", spinsystem.getHamiltonian());
    Logger::getInstance().debug(spinsystem.c_str());
}



void MonteCarloHost::clearRecords()
{
    qDebug() << __PRETTY_FUNCTION__;

    energies.clear();
    energiesSquared.clear();
    magnetisations.clear();
    magnetisationsSquared.clear();

    spinsystem.resetParameters();
    
}



void MonteCarloHost::run(const unsigned long& steps, const bool EQUILMODE)
{
    qDebug() << __PRETTY_FUNCTION__;

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
            Logger::getInstance().debug("[mc]", "random = ", acceptance->latestRandomNumber()," >= ", acceptance->latestConditionValue(), " = exp(-(energy_new-energy_old)/temperature)");
            Logger::getInstance().debug("[mc]", "new H would have been: ", energy_new);
        }
        else
        {
            Logger::getInstance().debug("[mc]", acceptance->latestRandomNumber(), " < ", acceptance->latestConditionValue());
            Logger::getInstance().debug("[mc]", "new H: ", energy_new);
            Logger::getInstance().debug(spinsystem.c_str());
            #endif
        }
    }
    
    if( !EQUILMODE )
    {
        energies.push_back(spinsystem.getHamiltonian());
        energiesSquared.push_back(spinsystem.getHamiltonian()*spinsystem.getHamiltonian());
        magnetisations.push_back(spinsystem.getMagnetisation());
        magnetisationsSquared.push_back(spinsystem.getMagnetisation()*spinsystem.getMagnetisation());
        if( parameters->getConstrained() ) 
            amplitudes.push_back(spinsystem.computeAmplitudes());
    }
}



void MonteCarloHost::print_data() const
{
    // save to file:  step  J  temperature  B  H  M  chi

    qDebug() << __PRETTY_FUNCTION__;
    
    Q_CHECK_PTR(parameters);
    std::string filekeystring = parameters->getFileKey();
    std::string filekey = filekeystring.substr( 0, filekeystring.find_first_of(" ") );
    filekey.append(".data");

    std::ofstream FILE;
    FILE.open(filekey);
    
    // print header line
    FILE << std::setw(14) << "# step"
    << std::setw(8) << "J"
    << std::setw(8) << "T"
    << std::setw(8) << "B"
    << std::setw(14) << "H"
    << std::setw(14) << "M"
    << '\n';
    
    assert(energies.size() == magnetisations.size());
    assert(energies.size() == energiesSquared.size());
    assert(magnetisations.size() == magnetisationsSquared.size());
    for(unsigned int i=0; i<energies.size(); ++i)
    {
        FILE << std::setw(14) << std::fixed << std::setprecision(0)<< (i+1)*parameters->getPrintFreq()
             << std::setw(8) << std::fixed << std::setprecision(1)<< parameters->getInteraction()
             << std::setw(8) << std::fixed << std::setprecision(2)<< parameters->getTemperature()
             << std::setw(8) << std::fixed << std::setprecision(1)<< parameters->getMagnetic()
             << std::setw(14) << std::fixed << std::setprecision(2) << energies[i]
             << std::setw(14) << std::fixed << std::setprecision(6) << magnetisations[i];
        FILE << '\n';
    }
    
    FILE.close();
}



void MonteCarloHost::print_correlation() const
{
    // compute correlations of actual state and save to file

    qDebug() << __PRETTY_FUNCTION__;

    Q_CHECK_PTR(parameters);
    std::string filekeystring = parameters->getFileKey();
    std::string filekey = filekeystring.substr( 0, filekeystring.find_first_of(" ") );
    filekey.append(".correlation");

    std::ofstream FILE(filekey);
    FILE << "# correlation <Si Sj>(r)\n";
    FILE << spinsystem.getCorrelation().formatted_string();
    FILE.close();
}



void MonteCarloHost::print_averages() const
{
    // compute averages and save to file: <energy>  <magnetisation>  <susceptibility>

    qDebug() << __PRETTY_FUNCTION__;

    Q_CHECK_PTR(parameters);
    std::string filekeystring = parameters->getFileKey();
    std::string filekey = filekeystring.substr( 0, filekeystring.find_first_of(" ") );
    filekey.append(".averaged_data");

    std::ofstream FILE;
    if( ! enhance::fileExists(filekey) )
    {
        // print header line
        FILE.open(filekey);
        FILE << std::setw(8) << "J"
             << std::setw(8) << "T"
             << std::setw(8) << "B"
             << std::setw(14) << "<H>"
             << std::setw(14) << "<H^2>"
             << std::setw(14) << "<M>"
             << std::setw(14) << "<M^2>"
             << std::setw(14) << "<chi>"
             << std::setw(14) << "<Cv>"
             << std::setw(14) << "# of samples"
             << '\n';
    }
    else
    {
        FILE.open(filekey, std::ios::app);
    }
    double averageEnergies = std::accumulate(std::begin(energies), std::end(energies), 0.0) / energies.size();
    double averageEnergiesSquared = std::accumulate(std::begin(energiesSquared), std::end(energiesSquared), 0.0) / energiesSquared.size();
    double averageMagnetisations = std::accumulate(std::begin(magnetisations), std::end(magnetisations), 0.0) / magnetisations.size();
    double averageMagnetisationsSquared = std::accumulate(std::begin(magnetisationsSquared), std::end(magnetisationsSquared), 0.0) / magnetisationsSquared.size();
    double denominator = std::pow(parameters->getTemperature(),2) * std::pow(parameters->getWidth()*parameters->getHeight(),2);
    
    FILE << std::setw(8) << std::fixed << std::setprecision(1) << parameters->getInteraction()
         << std::setw(8) << std::fixed << std::setprecision(1) << parameters->getTemperature()
         << std::setw(8) << std::fixed << std::setprecision(1) << parameters->getMagnetic()
         << std::setw(14) << std::fixed << std::setprecision(2) << averageEnergies
         << std::setw(14) << std::fixed << std::setprecision(2) << averageEnergiesSquared
         << std::setw(14) << std::fixed << std::setprecision(6) << averageMagnetisations
         << std::setw(14) << std::fixed << std::setprecision(6) << averageMagnetisationsSquared
         << std::setw(14) << std::fixed << std::setprecision(6) << (averageMagnetisationsSquared - averageMagnetisations*averageMagnetisations) / parameters->getTemperature()
         << std::setw(14) << std::fixed << std::setprecision(8) << (averageEnergiesSquared - averageEnergies*averageEnergies) / denominator
         << std::setw(14) << energies.size() 
         << '\n';
    
    FILE.close();
}



void MonteCarloHost::print_amplitudes() const
{
    qDebug() << __PRETTY_FUNCTION__;
    
    Q_CHECK_PTR(parameters);
    std::string filekeystring = parameters->getFileKey();
    std::string filekey = filekeystring.substr( 0, filekeystring.find_first_of(" ") );
    filekey.append(".amplitudes");

    std::ofstream FILE;
    FILE.open(filekey);
    
    // print header line
    FILE << std::setw(4) << "# k"
    << std::setw(10) << "MC steps:";

    for(unsigned int i=0; i<amplitudes.size(); ++i) FILE << std::setw(8) << i*parameters->getPrintFreq();
    FILE << '\n';
    
    for( unsigned int j=0; j<amplitudes.front().size(); ++j )
    {
        FILE << std::setw(14) << std::fixed << j; 
        for(unsigned int i=0; i<amplitudes.size(); ++i)
        {
            FILE << std::setw(8) << std::setprecision(2) << amplitudes[i][j];
        }
        FILE << '\n';
    }
    
    FILE.close();
}



const Spinsystem& MonteCarloHost::getSpinsystem() const
{
    qDebug() << __PRETTY_FUNCTION__;

    return spinsystem;
}

