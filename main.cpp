#include "./src/randomBin/random.hpp"
#include "./src/randomBin/random.cpp"
int main()
{
    int img_num;
    std::cin >> img_num;
    for (int i = 1; i <= img_num; i++)
        randomBin::createRandomBin("python ./src/randomBin/randomBin.py", i);
    return 0;
}