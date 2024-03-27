#include "check.h"

void checkVout(const string out_bin_path, const string vout_bin_path)
{
	std::ifstream file1(out_bin_path, std::ios::binary);
	std::ofstream diff_file(vout_bin_path, std::ios::binary);

	if (!file1.is_open() || !diff_file.is_open()) {
		std::cerr << "Error opening files!" << std::endl;
		return;
	}

	char byte1;
	while (file1.get(byte1))
	{
		char byte[9];
		for (int i = 0; i < 9; i++)
		{
			file1.get(byte[i]);
			byte1 |= byte[i];
		}
		if (byte1 == 0)
		{
			for(int i=0;i<10;i++)
				diff_file.put(0);
		}
		else
		{
			for (int i = 0; i < 10; i++)
				diff_file.put(255);
		}
	}

	file1.close();
	diff_file.close();

	return;
}
