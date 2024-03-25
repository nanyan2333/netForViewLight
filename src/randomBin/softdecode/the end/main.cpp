#include "identifyQRcode.h"
#include "encode.h"
#include "decode.h"
#include "crc.h"
#include "decrc.h"
#include "check.h"

int main(int argc, char* argv[])
{
	bool type = true;
	if (argc != 4)
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

	if (type) {
		string str;
		ReadBinFile(str, argv[1]);
		crc(str);
		WriteBinFile(str, "incrc.bin");

		vector<Mat> img;
		DrawQRCode(str, img);
		PicToVideo(img, argv[2], argv[3]);
	}
	else {
		vector<Mat> video_img;
		VideoToPic(argv[1], video_img);

		vector<Mat> QRcode;
		for (int i = 0; i < video_img.size(); i++) {
			IdentifyQRcode(video_img[i], QRcode);
		}

		string str;
		ReadBinFile(str, "incrc.bin");
		decrc(str);
		WriteBinFile(str, "checkout.bin");
		string video_data;
		QRCodeToData(QRcode, video_data);
		decrc(video_data);
		WriteBinFile(video_data, argv[2]);
		checkVout("checkout.bin", argv[2], argv[3]);
	}

	return 0;
}