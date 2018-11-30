#include "enhance.hpp"


namespace enhance 
{
    unsigned int    seed;
    std::mt19937_64 rand_engine;


    // random double from [a,b)
    double randomDouble(double a, double b)
    {
        std::uniform_real_distribution<double> distribution(a,b);
        return distribution(rand_engine);
    }

    // random int from [a,b]
    int randomInt(int a, int b)
    {
        std::uniform_int_distribution<int> intdistribution(a,b);
        return intdistribution(rand_engine);
    }

    
    // check if a file exists
    bool fileExists(const std::string& filename)
    {
        struct stat buf;
        if (stat(filename.c_str(), &buf) != -1)
        {
            return true;
        }
        return false;
    }
}
