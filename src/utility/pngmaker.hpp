#pragma once

/*
 * struct pngmaker
 */

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include "../system/spinsys.hpp"
#include "../system/spin.hpp"
#include <pngwriter.h>


struct pngmaker{

   pngmaker(const int &);
   void save(const std::string&, const int&, std::stringstream&);

private:
    const int magnifier;

 };
