#include "random.hpp"
void randomBin::createRandomBin(std::string cmd,int num)
{
    std::system((cmd+" "+std::to_string(num)).c_str());
}