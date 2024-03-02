#include "./src/randomBin/random.hpp"
#include "./src/randomBin/random.cpp"
int main()
{
    int img_num;
    std::cin >> img_num;
    randomBin::createRandomBin("python ./src/randomBin/randomBin.py", img_num);
    return 0;
}