#include "encode.h"

void ReadBinFile(string& str, const string fileName) {
	ifstream inFile;
	inFile.open(fileName, ios::binary);//打开文件

	if (!inFile) {
		cout << "Unable to open file!" << endl;
		exit(0);
	}

	//读入文件内容
	stringstream sStream;
	sStream << inFile.rdbuf();
	str = sStream.str();
	//cout << str << endl;
}


void DrawQRCode(string data1, vector<Mat>& img) {
	int key = 0;
	vector<uchar>data;
	for (int i = 0; i < data1.size(); i++)
		data.push_back(data1[i]);
	while (true) {
		Mat src(780, 780, CV_8UC3, Scalar(255, 255, 255));

		//左上定位点
		rectangle(src, Point(0, 0), Point(69, 69), Scalar(0, 0, 0), -1);
		rectangle(src, Point(10, 10), Point(59, 59), Scalar(255, 255, 255), -1);
		rectangle(src, Point(20, 20), Point(49, 49), Scalar(0, 0, 0), -1);
		//右上定位点
		rectangle(src, Point(710, 0), Point(779, 69), Scalar(0, 0, 0), -1);
		rectangle(src, Point(720, 10), Point(769, 59), Scalar(255, 255, 255), -1);
		rectangle(src, Point(730, 20), Point(759, 49), Scalar(0, 0, 0), -1);
		//左下定位点
		rectangle(src, Point(0, 710), Point(69, 779), Scalar(0, 0, 0), -1);
		rectangle(src, Point(10, 720), Point(59, 769), Scalar(255, 255, 255), -1);
		rectangle(src, Point(20, 730), Point(49, 759), Scalar(0, 0, 0), -1);

		//右下辅助点
		rectangle(src, Point(690, 690), Point(739, 739), Scalar(0, 0, 0), -1);
		rectangle(src, Point(700, 700), Point(729, 729), Scalar(255, 255, 255), -1);
		rectangle(src, Point(710, 710), Point(719, 719), Scalar(0, 0, 0), -1);

		int bits = 8;
		int flag = 0;
		//绘制数据信息不使用复杂的方法,直接按行列的形式填充
		while (true) {
			for (int col = 0; col < 78; col++) {
				for (int row = 0; row < 78; row++) {
					if (data.size() == key)
					{
						flag++;
						data.push_back('\0');
						data.push_back('\0');
					}
					if (flag == 2) {
						Mat backGround(860, 860, CV_8UC3, Scalar(255, 255, 255));
						Rect area(40, 40, 780, 780);
						src.copyTo(backGround(area));
						img.push_back(backGround);
						return;
					}
					if ((col < 8 && (row < 8 || row > 69)) || (col > 69 && row < 8) || (col > 67 && col < 75 && row>67 && row < 75)) {
						continue;
					}
					else {
						if (data.at(key) % 2 == 1) {
							rectangle(src, Point(col * 10, row * 10), Point(col * 10 + 9, row * 10 + 9), Scalar(0, 0, 0), -1);
						}
						else {
							rectangle(src, Point(col * 10, row * 10), Point(col * 10 + 9, row * 10 + 9), Scalar(255, 255, 255), -1);
						}
						bits--;
						data.at(key) /= 2;
					}
					if (bits == 0)
					{
						bits = 8;
						key++;
					}
				}
			}
			//imwrite("1.png", src);
			Mat backGround(860, 860, CV_8UC3, Scalar(255, 255, 255));
			Rect area(40, 40, 780, 780);
			src.copyTo(backGround(area));
			img.push_back(backGround);
		}
	}

	//cout << key << endl;

	//namedWindow("IMAGE");
	//imshow("IMAGE",src);
	//waitKey(0);
}

void PicToVideo(vector<Mat>& img, const string fileName) {
	int frame_rate = 4;
	VideoWriter video(fileName, VideoWriter::fourcc('m', 'p', '4', 'v'), frame_rate, Size(860, 860));

	for (int i = 0; i < img.size(); i++) {
		Mat image = img[i].clone();
		video << image;
	}
}
