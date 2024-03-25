#pragma once
#include<iostream>
#include<fstream>
#include<vector>
using namespace std;
void GenCrc32Tbl();
void crc(string& end);
extern unsigned int CRC_32_Tbl[256];