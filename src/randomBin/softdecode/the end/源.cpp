#include "parsingQRcodes.h"

//����ͼ����Ϊ����������ͼ����С���ο�
cv::Rect getMinRectBound(const cv::Mat& img) {
    CV_Assert(img.type() == CV_8UC1);  // ȷ��ͼ���ǵ�ͨ���Ҷ�ͼ

    bool key = false;
    int rows = img.rows, cols = img.cols;
    cv::Point topRight(0, rows);//���ϱߡ����ұ�
    cv::Point bottomLeft(cols, 0);//����ߡ����±�

    for (int i = 0; i < rows; ++i) {
        const uchar* row = img.ptr<uchar>(i);
        for (int j = 0; j < cols; ++j) {
            if (row[j] == 0) {  // ��ɫ���ݣ���ɫ����
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
    //�ҶȻ�����ֵ����
    cvtColor(img, gray, COLOR_BGR2GRAY);
    for (int i = 0; i < 3; i++) {
        //��˹ģ��,ѭ����ǿƽ��Ч��
        cv::GaussianBlur(gray, gray, Size(3, 3), 0);
    }
    threshold(gray, gray, 127, 255, THRESH_BINARY);

    // ��ȡ��С���α߿�
    cv::Rect rect = getMinRectBound(gray);

    // ����һ���µĿհ׻�������С�����Զ���  
    cv::Mat qrcode = cv::Mat::zeros(cv::Size(rect.width, rect.height), CV_8UC1); // ����һ��780x780��С�Ŀհ׻�����1ͨ�����Ҷ�ͼ��  

    // ���ý�ȡ�������»����ϵ�λ�� 
    cv::Rect roiInqrcode(0, 0, rect.width, rect.height);

    // ��ԭͼ�Ľ�ȡ�����Ƶ��»�����  
    gray(rect).copyTo(qrcode(roiInqrcode));

    // �����ȡ������
    QRcodes.push_back(qrcode);
    
}