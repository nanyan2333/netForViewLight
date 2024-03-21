#include "parsingQRcodes.h"
#include "identifyQRcode.h"
#include "encode.h"
#include "decode.h"

//CNI_Exp1 in.bin in.mp4 1000
//CNI_Exp1 in.mp4 out.bin vout.bin

int main()
{
	bool type = 0;
	//type == true ���ļ��������Ƶ�������в����������ļ�·�� �����Ƶ·��
	//type == flase ����Ƶ������ļ��������в�����������Ƶ·�� ����ļ�·��

	if (type) {
		string str;
		//ReadBinFile(str, argv[1]);
		ReadBinFile(str, "1.bin");

		vector<Mat> img;
		DrawQRCode(str, img);

		//PicToVideo(img, argv[2]);
		PicToVideo(img, "out.mp4");

		//int max_time = int(argv[3]);
	}
	else {
		vector<Mat> video_img;
		//VideoToPic(argv[1], video_img);
		VideoToPic("testc.mp4", video_img);

		vector<Mat> QRcode;
		for (int i = 0; i < video_img.size(); i++) {
			IdentifyQRcode(video_img[i], QRcode);
		}

		string video_data;
		QRCodeToData(QRcode, video_data);

		//WriteBinFile(video_data, argv[2]);
		WriteBinFile(video_data, "out.bin");
	}

	return 0;
}