#include "identifyQRcode.h"

float GetAngle(Point2f v1, Point2f v2) {
	float mod1 = sqrt(v1.x * v1.x + v1.y * v1.y);
	float mod2 = sqrt(v2.x * v2.x + v2.y * v2.y);
	float angle = acos((v1.x * v2.x + v1.y * v2.y) / (mod1 * mod2)) * 180.0 / CV_PI;
	return angle;
}

void IdentifyQRcode(Mat img, vector<Mat>& QRcodes) {
	Mat midImg;
	cvtColor(img, midImg, COLOR_BGR2GRAY);

	for (int i = 0; i < 3; i++) {
		GaussianBlur(midImg, midImg, Size(3, 3), 0);
	}
	threshold(midImg, midImg, 127, 255, THRESH_BINARY);
	//imwrite("./pic/" + to_string(QRcodes.size() - 1) + ".jpg", midImg);

	//查找轮廓之间的层级关系
	vector<vector<Point>>contours;
	vector<Vec4i>hierarchy;
	findContours(midImg, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);

	//父轮廓索引
	int ParentIndex = -1;
	int mark = 0;

	vector<Point2f>rect_points;
	double minRect = -1;
	int minRectIdx = -1;
	//Mat canva = Mat::zeros(midImg.size(), midImg.type());
	for (int j = 0; j < contours.size(); j++) {
		if (contourArea(contours[j]) > 50000) {
			continue;
		}
		if (hierarchy[j][2] != -1 && mark == 0) {
			ParentIndex = j;
			mark++;
		}
		else if (hierarchy[j][2] != -1 && mark == 1) {
			mark++;
		}
		else if (hierarchy[j][2] == -1) {
			ParentIndex = -1;
			mark = 0;
		}

		if (hierarchy[j][2] != -1 && mark == 2) {
			RotatedRect rect = minAreaRect(contours[ParentIndex]);
			rect_points.push_back(rect.center);

			if (minRect == -1) {
				minRect = contourArea(contours[ParentIndex]);
				minRectIdx = rect_points.size() - 1;
			}
			else {
				if (contourArea(contours[ParentIndex]) < minRect) {
					minRect = contourArea(contours[ParentIndex]);
					minRectIdx = rect_points.size() - 1;
				}
			}

			ParentIndex = -1;
			mark = 0;
			//drawContours(canva, contours, ParentIndex, Scalar::all(255), -1);
			//imwrite("canva_1.png", canva);
		}
	}
	//cout << minRectIdx << "\t" << rect_points.size() << endl;
	if (rect_points.size() < 4) {	//因为帧数足够多，可以舍弃一些识别模糊的帧
		return;
	}

	Point2f P1[4];
	P1[3] = Point2f(rect_points[minRectIdx].x, rect_points[minRectIdx].y);
	for (int i = minRectIdx; i < rect_points.size() - 1; i++) {
		rect_points[i] = rect_points[i + 1];
	}
	rect_points.pop_back();

	float angle1 = GetAngle(Point2f(rect_points[1].x - rect_points[0].x, rect_points[1].y - rect_points[0].y),
		Point2f(rect_points[2].x - rect_points[0].x, rect_points[2].y - rect_points[0].y));
	float angle2 = GetAngle(Point2f(rect_points[0].x - rect_points[1].x, rect_points[0].y - rect_points[1].y),
		Point2f(rect_points[2].x - rect_points[1].x, rect_points[2].y - rect_points[1].y));
	float angle3 = GetAngle(Point2f(rect_points[0].x - rect_points[2].x, rect_points[0].y - rect_points[2].y),
		Point2f(rect_points[1].x - rect_points[2].x, rect_points[1].y - rect_points[2].y));

	//cout << angle1 << '\t' << angle2 << '\t' << angle3 << endl;

	mark = -1;
	if (angle1 > angle2 && angle1 > angle3) {
		P1[0] = Point2f(rect_points[0].x, rect_points[0].y);
		mark = 0;
		//cout << "angle1 " << angle1 << endl;
	}
	else if (angle2 > angle1 && angle2 > angle3) {
		P1[0] = Point2f(rect_points[1].x, rect_points[1].y);
		mark = 1;
		//cout << "angle2 " << angle2 << endl;
	}
	else if (angle3 > angle1 && angle3 > angle2) {
		P1[0] = Point2f(rect_points[2].x, rect_points[2].y);
		mark = 2;
		//cout << "angle3 " << angle3 << endl;
	}

	Point2f midPoint = Point2f((rect_points[(mark + 1) % rect_points.size()].x + rect_points[(mark + 2) % rect_points.size()].x) / 2,
		(rect_points[(mark + 1) % rect_points.size()].y + rect_points[(mark + 2) % rect_points.size()].y) / 2);

	float tempX = rect_points[(mark + 1) % rect_points.size()].x - P1[0].x;
	float tempY = rect_points[(mark + 1) % rect_points.size()].y - P1[0].y;
	float x1 = (tempX) / sqrt(tempX * tempX + tempY * tempY);
	float y1 = (tempY) / sqrt(tempX * tempX + tempY * tempY);

	tempX = rect_points[(mark + 2) % rect_points.size()].x - P1[0].x;
	tempY = rect_points[(mark + 2) % rect_points.size()].y - P1[0].y;
	float x2 = (tempX) / sqrt(tempX * tempX + tempY * tempY);
	float y2 = (tempY) / sqrt(tempX * tempX + tempY * tempY);

	if (P1[0].x <= midPoint.x) {
		if (y1 < y2) {
			P1[1] = Point2f(rect_points[(mark + 1) % rect_points.size()].x, rect_points[(mark + 1) % rect_points.size()].y);
			P1[2] = Point2f(rect_points[(mark + 2) % rect_points.size()].x, rect_points[(mark + 2) % rect_points.size()].y);
		}
		else if (y1 > y2) {
			P1[1] = Point2f(rect_points[(mark + 2) % rect_points.size()].x, rect_points[(mark + 2) % rect_points.size()].y);
			P1[2] = Point2f(rect_points[(mark + 1) % rect_points.size()].x, rect_points[(mark + 1) % rect_points.size()].y);
		}
		else if (y1 == y2) {
			if (P1[0].y > midPoint.y) {
				if (x1 < x2) {
					P1[1] = Point2f(rect_points[(mark + 1) % rect_points.size()].x, rect_points[(mark + 1) % rect_points.size()].y);
					P1[2] = Point2f(rect_points[(mark + 2) % rect_points.size()].x, rect_points[(mark + 2) % rect_points.size()].y);
				}
				else {
					P1[1] = Point2f(rect_points[(mark + 2) % rect_points.size()].x, rect_points[(mark + 2) % rect_points.size()].y);
					P1[2] = Point2f(rect_points[(mark + 1) % rect_points.size()].x, rect_points[(mark + 1) % rect_points.size()].y);
				}
			}
		}
	}
	else {
		if (y1 > y2) {
			P1[1] = Point2f(rect_points[(mark + 1) % rect_points.size()].x, rect_points[(mark + 1) % rect_points.size()].y);
			P1[2] = Point2f(rect_points[(mark + 2) % rect_points.size()].x, rect_points[(mark + 2) % rect_points.size()].y);
		}
		else if (y1 < y2) {
			P1[1] = Point2f(rect_points[(mark + 2) % rect_points.size()].x, rect_points[(mark + 2) % rect_points.size()].y);
			P1[2] = Point2f(rect_points[(mark + 1) % rect_points.size()].x, rect_points[(mark + 1) % rect_points.size()].y);
		}
	}

	//for (int i = 0; i < 3; i++) {
	//	cout << P1[i].x << ',' << P1[i].y << '\t';
	//}
	//cout << endl;

	Point2f P2[4]{
		Point2f(35, 35), Point2f(745, 35), Point2f(35, 745), Point2f(715, 715)
	};

	Mat dstImg;
	Mat matrix = getPerspectiveTransform(P1, P2);
	warpPerspective(midImg, dstImg, matrix, Size(780, 780), 1, 0, Scalar(255, 255, 255));
	QRcodes.push_back(dstImg);
}