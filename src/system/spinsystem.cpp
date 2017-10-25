#include "spinsystem.hpp"


void Spinsystem::setParameters(ParametersWidget* prms)
{
    Q_CHECK_PTR(prms);
    parameters = prms;
    Q_CHECK_PTR(parameters);
}




void Spinsystem::setup()
{
    Q_CHECK_PTR(parameters);

    spins.clear();
    lastFlipped.clear();
    correlation.clear();
    Hamiltonian = 0;

    // safety check:
    if( parameters->getConstrained() && (parameters->getWidth()*parameters->getHeight()) % 2 != 0 )
    {
        throw std::logic_error("system size must be an even number if system is constrained");
    }
    if( parameters->getConstrained() && parameters->getMagnetic() != 0 )
    {
        throw std::logic_error("constrained system cannot have a magnetic field");
    }

    auto width  = parameters->getWidth();
    auto height = parameters->getHeight();
    auto totalnumber = width * height;

    // set spinarray:
    int random;
    if( ! parameters->getConstrained() ) // initialise spins randomly
    {
        for(unsigned int i=0; i<totalnumber; ++i)
        {
            random = enhance::random_int(0,1);
            spins.emplace_back(i, random == 1 ? UP : DOWN );
        }
    }                   // constrain: # spin up == # spin down
    else
    {
        for(unsigned int i=0; i<totalnumber; ++i)
            spins.emplace_back(i, UP);
        for(unsigned int i=0; i<totalnumber/2; ++i)
        {
            do
            {
                random = enhance::random_int(0, totalnumber-1);
            }
            while( spins[random].get_type() == DOWN );
            spins[random].set_type(DOWN);
        }

    }

    // set neighbours:
    for(auto& s: spins)
    {
        std::vector<std::reference_wrapper<Spin> > Nrefs;
        unsigned int Nid;
        const unsigned int id = s.get_ID();

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

        s.set_neighbours(Nrefs);
    }
    // debugging:
    for(auto& s: spins)
    {
        qDebug() << "spin " << s.get_ID() << " has neighbours : ";
        std::for_each( s.begin(), s.end(), [](const auto& N){qDebug() << N.get().get_ID() << " ";} );
        qDebug() << "\n";
    }

    // calculate initial Hamiltonian:
    Hamiltonian = std::accumulate(std::cbegin(spins), std::cend(spins), static_cast<float>(0), [&](float i, const Spin& S)
                        {
                            return i + local_energy(S);
                        }) / 2;
    qDebug() << "initial H =  " << Hamiltonian << '\n';

    // set bins in correlations:
    std::cout << "setting possible distance values" << std::endl;
    for(unsigned int i=0; i<=parameters->getWidth()/2; ++i)
    {
        for(unsigned int j=i; j<=parameters->getHeight()/2; ++j)
        {
            if( j != 0 )
                 correlation.add_bin( std::sqrt(i*i+j*j));
        }
    }
    std::cout << correlation.formatted_string() << std::endl;
    correlate();
    std::cout << correlation.formatted_string() << std::endl;
}

/***************************************************************************/

float Spinsystem::local_energy(const Spin& _spin) const
{
    // calculate local energy for given spin
    
    float energy = - Jij(SPINTYPE::UP, _spin.get_type()) * _spin.num_signed<SPINTYPE::UP>()
                   - Jij(SPINTYPE::DOWN, _spin.get_type()) * _spin.num_signed<SPINTYPE::DOWN>();
    energy -= parameters->getMagnetic() * (_spin.get_type() == SPINTYPE::UP ? 2.f : -2.f);
    qDebug() << "spin # " << _spin.get_ID() << "  has local energy " << energy;
    return energy;
}

/***************************************************************************/

float Spinsystem::Jij(const SPINTYPE _spin1, const SPINTYPE _spin2) const
{
    // return correct J for this pair of spins depending on CONSTRAINED
    
    if( ! parameters->getConstrained() ) 
        return parameters->getInteraction();
    else                
        return _spin1 != _spin2 ? parameters->getInteraction() : 0;
}

/***************************************************************************/

void Spinsystem::flip()
{
    lastFlipped.clear();
    float localEnergy_before = 0.f;
    float localEnergy_after = 0.f;

    if( ! parameters->getConstrained() )
    {
        // find random spin
        auto randomspin = enhance::random_iterator(spins);
        lastFlipped.emplace_back( std::ref(*randomspin) );
        // flip spin
        localEnergy_before = local_energy( *randomspin );
        randomspin->flip();
        localEnergy_after = local_energy( *randomspin );
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
        } while( randomspin->num_opposite() == 0 );
        
        // find random neighbour
        auto randomneighbour = enhance::random_iterator(randomspin->get_neighbours());
        do
        {
                randomneighbour = enhance::random_iterator(randomspin->get_neighbours());
        } while( randomneighbour->get().get_type() == randomspin->get_type() );
        
        // flip spins
        lastFlipped.emplace_back(*randomspin);
        lastFlipped.emplace_back(randomneighbour->get());
        localEnergy_before = local_energy(*randomspin) + local_energy(randomneighbour->get());
        randomspin->flip();
        randomneighbour->get().flip();
        localEnergy_after = local_energy(*randomspin) + local_energy(randomneighbour->get());

        // update Hamiltonian
        Hamiltonian += localEnergy_after - localEnergy_before;
    }

#ifndef NDEBUG
    qDebug() << "flipping spin: ";
    for(const auto& s: lastFlipped) qDebug() << s.get().get_ID() << " ";
    qDebug() << "\n";
#endif
}

/***************************************************************************/

void Spinsystem::flip_back()
{
    if( lastFlipped.size() == 0 )
        throw std::logic_error("Cannot flip back, since nothing has flipped yet");
    else
    {
        int localEnergy_before = 0;
        int localEnergy_after = 0;

        for( const auto& s: lastFlipped ) localEnergy_before += local_energy( s.get() );
        for( const auto& s: lastFlipped ) s.get().flip();
        for( const auto& s: lastFlipped ) localEnergy_after += local_energy( s.get() );

        // update Hamiltonian
        Hamiltonian += localEnergy_after - localEnergy_before;
    }

// #ifndef NDEBUG
    qDebug() << "flipping back: ";
    for(const auto& s: lastFlipped) qDebug() << s.get().get_ID() << " ";
    qDebug() << "\n";
// #endif

}

/***************************************************************************/

void Spinsystem::print(std::ostream & stream) const
{
    // print spinarray to stream
    for(const auto& s: spins)
    {
        stream << ( s.get_type() == DOWN ? "-" : "+" )
        << ( (static_cast<unsigned int long>(s.get_ID() + 1)) % parameters->getWidth() == 0 ? "\n" : " ");
    }
}

/***************************************************************************/

std::string Spinsystem::str() const
{
    // print spinarray to stream
    std::stringstream sstream;
    for(const auto& s: spins)
    {
        sstream << ( s.get_type() == DOWN ? "-" : "+" )
        << ( (static_cast<unsigned int long>(s.get_ID() + 1)) % parameters->getWidth() == 0 ? '\n' : ' ');
    }
    
    return sstream.str(); 
}

/***************************************************************************/

const char* Spinsystem::c_str() const
{
    return this->str().c_str();
}

/***************************************************************************/

float Spinsystem::distance(const Spin& _spin1, const Spin& _spin2) const
{
    int a, b, c, d, x, y;

    a = _spin1.get_ID() % getWidth();
    b = _spin1.get_ID() / getWidth();
    c = _spin2.get_ID() % getWidth();
    d = _spin2.get_ID() / getWidth();


    x = (std::abs(c - a) <= static_cast<int>(getWidth()/2) ? std::abs(c - a) : std::abs(c - a) - getWidth());
    y = (std::abs(d - b) <= static_cast<int>(getHeight()/2) ? std::abs(d - b) : std::abs(d - b) - getHeight());

    return  std::sqrt( x*x + y*y );
}

/***************************************************************************/

void Spinsystem::correlate()
{
    // compute correlations between spins

    correlation.reset();
    int counter = 0;

    for( auto s1 = std::begin(spins); s1 != std::end(spins); s1 += 1)
    {
        for( auto s2 = s1 + 1; s2 != std::end(spins); s2 += 1)
        {
            std::cout << "correlating " << std::setw(2) << s1->get_ID() << " with " << std::setw(2) << s2->get_ID() << " : ";
            if( s1->get_type() == s2->get_type() )
            {
                correlation.add_data( distance(*s1, *s2));
                std::cout << " 1 ";
            }
            else std::cout << "   ";
            std::cout << " distance " << distance(*s1, *s2) << std::endl;
            counter ++;
        }
    }
    correlation.normalise( (getWidth() * getHeight()) * ((getWidth() * getHeight()) - 1) / 2);
}