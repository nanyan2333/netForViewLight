#include "decrc.h"
#include"crc.h"

void decrc(string&end)
{
	vector<unsigned char> fileData;
	for (int i = 0; i < end.size(); i++)
		fileData.push_back(end[i]);
	end.clear();
	GenCrc32Tbl();
	unsigned char tmp[14] = { 0 };
	for (int i = 0; i < fileData.size(); i++) {
		if (i % 14 == 0 && i != 0)
		{
			unsigned int CRC32 = 0xffffffff; //设置初始值
			for (int j = 0; j < 10; j++)
				CRC32 = CRC_32_Tbl[(CRC32 ^ ((unsigned char*)tmp)[j]) & 0xff] ^ (CRC32 >> 8);
			unsigned char t1;
			unsigned char t2;
			unsigned char t3;
			unsigned char t4;
			t1 = (CRC32 & 0xff000000) >> 24;
			t2 = (CRC32 & 0xff0000) >> 16;
			t3 = (CRC32 & 0xff00) >> 8;
			t4 = (CRC32 & 0xff);
			if (t1 == tmp[10] && t2 == tmp[11] && t3 == tmp[12] && t4 == tmp[13])
				for (int j = 0; j < 10; j++)
					end += tmp[j];
			/*else
				for (int j = 0; j < 10; j++)
					end += '\0';*/
		}
		tmp[i % 14] = fileData[i];
		if (i == fileData.size() - 1)
		{
			unsigned int CRC32 = 0xffffffff; //设置初始值
			for (int j = 0; j < i % 14 - 3; j++)
				CRC32 = CRC_32_Tbl[(CRC32 ^ ((unsigned char*)tmp)[j]) & 0xff] ^ (CRC32 >> 8);
			unsigned char t1;
			unsigned char t2;
			unsigned char t3;
			unsigned char t4;
			t1 = (CRC32 & 0xff000000) >> 24;
			t2 = (CRC32 & 0xff0000) >> 16;
			t3 = (CRC32 & 0xff00) >> 8;
			t4 = (CRC32 & 0xff);
			if (t1 == tmp[i % 14 - 3] && t2 == tmp[i % 14 - 2] && t3 == tmp[i % 14 - 1] && t4 == tmp[i % 14])
				for (int j = 0; j < i % 14 - 3; j++)
					end += tmp[j];
			/*else
				for (int j = 0; j < i % 14 - 3; j++)
					end += '\0';*/
		}
	}
}