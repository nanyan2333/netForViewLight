#pragma once
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <fstream>

using namespace std;
using namespace cv;

void parsingQRcodes(Mat img, vector<Mat>& QRcodes);