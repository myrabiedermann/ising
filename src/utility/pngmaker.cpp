#include "pngmaker.hpp"

/*
 * struct pngmaker
 */

// constructor:
pngmaker::pngmaker(const int & _magnifier)
 : magnifier (_magnifier)
{
}

/***************************************************************************/

void pngmaker::save(const std::string& _folder, const int& _filenr, std::stringstream& _snapshot)
{

    // conversion from stringstream to std::vector<char>:
    std::vector<char> snapshot;
    std::copy(std::istream_iterator<char>(_snapshot), std::istream_iterator<char>(),
            std::back_inserter(snapshot));
    unsigned int size = std::sqrt( snapshot.size() );
    
    char filename [30];
    sprintf(filename, "%s/%05d.png", _folder.c_str(), _filenr);

    pngwriter png(size*magnifier, size*magnifier, 0.4, filename);

    unsigned int j = 0;        // columns in png
    unsigned int i = size - 1; // rows in png (want to start writing on top left corner, but origin of png is in bottom left corner --> start with max row and decrease)
    for(unsigned int stepper=0; stepper<snapshot.size(); ++stepper){
        if( stepper % size == 0 and stepper != 0 ){
            --i;
            j = 0;
        }
        for(unsigned int k1=i*magnifier; k1<(i+1)*magnifier; ++k1){
            for(unsigned int k2=j*magnifier; k2<(j+1)*magnifier; ++k2)
                if( snapshot[stepper] == '+')
                    png.plot((int) k2, (int) k1, 0.7,0.1,0.2);		//colour: red - green - blue, all 0 = black, all 1 = white  
                else
                    png.plot((int) k2, (int) k1, 0.1,0.0,0.9);		//colour: red - green - blue, all 0 = black, all 1 = white
        }
                
        ++j;
    }

    png.close();

}
