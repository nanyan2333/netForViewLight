#include<iostream>
#include<fstream>
#include<vector>
using namespace std;
unsigned int CRC;
unsigned int CRC_32_Tbl[256];

void GenCrc32Tbl()
{
	for (int i = 0; i < 256; i++) {
		CRC = i;
		for (int j = 0; j < 8; ++j)
		{
			if (CRC & 1)
				CRC = (CRC >> 1) ^ 0xEDB88320;
			else
				CRC >>= 1;
		}
		CRC_32_Tbl[i] = CRC;
	}
}
int main()
{
	ifstream fin;
	fin.open("out.bin", ios::in | ios::binary);
	ofstream fout("卍解.bin", ios::out | ios::binary);
	if (!fin.is_open())
	{
		cout << "文件无法打开";
		exit(1);
	}
	vector<unsigned char> fileData;
	fin.seekg(0, std::ios::end);
	fileData.resize(fin.tellg());
	fin.seekg(0);
	fin.read(reinterpret_cast<char*>(fileData.data()), fileData.size());
	fin.close();
	GenCrc32Tbl();
	unsigned char tmp[104] = { 0 };
	string end;
	for (int i = 0; i < fileData.size(); i++) {
		if (i % 104 == 0 && i != 0)
		{
			unsigned int CRC32 = 0xffffffff; //设置初始值
			for (int j = 0; j < 100; j++)
				CRC32 = CRC_32_Tbl[(CRC32 ^ ((unsigned char*)tmp)[j]) & 0xff] ^ (CRC32 >> 8);
			unsigned char t1;
			unsigned char t2;
			unsigned char t3;
			unsigned char t4;
			t1 = (CRC32 & 0xff000000) >> 24;
			t2 = (CRC32 & 0xff0000) >> 16;
			t3 = (CRC32 & 0xff00) >> 8;
			t4 = (CRC32 & 0xff);
			if (t1 == tmp[100] && t2 == tmp[101] && t3 == tmp[102] && t4 == tmp[103])
				for (int j = 0; j < 100; j++)
					end += tmp[j];
		}
		tmp[i % 104] = fileData[i];
	}
	fout << end;
	fout.close();
	return 0;
}