#include "spinsystem.hpp"

/*
 * functions in class "spinsys"
 */


Spinsystem::Spinsystem()
{
    
}



Spinsystem::~Spinsystem()
{
    delete parameters;
}



void Spinsystem::setParameters(ParametersWidget* prms)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    parameters = prms;
}



  
void Spinsystem::setup()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    assert(parameters);
//     assert(parameters->getHeight() == parameters->getWidth());
    
    spins.clear();
    lastFlipped.clear();
    Hamiltonian = 0;
    
    // safety check:
    if( parameters->getWidth()%2 != 0 && parameters->getConstrained() ) throw std::logic_error("system size must be an even number if system is constrained");
    
    auto size = parameters->getWidth();
    auto totalnumber = size*size;
    
    // set spinarray:
    SIGNED random;
    if( ! parameters->getConstrained() ) // initialise spins randomly
    {
        for(SIZE i=0; i<totalnumber; ++i)
        {
            random = random_int(0,1);
            spins.emplace_back(i, random == 1 ? UP : DOWN );
        }
    }                   // constraint: # spin up == # spin down
    else
    {
        for(SIZE i=0; i<totalnumber; ++i)
            spins.emplace_back(i, UP);
        for(SIZE i=0; i<totalnumber/2; ++i)
        {
            do
            {
                random = random_int(0, totalnumber-1);
            }
            while( spins[random].get_type() == DOWN );
            spins[random].set_type(DOWN);
        }

    }

    // set neighbours:
    for(auto& s: spins){
        std::vector<std::reference_wrapper<Spin> > Nrefs;
        SIZE Nid;
        const SIZE id = s.get_ID();

        {
            // up
            Nid = ((LONGSIGNED)id - static_cast<LONGSIGNED>(size)) < 0 ? id - size + totalnumber : id - size; 
            assert( Nid < spins.size() );
            Nrefs.push_back( std::ref(spins[Nid]) );
        }

        {
            // right
            Nid = (id + 1) % size == 0  ? id + 1 - size : id + 1;
            assert( Nid < spins.size() );
            Nrefs.push_back( std::ref(spins[Nid]) );
        }

        {
            // below
            Nid = id + size >= totalnumber ? id + size - totalnumber : id + size;
            assert( Nid < spins.size() );
            Nrefs.push_back( std::ref(spins[Nid]) );
        }

        {
            // left
            Nid = id % size == 0  ? id - 1 + size : id - 1;
            assert( Nid < spins.size() );
            Nrefs.push_back( std::ref(spins[Nid]) );
        }

        s.set_neighbours(Nrefs);
    }

// debugging:
#ifndef NDEBUG
    for(auto& s: spins){
      std::cout << "spin " << s.get_ID() << " has neighbours : ";
      std::for_each( s.begin(), s.end(), [](const auto& N){std::cout << N.get().get_ID() << " ";} );
      std::cout << "\n";
    }
#endif

    // calculate initial Hamiltonian:
    Hamiltonian = std::accumulate(std::cbegin(spins),std::cend(spins), static_cast<REAL>(0), [&](REAL i, const Spin& S)
                        {
                            return i + local_energy(S);
                        }) / 2;
}

/***************************************************************************/

REAL Spinsystem::local_energy(const Spin & _spin) const
{
    // go through neighbours and calculate local energy for given spin
    // Jij returns value of J/J for given spin pairs (1 or 0)
    // num_signed<T> returns the number of neighbours of type T for given spin:
    // ... sum( sigma_own * sigma_T ) --> therefore signed !
    return parameters->getInteraction() * static_cast<REAL>( -JijwithoutJ(SPINTYPE::UP,   _spin.get_type()) * _spin.num_signed<SPINTYPE::UP>()
                                    -JijwithoutJ(SPINTYPE::DOWN, _spin.get_type()) * _spin.num_signed<SPINTYPE::DOWN>() );
}

/***************************************************************************/

SIGNED Spinsystem::JijwithoutJ(const SPINTYPE _spin1, const SPINTYPE _spin2) const
{
    // return correct J for this pair of spins depending on CONSTRAINED
    if( ! parameters->getConstrained() ) return 1;
    else                return _spin1 != _spin2 ? 1 : 0;
}

/***************************************************************************/

void Spinsystem::flip()
{
    lastFlipped.clear();
    REAL localEnergy_before = 0;
    REAL localEnergy_after = 0;

    if( ! parameters->getConstrained() )
    {
        // find random spin
        auto randomspin = random_iterator(spins);
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
        // find first random spin
        auto randomspin = random_iterator(spins);
        // search for second spin
        auto randomspin2 = randomspin;
        do
        {
            randomspin2 = random_iterator(spins);
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
    std::cout << "flipping spin: ";
    for(const auto& s: lastFlipped) std::cout << s.get().get_ID() << " ";
    std::cout << "\n";
#endif
}

/***************************************************************************/

void Spinsystem::flip_back()
{
    if( lastFlipped.size() == 0 )
        throw std::logic_error("Cannot flip back, since nothing has flipped yet");
    else
    {
        SIGNED localEnergy_before = 0;
        SIGNED localEnergy_after = 0;
        
        for( const auto& s: lastFlipped ) localEnergy_before += local_energy( s.get() );
        for( const auto& s: lastFlipped ) s.get().flip();
        for( const auto& s: lastFlipped ) localEnergy_after += local_energy( s.get() );
        
        // update Hamiltonian
        Hamiltonian += localEnergy_after - localEnergy_before;
    }

#ifndef NDEBUG
    std::cout << "flipping back: ";
    for(const auto& s: lastFlipped) std::cout << s.get().get_ID() << " ";
    std::cout << "\n";
#endif

}

/***************************************************************************/

void Spinsystem::print(std::ostream & stream) const
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    // print spinarray to stream
    for(const auto& s: spins)
    {
        stream << ( s.get_type() == DOWN ? "-" : "+" )
        << ( (s.get_ID()+1)%parameters->getWidth() == 0 ? "\n" : " ");
    }
}

/***************************************************************************/
