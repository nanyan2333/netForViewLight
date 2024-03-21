#include "parsingQRcodes.h"
#include "encode.h"
#include "decode.h"

int main()
{
	bool type = 0;

	if (type) {
		string str;
		ReadBinFile(str, "in.bin");


		vector<Mat> img;
		DrawQRCode(str, img);
		PicToVideo(img, "out.mp4");
	}
	else {
		vector<Mat> video_img;
		VideoToPic("out.mp4", video_img);

		//Mat img = imread("test1.jpg");

		vector<Mat> QRcode;
		for (int i = 0; i < video_img.size(); i++) {
			parsingQRcodes(video_img[i], QRcode);
		}

		string video_data;
		QRCodeToData(QRcode, video_data);
		WriteBinFile(video_data, "out.bin");
	}

	return 0;
}