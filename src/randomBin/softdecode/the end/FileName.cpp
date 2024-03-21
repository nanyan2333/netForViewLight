#include "decode.h"
#include "huffman.h"
void VideoToPic(const string fileName, vector<Mat>& img) {
	VideoCapture video(fileName);
	int frame_num = video.get(CAP_PROP_FRAME_COUNT);
	//double fps = video.get(CAP_PROP_FPS);
	//cout << fps << endl;

	//int rate = (int)(fps / 10 + 0.5);

	for (int i = 1; i <= frame_num; i++) {
		Mat frame;
		video >> frame;
		//if (i % rate == 0 && !frame.empty()) {
		if (!frame.empty()) {
			imwrite( to_string(i) + ".jpg", frame);
			img.push_back(frame);
		}
	}
}
void QRCodeToData(vector<Mat>& img, string& data) {
	//int page = -1;
	int count = 0;
	int key = 0;
	int flag = 0;
	for (int i = 0; i < img.size(); i++) {
		/*int bits = 8;
		int number = 0;
		while (bits) {
			Scalar color = img[i].at<uchar>(Point(5, (8 + 8 - bits) * 10 + 5));
			if (color[0] < 128) {
				number += pow(2, 8 - bits);
			}
			bits--;
		}
		if (number != page + 1) {	//жҳ
			continue;
		}
		else {
			page += 1;
			//cout << number << endl;
		}*/
		for (int col = 0; col < 78; col++) {
			for (int row = 0; row < 78; row++) {
				if ((col < 8 && (row < 8 || row > 69)) || (col > 69 && row < 8)
					|| (col > 67 && col < 75 && row>67 && row < 75)) {
					continue;
				}
				else {
					if (count == 0)
						data +='\0';
					Scalar color = img[i].at<uchar>(Point(col * 10 + 5, row * 10 + 5));
					if (color[0] == 255) {
						data[key] += '\0';
					}
					else {
						data[key] += 1*pow(2,count);
					}
					count++;
					if (count == 8) {
						if (flag > 0 && data[key] != '\0')
							flag--;
						if (data[key] == '\0')
							flag++;
						if(flag==2)
							return;
						count = 0;
						key++;
					}
				}
			}
		}
	}
	//cout << data << endl;
}
void WriteBinFile(string& str, const string fileName) {
	ofstream outFile;
	outFile.open(fileName, ios::binary | ios::out);
	str.erase(str.end() - 1);
	str.erase(str.end() - 1);
	if (!outFile) {
		cout << "Unable to open file!" << endl;
		exit(0);
	}
	outFile << str;
}