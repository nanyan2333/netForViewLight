#include "./src/randomBin/random.hpp"
#include "./src/randomBin/random.cpp"
int main(int argc, char** argv)
{
    randomBin::createRandomBin("python ./src/randomBin/randomBin.py", std::atoi(argv[1]));
    return 0;
}