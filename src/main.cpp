#include "identifyQRcode.h"
#include "encode.h"
#include "decode.h"
#include "crc.h"
#include "decrc.h"
#include<time.h>
int main(int argc, char* argv[])
{
	bool type = 1;
	/*if (argc != 4)
	{
		return 0;
	}

	if (argv[0] == "decode")
	{
		type = false;
	}
	else if (argv[0] == "encode")
	{
		type = true;
	}
	else
	{
		return 0;
	}
	*/
	if (type) {
		string str;
		ReadBinFile(str, "4.bin");
		crc(str);

		vector<Mat> img;
		DrawQRCode(str, img,"10000");
		PicToVideo(img, "out.mp4");
	}
	else {
		clock_t start = clock();
		vector<Mat> video_img;
		VideoToPic(".mp4", video_img);

		vector<Mat> QRcode;

		for (int i = 0; i < video_img.size(); i++) {
			IdentifyQRcode(video_img[i], QRcode);
		}

		string video_data;
		QRCodeToData(QRcode, video_data);
		decrc(video_data,"vout.bin");
		WriteBinFile(video_data,"out.bin");
		clock_t end = clock();
		cout << end - start;
	}
	return 0;
}
