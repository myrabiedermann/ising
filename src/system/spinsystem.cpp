#include "spinsystem.hpp"



double Spinsystem::localEnergyInteraction(const Spin& _spin) const
{
    /* Aufgabe 1.2:
     *
     * input:    Ein spezifischer Spin
     * return:   Der Beitrag zum Wechselwirkungsterm im Hamiltonian durch diesen Spin
     * Funktion: Berechnung des return Wertes
     */

    return 0;
}



double Spinsystem::localEnergyMagnetic(const Spin& _spin) const
{
    /* Aufgabe 1.2:
     *
     * input:    Ein spezifischer Spin
     * return:   Der Beitrag zum magnetischen Term im Hamiltonian durch diesen Spin
     * Funktion: Berechnung des return Wertes.
     */

    return 0;
}



void Spinsystem::computeHamiltonian()
{
    /* Aufgabe 1.3:
     *
     * input:    /
     * return:   /
     * Funktion: Berechnung des gesamten Hamiltonian des Systems
     *           bei gegebener Spinkonfiguration und Abspeicherung 
     *           dieses Wertes in der Membervariable "Hamiltonian".
     */

    
}



void Spinsystem::flip()
{
    /* Aufgabe 1.4:
     *
     * input:    /
     * return:   /
     * Funktion: - Im spin-flip Modus:
     *              Auswahl eines zufälligen Spins, Flip dieses Spins, 
     *              Update des Hamiltonian, Abspeichern dieses Spins in
     *              der Membervariable lastFlipped
     *           - Im spin-exchange Modus:
     *              Auswahl eines zufälligen Spins und eines weiteren, 
     *              zufälligen, Nachbarspins mit entgegengesetzer Ausrichtung, 
     *              Flip dieser beiden Spins, Update des Hamiltonian, Abspeichern
     *              der geflippten Spins in der Membervariable lastFlipped
     */


}



void Spinsystem::flip_back()
{
     /* Aufgabe 1.4:
     *
     * input:    /
     * return:   /
     * Funktion: Macht den gesamten in flip() durchgeführten Prozess rückgängig. 
     */

    

}



double Spinsystem::getMagnetisation() const
{
    /* Aufgabe 1.5:
     *
     * input:    /
     * return:   Mittlere Magnetisierung M
     * Funktion: Berechnung der mittleren Magnetisierung M für die aktuelle Spin-
     *           konfiguration.  
     */

    return 0;
}




/*
 * DER HIER FOLGENDE TEIL DER KLASSE IST NICHT RELEVANT FUER 
 * DIE IMPLEMENTIERUNGSAUFGABEN UND KANN IGNORIERT WERDEN 
 */

unsigned long Spinsystem::getHeight() const 
{ 
    Q_CHECK_PTR(parameters); 
    return parameters->getHeight(); 
}


unsigned long Spinsystem::getWidth() const 
{ 
    Q_CHECK_PTR(parameters); 
    return parameters->getWidth(); 
}


double Spinsystem::getInteraction() const 
{ 
    Q_CHECK_PTR(parameters); 
    return parameters->getInteraction(); 
}


double Spinsystem::getMagnetic() const 
{ 
    Q_CHECK_PTR(parameters); 
    return parameters->getMagnetic(); 
}


bool Spinsystem::getSpinExchange() const 
{ 
    Q_CHECK_PTR(parameters); 
    return parameters->getConstrained(); 
}


double Spinsystem::getRatio() const 
{ 
    Q_CHECK_PTR(parameters); 
    return parameters->getRatio(); 
}


bool Spinsystem::getWavelengthPattern() const 
{ 
    Q_CHECK_PTR(parameters); 
    return parameters->getWavelengthPattern(); 
}


int Spinsystem::getWavelength() const 
{ 
    Q_CHECK_PTR(parameters); 
    return parameters->getWavelength(); 
}


void Spinsystem::setParameters(BaseParametersWidget* prms)
{
    qDebug() << __PRETTY_FUNCTION__;

    Q_CHECK_PTR(prms);
    parameters = prms;
    Q_CHECK_PTR(parameters);
}


void Spinsystem::resetParameters()
{
    qDebug() << __PRETTY_FUNCTION__;

    computeHamiltonian();
    Logger::getInstance().debug_new_line("[spinsystem]", "resetting parameters ... new initial H = ", Hamiltonian);
}


void Spinsystem::setup()
{
    // setup of the spinsystem: add all spins, add corresponding neighbours to each spin, set all spintypes randomly

    qDebug() << __PRETTY_FUNCTION__;

    spins.clear();
    lastFlipped.clear();

    // some safety checks:
    if( getSpinExchange() )
    {
        if( parameters->getWidth() % 2 != 0 )
        {
            parameters->setWidth(parameters->getWidth()+1);
            qInfo() << "Remember: system size must be an even number if system is constrained!";
        }
        if( parameters->getHeight() % 2 != 0 )
        {
            parameters->setHeight(parameters->getHeight()+1);
            qInfo() << "Remember: system size must be an even number if system is constrained!";
        }
    }

    auto width  = getWidth();
    auto height = getHeight();
    auto totalnumber = width * height;

    // create spins:
    for(unsigned int i=0; i<totalnumber; ++i)
        spins.emplace_back(i, +1);

    // set neighbours:
    Logger::getInstance().debug_new_line("[spinsystem]", "system setup: setting neighbours for", getWidth(), "*", getHeight(), "system");
    for(auto& s: spins)
    {
        std::vector<std::reference_wrapper<Spin> > Nrefs;
        unsigned int Nid;
        const unsigned int id = s.getID();

        {
            // up
            Nid = ((long)id - static_cast<long>(width)) < 0 ? id - width + totalnumber : id - width;
            assert( Nid < spins.size() );
            if( Nid != id )
                Nrefs.push_back( std::ref(spins[Nid]) );
        }

        {
            // right
            Nid = (id + 1) % width == 0  ? id + 1 - width : id + 1;
            assert( Nid < spins.size() );
            if( Nid != id )
                Nrefs.push_back( std::ref(spins[Nid]) );
        }

        {
            // below
            Nid = id + width >= totalnumber ? id + width - totalnumber : id + width;
            assert( Nid < spins.size() );
            if( Nid != id )
                 Nrefs.push_back( std::ref(spins[Nid]) );
        }

        {
            // left
            Nid = id % width == 0  ? id - 1 + width : id - 1;
            assert( Nid < spins.size() );
            if( Nid != id )
                 Nrefs.push_back( std::ref(spins[Nid]) );
        }

        s.setNeighbours(Nrefs);
        Logger::getInstance().debug_new_line("            ",  "spin", s.getID(), "has neighbours :");
        std::for_each( std::begin(s.getNeighbours()), std::end(s.getNeighbours()), [](auto& N){ Logger::getInstance().debug( N.get().getID()," "); } );
    }
    
    // set spin types:
    if( getWavelengthPattern() )
    {
        resetSpinsCosinus( getWavelength() );
    }
    else
    {
        resetSpins();
    }

}


void Spinsystem::resetSpins() 
{
    // randomly set types of all spins new

    qDebug() << __PRETTY_FUNCTION__;

    int random;
    if( ! getSpinExchange() ) // initialise spins randomly
    {
        for( auto& s: spins )
        {
            random = enhance::random_int(0,1);
            s.setType( random == 1 ? +1 : -1 );
        }
    }      
    else  // constrained to specific up-spin to down-spin ratio
    {
        Logger::getInstance().debug_new_line("[spinsystem]", "ratio =", getRatio(), ", results in", static_cast<unsigned int>(getRatio() * spins.size()), " down spins.");
        for( auto& s: spins ) 
            s.setType( +1 );
        for(unsigned int i=0; i<static_cast<unsigned int>( getRatio() * spins.size()); ++i)
        {
            do
            {
                random = enhance::random_int(0, spins.size()-1);
            }
            while( spins[random].getType() == -1 );
            spins[random].setType(-1);
        }
    }

    // clear / reset all vectors: 
    lastFlipped.clear();
    
    // calculate initial Hamiltonian:
    computeHamiltonian();
    Logger::getInstance().debug_new_line("[spinsystem]", "resetting spins randomly... new initial H =", Hamiltonian);
    Logger::getInstance().debug_new_line(getStringOfSystem());

}


void Spinsystem::resetSpinsCosinus(const double k) 
{
    // set types of all spins new according to c(x) = cos(kx) 

    qDebug() << __PRETTY_FUNCTION__;

    int random;
    
    unsigned int totNrDownSpins = 0;
    for( auto& s: spins ) 
        s.setType( +1 );
    for(unsigned int i = 0; i<getWidth(); ++i)
    {
        double ratio = ((0.5*std::cos(k*(2*M_PI/getWidth())*static_cast<double>(i+0.5)) + 1) / 2);
        unsigned int nrDownSpins = std::round(ratio*getWidth());
        totNrDownSpins += nrDownSpins;
        for(unsigned int j=0; j<nrDownSpins; ++j)
        {
            do
            {
                random = enhance::random_int(i*getWidth(), (i+1)*getWidth() - 1);
            }
            while( spins[random].getType() == -1 );
            spins[random].setType(-1);
        }
    }

    // clear / reset all vectors: 
    lastFlipped.clear();
    
    // calculate initial Hamiltonian:
    computeHamiltonian();
    Logger::getInstance().debug_new_line("[spinsystem]", "resetting spins with cos(", getWavelength(),"y ) pattern ... new initial H =", Hamiltonian);
    Logger::getInstance().debug_new_line("[spinsystem]", "# of down spins:", totNrDownSpins);
    Logger::getInstance().debug_new_line(getStringOfSystem());

}


void Spinsystem::print(std::ostream & stream) const
{
    // print spins to stream

    for( const auto& s: spins )
    {
        stream << ( s.getType() == -1 ? "-" : "+" )
        << ( (static_cast<unsigned int>(s.getID() + 1)) % getWidth() == 0 ? '\n' : ' ');
    }
}


std::string Spinsystem::getStringOfSystem() const
{
    // return string of spinsystem

    std::stringstream sstream;
    sstream << *this;
    
    return sstream.str(); 
}


double Spinsystem::distance(const Spin& _spin1, const Spin& _spin2) const
{
    // compute distance between spins _spin1 and _spin2

    int a, b, c, d, x, y;

    a = _spin1.getID() % getWidth();
    b = _spin1.getID() / getWidth();
    c = _spin2.getID() % getWidth();
    d = _spin2.getID() / getWidth();

    x = (std::abs(c - a) <= static_cast<int>(getWidth()/2) ? std::abs(c - a) : std::abs(c - a) - getWidth());
    y = (std::abs(d - b) <= static_cast<int>(getHeight()/2) ? std::abs(d - b) : std::abs(d - b) - getHeight());

    return  std::sqrt( x*x + y*y );
}


Histogram<double> Spinsystem::computeCorrelation() const
{
    // compute correlation between spins: G(r) = <S(0)S(r)> - <S>^2

    double binWidth = 0.1;
    Histogram<double> correlation {binWidth};
    Histogram<double> counter {binWidth};
    Logger::getInstance().debug_new_line("[spinsystem]", "computing correlation <Si Sj>");

    // first: <S(0)S(r)>:
    double maxDist = ( getWidth() >= getHeight() ? (double) getWidth() : (double) getHeight() )/2  + binWidth/2;
    for( auto& s1 : spins )
    {
        for( auto& s2 : spins )
        {
            if( s1.getID() != s2.getID() )
            {
                double dist = distance( s1,  s2);
                if( dist < maxDist )
                {
                    correlation.add_data( dist, s1.getType() == s2.getType() ? 1 : -1);
                    counter.add_data( dist );
                    Logger::getInstance().debug_new_line("            ", "correlating ", s1.getID(), " with ", s2.getID()," : ", s1.getType() == s2.getType() ? 1 : -1);
                    Logger::getInstance().debug("   distance ", dist);
                }
            }
        }
    }
    // normalisation:
    std::for_each(std::begin(correlation), std::end(correlation), [&](auto& B)
    { 
        B.counter /= counter.get_data(B.position()); 
    });

    // then:  - <S>^2:
    correlation.shift( getMagnetisation()*getMagnetisation() );

    correlation.sort_bins();
    return correlation;
}


Histogram<double> Spinsystem::computeStructureFunction(Histogram<double> correlation) const
{
    // compute Fourier transformation S(k) = integral cos(2PI/width*k*r) G(r) dr, with r = distance between spins

    Logger::getInstance().debug_new_line("[spinsystem]", "computing structure function S(k)");

    // computation of delta r's:
    Histogram<double> deltaR {correlation};
    double previous = 0;
    double next = 0;
    auto it = std::begin(deltaR);
    for( ; it != std::end(deltaR) - 1; it ++)
    {
        double current = it->position();
        it ++;
        next = it->position();
        it --;
        double left = (current - previous)/2 + previous;
        double right = (next - current)/2 + current;
        it->counter = right - left;
        previous = current;
    }
    it->counter = it->position() - previous;

    // computation of structure factor:
    Histogram<double> structureFunction {0.5};
    double k = 0;
    while( k < getWidth()/2 )
    {
        structureFunction.add_data(k, 0.5);     // includes intitial point for r=0 where cos(k*0)*corr(0)*deltar = 0.5 because corr(0)=1 and deltar = 0.5
        for(auto& B: correlation)
        {
            structureFunction.add_data(k, std::cos( k*B.position()*2*M_PI/getWidth() ) * B.counter * deltaR.get_data(B.position()) );
        }
        k += 0.5;
    }

    return structureFunction;
}


// void Spinsystem::computeSystemTimesCos() const
// {
//     // compute S(x,y)*cos(k0*y)

//     Q_CHECK_PTR(parameters);
//     std::string filekeystring = parameters->getFileKey();
//     std::string filekey = filekeystring.substr( 0, filekeystring.find_first_of(" ") );
//     filekey.append(".cos");

//     std::ofstream FILE;
//     FILE.open(filekey);

//     std::vector<double> amplitudes;
//     for( int k = 0; k < getWidth() / 2; ++k )
//     {
//         amplitudes.emplace_back(0);
//         for( unsigned int y = 0; y < getWidth(); ++y )
//         {
//             double cosTerm = ((0.5*std::cos(k*(2*M_PI/getWidth())*static_cast<double>(y+0.5)) + 1) / 2);
//             int sumSy = 0;
//             for(unsigned int x=y*getWidth(); x<(y+1)*getWidth()-1; ++x)
//             {
//                 sumSy += spins[x].getType();
//             }
//             // std::cout << y << "   " << (double) sumSy / getWidth() << " " << cosTerm  << " " << cosTerm * sumSy / getWidth() << std::endl; 
//             amplitudes.back() += cosTerm * sumSy / getWidth();
//         }
//         FILE << k << "  " << amplitudes.back() << '\n';
//     }

//     FILE.close();
// }



