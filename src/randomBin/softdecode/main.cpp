#include "identifyQRcode.h"
#include "encode.h"
#include "decode.h"
#include"crc.h"
#include"decrc.h"
int main()
{
	bool type = 0;
	if (type) {
		string str;
		ReadBinFile(str, "in.bin");
		crc(str);

		vector<Mat> img;
		DrawQRCode(str, img);
		PicToVideo(img, "out.mp4");
	}
	else {
		vector<Mat> video_img;
		VideoToPic("in.mp4", video_img);

		//Mat img = imread("test1.jpg");

		vector<Mat> QRcode;
		for (int i = 0; i < video_img.size(); i++) {
			IdentifyQRcode(video_img[i], QRcode);
		}

		string video_data;
		QRCodeToData(QRcode, video_data);
		decrc(video_data);
		WriteBinFile(video_data, "out.bin");
	}

	return 0;
}