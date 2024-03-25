#include"crc.h"
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
void crc(string& end)
{
	vector<unsigned char> fileData;
	for (int i = 0; i < end.size(); i++)
		fileData.push_back(end[i]);
	GenCrc32Tbl();
	end.clear();
	unsigned char tmp[10] = { 0 };
	for (int i = 0; i < fileData.size(); i++) {
		if (i % 10 == 0 && i != 0)
		{
			unsigned int CRC32 = 0xffffffff; //ÉèÖÃ³õÊ¼Öµ
			for (int j = 0; j < 10; j++)
			{
				end += tmp[j];
				CRC32 = CRC_32_Tbl[(CRC32 ^ ((unsigned char*)tmp)[j]) & 0xff] ^ (CRC32 >> 8);
			}
			unsigned char t1;
			t1 = (CRC32 & 0xff000000) >> 24;
			end += t1;
			t1 = (CRC32 & 0xff0000) >> 16;
			end += t1;
			t1 = (CRC32 & 0xff00) >> 8;
			end += t1;
			t1 = (CRC32 & 0xff);
			end += t1;

		}
		tmp[i % 10] = fileData[i];
		if (i == fileData.size() - 1)
		{
			unsigned int CRC32 = 0xffffffff; //ÉèÖÃ³õÊ¼Öµ
			for (int j = 0; j <= i % 10; j++)
			{
				end += tmp[j];
				CRC32 = CRC_32_Tbl[(CRC32 ^ ((unsigned char*)tmp)[j]) & 0xff] ^ (CRC32 >> 8);
			}
			unsigned char t1;
			t1 = (CRC32 & 0xff000000) >> 24;
			end += t1;
			t1 = (CRC32 & 0xff0000) >> 16;
			end += t1;
			t1 = (CRC32 & 0xff00) >> 8;
			end += t1;
			t1 = (CRC32 & 0xff);
			end += t1;
		}
	}
}