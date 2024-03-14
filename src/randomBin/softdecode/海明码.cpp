#include<iostream>
#include<bitset>
#include<cmath>
#include<fstream>
#include <sys/stat.h> 
#include<vector>
#include <cassert>
using namespace std;
string bm(string text)
{
	string final(23,'0');
	int j = 0;
	for (int i = 1; i <= 22; i++)
	{
		if (i == 1)
			final[i] = text[0] ^ text[1] ^ text[3] ^ text[4] ^ text[6] ^ text[8] ^ text[10] ^ text[11] ^ text[14];
		else if (i == 2)
			final[i] = text[0] ^ text[2] ^ text[3] ^ text[5] ^ text[6] ^ text[9] ^ text[10] ^ text[12] ^ text[15];
		else if (i == 4)
			final[i] = text[1] ^ text[2] ^ text[3] ^ text[7] ^ text[8] ^ text[9] ^ text[10] ^ text[13] ^ text[14] ^ text[15];
		else if (i == 8)
			final[i] = text[4] ^ text[5] ^ text[6] ^ text[7] ^ text[8] ^ text[9] ^ text[10];
		else if (i == 16)
			final[i] = text[11] ^ text[12] ^ text[13] ^ text[14] ^ text[15];
		else if (i == 22)
			final[i] = final[1] ^ final[2] ^ final[4] ^ final[8] ^ final[16] ^ text[0] ^ text[1] ^ text[2] ^ text[3] ^ text[4] ^ text[5] ^ text[6] ^ text[7] ^ text[8] ^ text[9] ^ text[10] ^ text[11] ^ text[12] ^ text[13] ^ text[14] ^ text[15];
		else
			final[i] = text[j++];
	}
	for (int i = 1; i <= 21; i++)
		cout << final[i];
	return final;
}
string jm(string text)
{
	int G[6];
	G[0]= text[1]^text[3] ^ text[5] ^ text[7] ^ text[9] ^ text[11] ^ text[13] ^ text[15] ^ text[17] ^ text[20];
	G[1]= text[2] ^text[3] ^ text[6] ^ text[7] ^ text[10] ^ text[11] ^ text[14] ^ text[15] ^ text[18] ^ text[21];
	G[2]=text[4] ^ text[5] ^ text[6] ^ text[7] ^ text[12] ^ text[13] ^ text[14] ^ text[15] ^ text[19] ^ text[20] ^ text[21];
	G[3]= text[8] ^ text[9] ^ text[10] ^ text[11] ^ text[12] ^ text[13] ^ text[14] ^ text[15];
	G[4] = text[16] ^ text[17] ^ text[18] ^ text[19] ^ text[20] ^ text[21];
	G[5] =  text[1] ^ text[2] ^ text[3] ^ text[4] ^ text[5] ^ text[6] ^ text[7] ^ text[8] ^ text[9] ^ text[10] ^ text[11] ^ text[12] ^ text[13] ^ text[14] ^ text[15] ^  text[16]^text[17] ^ text[18] ^ text[19] ^ text[20] ^ text[21] ^ text[22];
	int B[5][10] = { {3,5,7,9,11,13,15,17,20},{3,6,7,10,11,14,15,18,21},{5,6,7,12,13,14,15,19,20,21},{9,10,11,12,13,14,15},{17,18,19,20,21} };
	int flag = G[0] & G[1] & G[2] & G[3] & G[4];
	int flag2 = G[5];
	/*if (flag != 0 && flag2 != 0) {
		int flag3[5] = { 0,0,0,0,0 };
		for (int i = 0; i < 5; i++)
			if (G[i] != 0)
				flag3[i] = 1;
		int hash[20] = {0};
		for (int i = 0; i < 5; i++)
			if (flag3[i] != 0)
				for (int j = 0; j < 10; j++)
					hash[B[i][j]]++;
		for (int i = 1; i < 21; i++)
			if (hash[i] > 1)
				text[i] = ~text[i];
	}*/
	unsigned char end[2];
	end[0] = pow(2, 7) * text[3] + pow(2, 6) * text[5] + pow(2, 5) * text[6] + pow(2, 4) * text[7] + pow(2, 3) * text[9] + pow(2, 2) * text[10] + pow(2, 1) * text[11] + pow(2, 0) * text[12];
	end[1] = pow(2, 7) * text[13] + pow(2, 6) * text[14] + pow(2, 5) * text[15] + pow(2, 4) * text[17] + pow(2, 3) * text[18] + pow(2, 2) * text[19] + pow(2, 1) * text[20] + pow(2, 0) * text[21];
	string endd;
	endd += end[0];
	endd += end[1];
	if (flag2 == 0 && flag != 0)
		return 0;
	else
		return endd;
}
int main()
{
	FILE* fin;
	fopen_s(&fin,"in.bin", "rb+");
	ofstream fout("out.bin", ios::out | ios::binary);
	std::vector<unsigned char> result;
	if (fin==nullptr)
	{
		cout << "文件无法打开";
		exit(1);
	}
	struct stat statbuf;
	stat("in.bin", &statbuf);
	size_t fileSize = statbuf.st_size;
	string bin;
	if (fileSize != 0) {
		result.resize(fileSize);
		size_t n = fread(&result[0], 1, fileSize, fin);
		assert(n <= fileSize);
		if (n != fileSize) {
			result.resize(n);
		}
	}
	char buf[1024];
	for (;;) {
		size_t n = fread(buf, 1, 1024, fin);
		result.insert(result.end(), buf, buf + n);
		if (n < 1024) {
			break;
		}
	}
	fclose(fin);
	char text[16];
	string end;
	for (int i = 0; i < result.size()-1; i++) {
		
		for(int j=0;j<8;j++)
			text[7-j] = ((result[i] >> j) & 1);
		i++;
		for(int j=0;j<8;j++)
			text[15-j]= ((result[i] >> j) & 1);
		string tmp(text, 16);
		end += jm(bm(tmp));
	}
	fout << end;
	fout.close();
	return 0;
}