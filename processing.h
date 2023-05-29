#ifndef PROCESSING_H
#define PROCESSING_H


#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QThread>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <opencv2/objdetect.hpp>    //HOG

using namespace cv;

class Processing
{
public:
    Processing()=default;
    Processing(QString file_path);
    Processing(QString file_path,int argu_i);
    Processing(QString file_path,double argu_d);
    
    QImage mat2qimage(const Mat& mat);
    Mat qimage2mat(const QImage& qimage);
    
    QImage My_read();
    QImage My_gray();
    QImage My_blur();
    QImage My_edges();
    QImage My_gamma();
    QImage My_contrast();
    QImage My_saturation();
    QImage My_calcHist();
    QImage My_Otsu();
    
    
    Mat Get_img_1(){return img_1;}
   
private:
    QString file_path;
    int argu_i = 1;
    double argu_d = 1.0;
    Mat img_0;
    Mat img_1;
    QVector<QRgb> grayColorTable()
    {
        qDebug()<<"graycolorTable";
        QVector<QRgb> table;
        for (int i = 0; i < 256; ++i) {
            table.push_back(qRgb(i, i, i));
        }
        return table;
    }
    Mat Open_read();    //读取
    Mat Open_gray();    //灰度化
    Mat Open_blur();    //均值滤波
    Mat Open_edges();   //边缘检测
    Mat Open_gamma();   //伽马变换（亮度调整）
    Mat Open_contrast();    //对比度调整
    Mat Open_saturation();  //饱和度调整
    Mat Open_calcHist();    //灰度直方图
    Mat Open_Otsu();        //二值化（自动阈值选取--基于灰度直方图）
};

#endif // PROCESSING_H
