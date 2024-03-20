#include "parsingQRcodes.h"

//接收图像作为参数，返回图像最小矩形框
cv::Rect getMinRectBound(const cv::Mat& img) {
    CV_Assert(img.type() == CV_8UC1);  // 确保图像是单通道灰度图

    bool key = false;
    int rows = img.rows, cols = img.cols;
    cv::Point topRight(0, rows);//最上边、最右边
    cv::Point bottomLeft(cols, 0);//最左边、最下边

    for (int i = 0; i < rows; ++i) {
        const uchar* row = img.ptr<uchar>(i);
        for (int j = 0; j < cols; ++j) {
            if (row[j] == 0) {  // 黑色内容，白色背景
                topRight.x = std::max(topRight.x, j);
                topRight.y = std::min(topRight.y, i);
                bottomLeft.x = std::min(bottomLeft.x, j);
                bottomLeft.y = std::max(bottomLeft.y, i);
            }
        }
    }
    return cv::Rect(bottomLeft.x, topRight.y, topRight.x - bottomLeft.x, bottomLeft.y - topRight.y);
}

void parsingQRcodes(Mat img, vector<Mat>& QRcodes) {
    cv::Mat gray;
    //灰度化、二值化：
    cvtColor(img, gray, COLOR_BGR2GRAY);
    for (int i = 0; i < 3; i++) {
        //高斯模糊,循环增强平滑效果
        cv::GaussianBlur(gray, gray, Size(3, 3), 0);
    }
    threshold(gray, gray, 127, 255, THRESH_BINARY);

    // 获取最小矩形边框
    cv::Rect rect = getMinRectBound(gray);

    // 创建一个新的空白画布，大小可以自定义  
    cv::Mat qrcode = cv::Mat::zeros(cv::Size(rect.width, rect.height), CV_8UC1); // 创建一个780x780大小的空白画布，1通道（灰度图）  

    // 设置截取区域在新画布上的位置 
    cv::Rect roiInqrcode(0, 0, rect.width, rect.height);

    // 将原图的截取区域复制到新画布上  
    gray(rect).copyTo(qrcode(roiInqrcode));

    // 保存截取的区域
    QRcodes.push_back(qrcode);
}