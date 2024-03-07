#include<iostream>
	
void Valid(std::string OutPath,std::string filePath)
{
	FILE* fp1 = fopen(OutPath.c_str(), "rb");
	if (fp1 == nullptr) return;
	fseek(fp1, 0, SEEK_END);
	int size1 = ftell(fp1);
	rewind(fp1);
	
	FILE* fp2 = fopen("../../assets/bin/in.bin", "rb");
	if (fp2 == nullptr) return;
	fseek(fp2, 0, SEEK_END);
	int size2 = ftell(fp2);
	rewind(fp2);
	
	FILE* fp3 = fopen(filePath.c_str(), "wb");

	bool a = fgetc(fp1), b = fgetc(fp2);
    while (a || b)
	{
        bool result = ~(a ^ b);
        fputc(result,fp3);
    }
}
	
