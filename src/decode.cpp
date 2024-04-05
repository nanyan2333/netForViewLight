#include"decode.h"
void VideoToPic(const string fileName, vector<Mat>& img) {
	VideoCapture video(fileName);
	int frame_num = video.get(CAP_PROP_FRAME_COUNT);
	double fps = video.get(CAP_PROP_FPS);
	cout << fps << endl;

	int rate = 3;

	for (int i = 1; i <= frame_num; i++) {
		Mat frame;
		video >> frame;
		if (i % rate == 0 && !frame.empty()) {
			img.push_back(frame);
		}
	}
}
void QRCodeToData(vector<Mat>& img, string& data) {

	//int page = -1;
	int count = 0;
	int count1 = 0;
	int key = 0;
	int page[1000] = { 0 };
	int tmp = 0;
	int flag = 0;
	string ta[1000];
	for (int i = 0; i < img.size(); i++) {
		for (int col = 0; col < 78; col++) {
			for (int row = 0; row < 78; row++) {
				if (col == 0 && row == 16)
				{
					if (page[tmp] != 0||tmp==0)
					{
						flag = 1;
						tmp = 0;
						count1 = 0;
						break;
					}
					else
						page[tmp]++;
					count1 = 0;
				}
				if ((col < 8 && (row < 8 || row > 69)) || (col > 69 && row < 8)
					|| (col > 67 && col < 75 && row>67 && row < 75)||(col==77&&row>66) ){
					continue;
				}
				else if (col == 0 && row >= 8 && row < 16) {
					Scalar color = img[i].at<uchar>(Point(col * 10 + 5, row * 10 + 5));
					if (color[0] < 128)
						tmp += 1 * pow(2, count1);
					count1++;
				}
				else {
					if (count == 0)
						ta[tmp] += '\0';
					Scalar color = img[i].at<uchar>(Point(col * 10 + 5, row * 10 + 5));
					if (color[0] > 128) {
						ta[tmp][key] += '\0';
					}
					else {
						ta[tmp][key] += 1 * pow(2, count);
					}
					count++;
					if (count == 8) {
						count = 0;
						key++;
					}
				}
			}
			if (flag == 1)
			{
				flag = 0;
				break;
			}
		}
		tmp = 0;
		key = 0;
	}
	for (int i = 1; i < img.size(); i++)
	{
		if (ta[i].empty())
			for (int j = 0; j < 728; j++)
			{
				if (j % 13 == 0&&j!=0)
					data += 1;
				else
					data += '\0';
			}
		else
			data += ta[i];
	}
}
void WriteBinFile(string& str, const string fileName) {
	ofstream outFile;
	outFile.open(fileName, ios::binary | ios::out);
	if (!outFile) {
		cout << "Unable to open file!" << endl;
		exit(0);
	}
	outFile << str;
}
