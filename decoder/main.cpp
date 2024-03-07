#include "decode.cpp"
#include "valid.cpp"

int main(int argc, char* argv[])
{
    if (argc == 4)
	{
        Decode(argv[1],argv[2]);
        Valid(argv[2],argv[3]);
    }
	else
	{
        std::cout << "First command line argument: " << argv[1] << std::endl;
    }
    return 0;
}  
