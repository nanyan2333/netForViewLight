#include "check.h"

void checkVout(char* incrc_bin_path, char* out_bin_path, char* vout_bin_path)
{
	std::ifstream file1(in_bin_path, std::ios::binary);
	std::ifstream file2(out_bin_path, std::ios::binary);
	std::ofstream diff_file(vout_bin_path, std::ios::binary);

	if (!file1.is_open() || !file2.is_open() || !diff_file.is_open()) {
		std::cerr << "Error opening files!" << std::endl;
		return;
	}

	char byte1, byte2;
	while (file1.get(byte1) && file2.get(byte2))
	{
		uchar byte3 = ~((uchar)byte1 ^ (uchar)byte2);
		//低字节
		byte3 &= 0x0f;
		if (byte3 < 10) {
			byte3 += '0';  // 0-9的字符表示
		}
		else {
			byte3 += 'A' - 10;  // A-F的字符表示
		}
		diff_file.put(byte3);
	}

	file1.close();
	file2.close();
	diff_file.close();

	return;
}
