#include "montecarlohost.hpp"



MonteCarloHost::MonteCarloHost()
{
    qDebug() << __PRETTY_FUNCTION__;
}



MonteCarloHost::~MonteCarloHost()
{
    qDebug() << __PRETTY_FUNCTION__;
}



const Spinsystem& MonteCarloHost::getSpinsystem() const
{
    qDebug() << __PRETTY_FUNCTION__;

    return spinsystem;
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
}



void MonteCarloHost::clearRecords()
{
    qDebug() << __PRETTY_FUNCTION__;

    energies.clear();
    magnetisations.clear();

    spinsystem.resetParameters();
    
}


bool MonteCarloHost::acceptance(const double Eold, const double Enew, const double temperature)
{
    // check acceptance via Metropolis criterion

    #ifndef NDEBUG
        double random = enhance::random_double(0.0, 1.0);
        double condition = std::exp(-(Enew-Eold)/temperature);
        Logger::getInstance().debug_new_line("[mc]", "random = ", random, ", exp(-(energy_new-energy_old)/temperature) = ", condition);
        return random < condition ? true : false;
    #endif

    return enhance::random_double(0.0, 1.0) < std::exp(-(Enew-Eold)/temperature) ? true : false;
}



void MonteCarloHost::run(const unsigned long& steps, const bool EQUILMODE)
{
    qDebug() << __PRETTY_FUNCTION__;

    Q_CHECK_PTR(parameters);
    
    double energy_old;
    double energy_new;
    
    
    for(unsigned int t=0; t<steps; ++t)   
    {
        // flip spin:
        energy_old = spinsystem.getHamiltonian();
        spinsystem.flip();
        energy_new = spinsystem.getHamiltonian();
        
        // check metropolis criterion:
        if( ! acceptance(energy_old, energy_new, parameters->getTemperature()) )
        {
            spinsystem.flip_back(); 
            #ifndef NDEBUG
            Logger::getInstance().debug_new_line("[mc]", "move rejected, new H would have been: ", energy_new);
        }
        else
        {
            Logger::getInstance().debug_new_line("[mc]", "move accepted, new H: ", energy_new);
            Logger::getInstance().debug_new_line(spinsystem.str());
            #endif
        }
    }
    
    if( !EQUILMODE )
    {
        energies.push_back(spinsystem.getHamiltonian());
        magnetisations.push_back(spinsystem.getMagnetisation());
        // magnetisationsSquared.push_back(spinsystem.getMagnetisation()*spinsystem.getMagnetisation());
        // if( parameters->getConstrained() ) 
            // amplitudes.push_back(spinsystem.computeStructureFunction());
    }
}



void MonteCarloHost::print_data() const
{
    // save to file:  step  J  T  B  H  M  

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
    for(unsigned int i=0; i<energies.size(); ++i)
    {
        FILE << std::setw(14) << std::fixed << std::setprecision(0)<< (i+1)*parameters->getPrintFreq()
             << std::setw(8) << std::fixed << std::setprecision(2)<< parameters->getInteraction()
             << std::setw(8) << std::fixed << std::setprecision(2)<< parameters->getTemperature()
             << std::setw(8) << std::fixed << std::setprecision(2)<< parameters->getMagnetic()
             << std::setw(14) << std::fixed << std::setprecision(2) << energies[i]
             << std::setw(14) << std::fixed << std::setprecision(6) << magnetisations[i];
        FILE << '\n';
    }
    
    FILE.close();
}



void MonteCarloHost::print_correlation(Histogram<double>& correlation) const
{
    // compute correlations of actual state and save to file

    qDebug() << __PRETTY_FUNCTION__;

    Q_CHECK_PTR(parameters);
    std::string filekeystring = parameters->getFileKey();
    std::string filekey = filekeystring.substr( 0, filekeystring.find_first_of(" ") );
    filekey.append(".correlation");

    std::ofstream FILE(filekey);
    FILE << "# correlation <Si Sj>(r)\n";
    FILE << correlation.formatted_string();
    FILE.close();
}



void MonteCarloHost::print_averages() const
{
    // compute averages and save to file: <energy>  <magnetisation>  <susceptibility>  <heat capacity>

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
             << std::setw(14) << "<M>"
             << std::setw(18) << "<chi>"
             << std::setw(18) << "<Cv>"
             << std::setw(14) << "# of samples"
             << '\n';
    }
    else
    {
        FILE.open(filekey, std::ios::app);
    }
    double averageEnergies = std::accumulate(std::begin(energies), std::end(energies), 0.0) / energies.size();
    double averageEnergiesSquared = std::accumulate(std::begin(energies), std::end(energies), 0.0, [](auto lhs, auto rhs){ return lhs + rhs*rhs; }) / energies.size();
    double averageMagnetisations = std::accumulate(std::begin(magnetisations), std::end(magnetisations), 0.0) / magnetisations.size();
    double averageMagnetisationsSquared = std::accumulate(std::begin(magnetisations), std::end(magnetisations), 0.0, [](auto lhs, auto rhs){ return lhs + rhs*rhs; }) / magnetisations.size();
    double denominator = std::pow(parameters->getTemperature(),2) * std::pow(parameters->getWidth()*parameters->getHeight(),2);
    
    FILE << std::setw(8) << std::fixed << std::setprecision(2) << parameters->getInteraction()
         << std::setw(8) << std::fixed << std::setprecision(2) << parameters->getTemperature()
         << std::setw(8) << std::fixed << std::setprecision(2) << parameters->getMagnetic()
         << std::setw(14) << std::fixed << std::setprecision(2) << averageEnergies
         << std::setw(14) << std::fixed << std::setprecision(6) << averageMagnetisations
         << std::setw(18) << std::fixed << std::setprecision(10) << (averageMagnetisationsSquared - averageMagnetisations*averageMagnetisations) / parameters->getTemperature()
         << std::setw(18) << std::fixed << std::setprecision(10) << (averageEnergiesSquared - averageEnergies*averageEnergies) / denominator
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




