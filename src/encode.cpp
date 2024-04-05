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

int en = 0;

void DrawQRCode(string data1, vector<Mat>& img, const string limitTime) {
	int frame_rate = 20;
	double target_duration_seconds = stoi(limitTime) * 1.0 / 1000;
	// 计算最大帧数（向上取整）  
	int target_frames = static_cast<int>(std::ceil(target_duration_seconds * frame_rate));
	int key = 0;
	int num = 0;
	vector<uchar>data;
	for (int i = 0; i < data1.size(); i++)
		data.push_back(data1[i]);
	while (true) {
		Mat src(1440, 1440, CV_8UC3, Scalar(255, 255, 255));

		//左上定位点
		rectangle(src, Point(0, 0), Point(69, 69), Scalar(0, 0, 0), -1);
		rectangle(src, Point(10, 10), Point(59, 59), Scalar(255, 255, 255), -1);
		rectangle(src, Point(20, 20), Point(49, 49), Scalar(0, 0, 0), -1);
		//右上定位点
		rectangle(src, Point(1370, 0), Point(1439, 69), Scalar(0, 0, 0), -1);
		rectangle(src, Point(1380, 10), Point(1429, 59), Scalar(255, 255, 255), -1);
		rectangle(src, Point(1390, 20), Point(1419, 49), Scalar(0, 0, 0), -1);
		//左下定位点
		rectangle(src, Point(0, 1370), Point(69, 1439), Scalar(0, 0, 0), -1);
		rectangle(src, Point(10, 1380), Point(59, 1429), Scalar(255, 255, 255), -1);
		rectangle(src, Point(20, 1390), Point(49, 1419), Scalar(0, 0, 0), -1);

		//右下辅助点
		rectangle(src, Point(1350, 1350), Point(1399, 1399), Scalar(0, 0, 0), -1);
		rectangle(src, Point(1360, 1360), Point(1389, 1389), Scalar(255, 255, 255), -1);
		rectangle(src, Point(1370, 1370), Point(1379, 1379), Scalar(0, 0, 0), -1);

		int bits = 8;
		int flag = 0;
		int page = 0;
		int real_page = 0;
		//绘制数据信息不使用复杂的方法,直接按行列的形式填充
		while (true) {
			real_page++;
			page = real_page;
			for (int col = 0; col < 144; col++) {
				for (int row = 0; row < 144; row++) {
					if (data.size() == key)
					{
						flag++;
						data.push_back('\0');
						data.push_back('\0');
					}
					if (flag == 2) {
						Mat backGround(1600, 1600, CV_8UC3, Scalar(255, 255, 255));
						Rect area(40, 40, 1440, 1440);
						src.copyTo(backGround(area));
						//imwrite(to_string(en++) + ".png", backGround);
						num++;
						if (num > target_frames)
						{
							return;
						}
						img.push_back(backGround);
						return;
					}
					if ((col < 8 && (row < 8 || row > 135)) || (col > 135 && row < 8) || (col > 123 && col < 141 && row>123 && row < 141)) {
						continue;
					}
					else if (col == 143 && row > 56)
						continue;
					else if (col == 0 && row < 16) {
						if (page % 2 == 1) {
							rectangle(src, Point(col * 10, row * 10), Point(col * 10 + 9, row * 10 + 9), Scalar(0, 0, 0), -1);
						}
						else {
							rectangle(src, Point(col * 10, row * 10), Point(col * 10 + 9, row * 10 + 9), Scalar(255, 255, 255), -1);
						}
						page /= 2;
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
			Mat backGround(1600, 1600, CV_8UC3, Scalar(255, 255, 255));
			Rect area(80, 80, 1440, 1440);
			src.copyTo(backGround(area));
			//imwrite(to_string(en++) + ".png", backGround);
			num++;
			if (num > target_frames)
			{
				return;
			}
			img.push_back(backGround);
		}
	}

	//cout << key << endl;

	//namedWindow("IMAGE");
	//imshow("IMAGE",src);
	//waitKey(0);
}

void PicToVideo(vector<Mat>& img, const string fileName) {
	int frame_rate = 20;
	VideoWriter video(fileName, VideoWriter::fourcc('m', 'p', '4', 'v'), frame_rate, Size(1600, 1600));

	for (int i = 0; i < img.size(); i++) {
		Mat image = img[i].clone();
		video << image;
	}
}
