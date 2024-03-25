#pragma once
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <fstream>

using namespace cv;
using namespace std;

void VideoToPic(const string fileName, vector<Mat>& img);

void QRCodeToData(vector<Mat>& img, string& data);

void WriteBinFile(string& str, const string fileName);