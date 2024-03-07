#include<iostream>
#include<string>
#include<stdlib.h>

void Decode(std::string InPath,std::string OutPath)
{
	system(("ffmpeg -i " + InPath + " -c copy -f rawvideo " + OutPath).c_str());
}



