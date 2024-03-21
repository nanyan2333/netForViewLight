#pragma once
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <fstream>

using namespace cv;
using namespace std;

void ReadBinFile(string& str, const string fileName);

void DrawQRCode(string code, vector<Mat>& img);

void PicToVideo(vector<Mat>& img, const string fileName);