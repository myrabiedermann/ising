#include "spinsystem.hpp"


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

    Q_CHECK_PTR(parameters);

    computeHamiltonian();
    Logger::getInstance().write_new_line("[spinsystem]", "resetting parameters ... new initial H = ", Hamiltonian);
}



void Spinsystem::setup()
{
    // setup of the spinsystem: add all spins, add corresponding neighbours to each spin, et all spintypes randomly

    qDebug() << __PRETTY_FUNCTION__;

    Q_CHECK_PTR(parameters);

    spins.clear();
    lastFlipped.clear();

    // some safety checks:
    if( parameters->getConstrained() && (getWidth()*getHeight()) % 2 != 0 )
    {
        throw std::logic_error("[spinsystem] system size must be an even number if system is constrained");
    }

    auto width  = getWidth();
    auto height = getHeight();
    auto totalnumber = width * height;

    // create spins:
    for(unsigned int i=0; i<totalnumber; ++i)
        spins.emplace_back(i, +1);

    // set neighbours:
    Logger::getInstance().write_new_line("[spinsystem]", "system setup: setting neighbours for", getWidth(), "*", getHeight(), "system");
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
    
    // set spins:
    if(parameters->getWavelengthPattern())
    {
        resetSpinsCosinus(parameters->getWavelength());
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

    Q_CHECK_PTR(parameters);

    int random;
    if( ! parameters->getConstrained() ) // initialise spins randomly
    {
        for( auto& s: spins )
        {
            random = enhance::random_int(0,1);
            s.setType( random == 1 ? +1 : -1 );
        }
    }      
    else  // constrained to specific up-spin to down-spin ratio
    {
        Logger::getInstance().write_new_line("[spinsystem]", "ratio =", parameters->getRatio(), ", results in", static_cast<unsigned int>(parameters->getRatio() * spins.size()), " down spins.");
        for( auto& s: spins ) 
            s.setType( +1 );
        for(unsigned int i=0; i<static_cast<unsigned int>(parameters->getRatio() * spins.size()); ++i)
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
    Logger::getInstance().write_new_line("[spinsystem]", "resetting spins randomly... new initial H =", Hamiltonian);
    Logger::getInstance().debug_new_line(str());

}



void Spinsystem::resetSpinsCosinus(const double k) 
{
    // set types of all spins new according to c(x) =  cos(kx) 

    qDebug() << __PRETTY_FUNCTION__;

    Q_CHECK_PTR(parameters);

    int random;
    if( ! parameters->getConstrained() ) 
        throw std::logic_error("[spinsystem] resetting spins according to cos not implemented for !CONSTRAINED");
    
    unsigned int totNrDownSpins = 0;
    for( auto& s: spins ) 
        s.setType( +1 );
    for(unsigned int i = 0; i<parameters->getWidth(); ++i)
    {
        double ratio = (std::cos(k*(2*M_PI/parameters->getWidth())*static_cast<double>(i+0.5)) + 1) / 2;
        unsigned int nrDownSpins = std::round(ratio*parameters->getWidth());
        totNrDownSpins += nrDownSpins;
        for(unsigned int j=0; j<nrDownSpins; ++j)
        {
            do
            {
                random = enhance::random_int(i*parameters->getWidth(), (i+1)*parameters->getWidth() - 1);
            }
            while( spins[random].getType() == -1 );
            spins[random].setType(-1);
        }
    }

    // clear / reset all vectors: 
    lastFlipped.clear();
    
    // calculate initial Hamiltonian:
    computeHamiltonian();
    Logger::getInstance().write_new_line("[spinsystem]", "resetting spins with cos(", parameters->getWavelength(),"y ) pattern ... new initial H =", Hamiltonian);
    Logger::getInstance().write_new_line("[spinsystem]", "# of down spins:", totNrDownSpins);
    Logger::getInstance().debug_new_line(str());

}



double Spinsystem::local_energy_interaction(const Spin& _spin) const
{
    // calculate interaction contribution to local energy for given spin

    return - parameters->getInteraction() * _spin.sumNeighbours();
}



double Spinsystem::local_energy_magnetic(const Spin& _spin) const
{
    // calculate magnetic contribution to local energy for given spin

    return - parameters->getMagnetic() * _spin.getType();
}



void Spinsystem::computeHamiltonian()
{
    // calculate Hamiltonian of the system

    Hamiltonian = 0;
    for(const auto& S : spins)
    {
        Hamiltonian += local_energy_interaction(S) / 2 + local_energy_magnetic(S);
    }
}



void Spinsystem::flip()
{
    // perform one move and save flipped spins in lastFlipped

    lastFlipped.clear();
    double localEnergy_before = 0;
    double localEnergy_after = 0;

    if( ! parameters->getConstrained() )
    {
        // find random spin
        auto randomspin = enhance::random_iterator(spins);
        lastFlipped.emplace_back( std::ref(*randomspin) );
        // flip spin
        localEnergy_before = local_energy_interaction( *randomspin ) + local_energy_magnetic( *randomspin );
        randomspin->flip();
        localEnergy_after = local_energy_interaction( *randomspin ) + local_energy_magnetic( *randomspin );
        // update Hamiltonian:
        Hamiltonian += localEnergy_after - localEnergy_before;
    }
    else
    {
        // find random spin
        auto randomspin = enhance::random_iterator(spins);
        do
        {
            randomspin = enhance::random_iterator(spins);
        } while( randomspin->sumOppositeNeighbours() == 0 );
        
        // find random neighbour
        auto randomneighbour = enhance::random_iterator(randomspin->getNeighbours());
        do
        {
            randomneighbour = enhance::random_iterator(randomspin->getNeighbours());
        } while( randomneighbour->get().getType() == randomspin->getType() );
        
        // flip spins
        lastFlipped.emplace_back(*randomspin);
        lastFlipped.emplace_back(randomneighbour->get());
        localEnergy_before = local_energy_interaction(*randomspin) + local_energy_interaction(randomneighbour->get());
        randomspin->flip();
        randomneighbour->get().flip();
        localEnergy_after = local_energy_interaction(*randomspin) + local_energy_interaction(randomneighbour->get());

        // update Hamiltonian
        Hamiltonian += localEnergy_after - localEnergy_before;
        
    }

    Logger::getInstance().debug_new_line("[spinsystem]",  "flipping spin: ");
    for(const auto& s: lastFlipped) Logger::getInstance().debug( " ", s.get().getID());
}



void Spinsystem::flip_back()
{
    // flip all spins in lastFlipped

    if( lastFlipped.size() == 0 )
        throw std::logic_error("[spinsystem] Cannot flip back, since nothing has flipped yet");
    
    double localEnergy_before = 0;
    double localEnergy_after = 0;

    for( const auto& s: lastFlipped ) 
    {
        localEnergy_before += local_energy_interaction( s.get() ) + local_energy_magnetic( s.get() );
    }
    for( const auto& s: lastFlipped ) 
    {
        s.get().flip();
    }
    for( const auto& s: lastFlipped ) 
    {
        localEnergy_after += local_energy_interaction( s.get() ) + local_energy_magnetic( s.get() );
    }

    // update Hamiltonian
    Hamiltonian += localEnergy_after - localEnergy_before;

    Logger::getInstance().debug_new_line("[spinsystem]", "flipping back: ");
    for(const auto& s: lastFlipped) Logger::getInstance().debug("  ", s.get().getID());

}



double Spinsystem::getMagnetisation() const
{
    // compute magnetisation M = <S_i>

    int sum = 0;
    for( const auto& S: spins )
    {
        sum += S.getType();
    }
    return static_cast<double>(sum) / spins.size();
}



void Spinsystem::print(std::ostream & stream) const
{
    // print spins to stream

    for( const auto& s: spins )
    {
        stream << ( s.getType() == -1 ? "-" : "+" )
        << ( (static_cast<unsigned int>(s.getID() + 1)) % parameters->getWidth() == 0 ? '\n' : ' ');
    }
}



std::string Spinsystem::str() const
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
    Logger::getInstance().write_new_line("[spinsystem]", "computing correlation <Si Sj>");

    // first: <S(0)S(r)>:
    for( auto s1 = std::begin(spins); s1 != std::end(spins); s1 += 1)
    {
        // for( auto s2 = s1 + 1; s2 != std::end(spins); s2 += 1)
        for( auto s2 = std::begin(spins); s2 != std::end(spins); s2 += 1)
        {
            if( s1 != s2 )
            {
                double dist = distance(*s1, *s2);
                if( dist < (double) parameters->getWidth()/2 + binWidth/2 )
                {
                    correlation.add_data( dist, s1->getType() == s2->getType() ? 1 : -1);
                    counter.add_data( dist );
                    Logger::getInstance().debug_new_line("            ", "correlating ", s1->getID(), " with ", s2->getID()," : ", s1->getType() == s2->getType() ? 1 : -1);
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



std::vector<double> Spinsystem::computeStructureFunction(Histogram<double> correlation) const
{
    // compute Fourier transformation S(k) = integral cos(2PI/width*k*r) G(r) dr, with r = distance between spins

    Logger::getInstance().write_new_line("[spinsystem]", "computing structure function S(k)");

    std::vector<double> structureFunction;

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
        // Logger::getInstance().write_new_line("previous = ", previous, "current = ", current, "next = ", next);
        // Logger::getInstance().write_new_line(it->position(), "min = ", left, "max = ", right, "deltaR = ", it->counter);
        previous = current;
        // Logger::getInstance().write_new_line();
    }
    // it ++;
    it->counter = it->position() - previous;

    // Logger::getInstance().write_new_line("correlation:");
    // Logger::getInstance().write_new_line(correlation.formatted_string());
    // Logger::getInstance().write_new_line("deltaR:");
    // Logger::getInstance().write_new_line(deltaR.formatted_string());

    // computation of structure factor:
    for(double k=0; k<parameters->getWidth()/2; k+=0.5)
    {
        // Logger::getInstance().write_new_line("k =", k);
        structureFunction.push_back(0);
        // structureFunction.back() += 0.5;    // initial point where corr(0) = 1
        for(auto& B: correlation)
        {
            // positionLeft = (B.position() - lastPosition)*0.5 + lastPosition;

            structureFunction.back() += std::cos( k*B.position()*2*M_PI/parameters->getWidth() ) * B.counter * deltaR.get_data(B.position());
        }
        // Logger::getInstance().write(" S(k) = ", structureFunction.back());
    }

    return structureFunction;
}




