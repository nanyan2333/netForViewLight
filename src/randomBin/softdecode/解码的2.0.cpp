#define _CRT_SECURE_NO_WARNINGS
#include <opencv2/core/core.hpp>  
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<iostream>
#include<string>
#include<vector>
#include <fstream>
using namespace cv;
using namespace std;

int picture_to_bin(char* out_bin_path)
{
	// ����ͼ��
	string path = "*.jpg"; // ���Ը�����Ҫ�޸��ļ���׺
	vector<String> images;

	// ʹ��cv::glob��ȡƥ����ļ�·��
	glob(path, images);

	// ����������ļ���
	for (size_t i = 0; i < images.size(); i++) {
		Mat image = imread(images[i], IMREAD_COLOR);

		if (image.empty()) {
			cerr << "ͼ�����ʧ�ܣ�" << endl;
			return -1;
		}

		// ����һ�����ڴ洢�����Ķ��������ݵ�����
		vector<uchar> buffer;

		// ��ͼ�����ΪJPG��ʽ�Ķ���������
		vector<int> params;
		params.push_back(IMWRITE_JPEG_QUALITY);
		params.push_back(95); // ����JPEG����
		imencode(".jpg", image, buffer, params);

		// ������������д���ļ�
		ofstream file(out_bin_path,ios:: binary|ios::app);
		if (file.is_open()) {
			file.write((const char*)(buffer.data()), buffer.size());
			file.close();
		}
		else {
			cerr << "�޷����ļ���" << endl;
			return -1;
		}
	}

	return 0;
}

void video_to_pictures(char* video_path)
{
	//��VideoCaptureʵ�����ٳ�ʼ��
	//VideoCapture capture;
	//capture.open("Megamind.avi");
	//��VideoCaptureʵ������ͬʱ��ʼ��
	VideoCapture capture(video_path);
	if (!capture.isOpened())
	{
		return;
	}
	int frameH = capture.get(CAP_PROP_FRAME_HEIGHT);
	int frameW = capture.get(CAP_PROP_FRAME_WIDTH);
	int fps = capture.get(CAP_PROP_FPS);
	long numFrames = capture.get(CAP_PROP_FRAME_COUNT);
	printf("vedio's width = %d\t height = %d\nvideo's fps = %d\t nums = %d", frameW, frameH, fps, numFrames);

	int imgIndex = 10;
	Mat frame;
	int frameRate =fps;
	while (imgIndex<=numFrames)
	{
		capture >> frame;
		if (frame.empty())
		{
			break;
		}
		if (imgIndex % frameRate == 0)
		{
			//char* imageSaveName = new char[200];
			char imageSaveName[256];
			//��ͼƬ��·�����������뵽imageSaveName��
			sprintf_s(imageSaveName, "%05d֡.jpg", imgIndex);

			//��ÿһ֡ͼ��frame���浽imageSaveNameָ�����ļ���
			imwrite(imageSaveName, frame);
		}
		//delete[] imageSaveName;
		imgIndex++;
	}
	capture.release();
}

int main()
{
	char video_path[] = "zc.mp4";
	char out_bin_path[] = "out.bin";
	video_to_pictures(video_path);
	picture_to_bin(out_bin_path);
	return 0;
}