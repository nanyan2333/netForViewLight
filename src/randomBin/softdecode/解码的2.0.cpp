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
	// 加载图像
	string path = "*.jpg"; // 可以根据需要修改文件后缀
	vector<String> images;

	// 使用cv::glob获取匹配的文件路径
	glob(path, images);

	// 遍历并输出文件名
	for (size_t i = 0; i < images.size(); i++) {
		Mat image = imread(images[i], IMREAD_COLOR);

		if (image.empty()) {
			cerr << "图像加载失败！" << endl;
			return -1;
		}

		// 创建一个用于存储编码后的二进制数据的向量
		vector<uchar> buffer;

		// 将图像编码为JPG格式的二进制数据
		vector<int> params;
		params.push_back(IMWRITE_JPEG_QUALITY);
		params.push_back(95); // 设置JPEG质量
		imencode(".jpg", image, buffer, params);

		// 将二进制数据写入文件
		ofstream file(out_bin_path,ios:: binary|ios::app);
		if (file.is_open()) {
			file.write((const char*)(buffer.data()), buffer.size());
			file.close();
		}
		else {
			cerr << "无法打开文件！" << endl;
			return -1;
		}
	}

	return 0;
}

void video_to_pictures(char* video_path)
{
	//类VideoCapture实例化再初始化
	//VideoCapture capture;
	//capture.open("Megamind.avi");
	//类VideoCapture实例化的同时初始化
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
			//将图片的路径及名称输入到imageSaveName中
			sprintf_s(imageSaveName, "%05d帧.jpg", imgIndex);

			//将每一帧图像frame保存到imageSaveName指定的文件中
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