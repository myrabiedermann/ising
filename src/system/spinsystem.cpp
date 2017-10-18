#include "spinsystem.hpp"



Spinsystem::Spinsystem()
{}



Spinsystem::~Spinsystem()
{}



void Spinsystem::setParameters(ParametersWidget* prms)
{
    Q_CHECK_PTR(prms);
    parameters = prms;
    Q_CHECK_PTR(parameters);
}




void Spinsystem::setup()
{
    Q_CHECK_PTR(parameters);
//     assert(parameters->getHeight() == parameters->getWidth());

    spins.clear();
    lastFlipped.clear();
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
// #ifndef NDEBUG
    for(auto& s: spins)
    {
        qDebug() << "spin " << s.get_ID() << " has neighbours : ";
        std::for_each( s.begin(), s.end(), [](const auto& N){qDebug() << N.get().get_ID() << " ";} );
        qDebug() << "\n";
    }
// #endif

    // calculate initial Hamiltonian:
    Hamiltonian = std::accumulate(std::cbegin(spins),std::cend(spins), static_cast<double>(0), [&](double i, const Spin& S)
                        {
                            return i + local_energy(S);
                        }) / 2;
    qDebug() << "initial H without magnetic part: " << Hamiltonian << '\n';
    // add magnetic part
    Hamiltonian += -1.0 * num<SPINTYPE::UP>() * parameters->getMagnetic() + 1.0 * num<SPINTYPE::DOWN>() * parameters->getMagnetic();
    qDebug() << "    ... and plus magnetic part: " << Hamiltonian << '\n';
}

/***************************************************************************/

double Spinsystem::local_energy(const Spin & _spin) const
{
    // go through neighbours and calculate local energy for given spin
    // Jij returns value of J/J for given spin pairs (1 or 0)
    // num_signed<T> returns the number of neighbours of type T for given spin:
    // ... sum( sigma_own * sigma_T ) --> therefore signed !
    double energy = parameters->getInteraction() 
                    * static_cast<double>( -JijwithoutJ(SPINTYPE::UP, _spin.get_type()) * _spin.num_signed<SPINTYPE::UP>()
                                           -JijwithoutJ(SPINTYPE::DOWN, _spin.get_type()) * _spin.num_signed<SPINTYPE::DOWN>() );
    return energy;
}

/***************************************************************************/

int Spinsystem::JijwithoutJ(const SPINTYPE _spin1, const SPINTYPE _spin2) const
{
    // return correct J for this pair of spins depending on CONSTRAINED
    if( ! parameters->getConstrained() ) return 1;
    else                return _spin1 != _spin2 ? 1 : 0;
}

/***************************************************************************/

void Spinsystem::flip()
{
    lastFlipped.clear();
    double localEnergy_before = 0;
    double localEnergy_after = 0;

    if( ! parameters->getConstrained() )
    {
        // find random spin
        auto randomspin = enhance::random_iterator(spins);
        lastFlipped.emplace_back( std::ref(*randomspin) );
        // flip spin
        localEnergy_before = local_energy( *randomspin ) 
                           - parameters->getMagnetic() * (randomspin->get_type() == SPINTYPE::UP ? 1.0 : -1.0);
        randomspin->flip();
        localEnergy_after = local_energy( *randomspin )
                          - parameters->getMagnetic() * (randomspin->get_type() == SPINTYPE::UP ? 1.0 : -1.0);
        // update Hamiltonian:
        Hamiltonian += localEnergy_after - localEnergy_before;
    }
    else
    {
        // find first random spin
        auto randomspin = enhance::random_iterator(spins);
        // search for second spin
        auto randomspin2 = randomspin;
        do
        {
            randomspin2 = enhance::random_iterator(spins);
        }
        while( randomspin->get_type() == randomspin2->get_type() or randomspin->get_ID() == randomspin2->get_ID() );
        lastFlipped.emplace_back(*randomspin);
        lastFlipped.emplace_back(*randomspin2);
//         lastFlipped.emplace_back( std::ref(*randomspin) );
//         lastFlipped.emplace_back( std::ref(*randomspin2) );
        // flip both spins:
        localEnergy_before = local_energy( *randomspin ) + local_energy( *randomspin2 );
        randomspin->flip();
        randomspin2->flip();
        localEnergy_after = local_energy( *randomspin ) + local_energy( *randomspin2 );
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
    qDebug() << __PRETTY_FUNCTION__ << '\n';
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
    qDebug() << __PRETTY_FUNCTION__ << '\n';
    // print spinarray to stream
    std::stringstream stream;
    // for(const auto& s: spins)
    // {
    //     stream << ( s.get_type() == DOWN ? "-" : "+" )
    //     << ( (s.get_ID()+1)%parameters->getWidth() == 0 ? "\n" : " ");
    // }
    stream << *this;

    return stream.str();
}

/***************************************************************************/

const char* Spinsystem::c_str() const
{
    qDebug() << __PRETTY_FUNCTION__ << '\n';

    return str().c_str();
}
