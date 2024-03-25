#include "check.h"

void checkVout(const string in_bin_path, const string out_bin_path, const string vout_bin_path)
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
		diff_file.put(byte3);
	}

	file1.close();
	file2.close();
	diff_file.close();

	return;
}