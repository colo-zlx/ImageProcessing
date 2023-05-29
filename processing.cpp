#include "processing.h"

#include <opencv2/objdetect.hpp>



//构造函数
Processing::Processing(QString file_path):
    file_path(file_path)
{
    img_0 = imread(this->file_path.
                   toLocal8Bit().   //解决imread()中文路径错误
                   toStdString(),0);
    img_1 = imread(this->file_path.
                   toLocal8Bit().
                   toStdString(),1);
    if(img_0.empty() || img_1.empty())
    {
        qDebug()<<"img read fail!";
    }
    if(!img_0.empty() && !img_1.empty())
    {
        qDebug()<<"img read success!";
    }
}

//构造函数
Processing::Processing(QString file_path, int argu_i):
    file_path(file_path),argu_i(argu_i)
{
    img_0 = imread(this->file_path.
                   toLocal8Bit().   //解决imread()中文路径错误
                   toStdString(),0);
    img_1 = imread(this->file_path.
                   toLocal8Bit().
                   toStdString(),1);
    if(img_0.empty() || img_1.empty())
    {
        qDebug()<<"img read fail!";
    }
    if(!img_0.empty() && !img_1.empty())
    {
        qDebug()<<"img read success!";
    }
}

//构造函数
Processing::Processing(QString file_path, double argu_d):
    file_path(file_path),argu_d(argu_d)
{
    img_0 = imread(this->file_path.
                   toLocal8Bit().   //解决imread()中文路径错误
                   toStdString(),0);
    img_1 = imread(this->file_path.
                   toLocal8Bit().
                   toStdString(),1);
    if(img_0.empty() || img_1.empty())
    {
        qDebug()<<"img read fail!";
    }
    if(!img_0.empty() && !img_1.empty())
    {
        qDebug()<<"img read success!";
    }
}

//Mat——to——QImage 转换
QImage Processing::mat2qimage(const Mat &mat)
{
    QImage qimage;
        if (mat.type() == CV_8UC1) { // 8位灰度图像
            qimage = QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
            //qimage.setColorTable(grayColorTable()); // 设置灰度颜色表
            qDebug()<<"8";
        } else if (mat.type() == CV_8UC3) { // 24位RGB图像
            qimage = QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
            qDebug()<<"24";
        } else if (mat.type() == CV_8UC4) { // 32位ARGB或RGBA图像
            qimage = QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
            qDebug()<<"32";
        } else {
            qWarning("Unsupported mat type %d, unable to convert to QImage.", mat.type());
            return QImage();
        }
        return qimage;
}

//QImage——to——Mat 转换
Mat Processing::qimage2mat(const QImage &qimage)
{
    Mat mat;
    QImage image;
    image =qimage.convertToFormat(QImage::Format_ARGB32); //转换存储图像的数据格式
       switch (qimage.format()) {
       case QImage::Format_Indexed8: // 8位灰度图像
           mat = Mat(image.height(), image.width(), CV_8UC1, const_cast<uchar*>(image.bits()), image.bytesPerLine());
           break;
       case QImage::Format_RGB888: // 24位RGB图像
           mat = Mat(image.height(), image.width(), CV_8UC3, const_cast<uchar*>(image.bits()), image.bytesPerLine());
           cvtColor(mat, mat, COLOR_RGB2BGR); // 转换为BGR格式
           break;
       case QImage::Format_ARGB32:
       case QImage::Format_RGBA8888: // 32位ARGB或RGBA图像
           mat = Mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine());
           cvtColor(mat, mat, COLOR_RGBA2BGRA); // 转换为BGRA格式
           break;
       default:
           qWarning("Unsupported qimage format %d, unable to convert to Mat.", image.format());
           return Mat();
       }
       return mat;
}

QImage Processing::My_read()
{
    Mat mat_read = Open_read();
    cvtColor(mat_read, mat_read, COLOR_BGR2RGB);
    QImage qimage = mat2qimage(mat_read);
    return qimage;
}

QImage Processing::My_gray()
{
    QImage qimage;
    Mat mat_gray = Open_gray();
    qimage = QImage(mat_gray.data, mat_gray.cols, mat_gray.rows, mat_gray.step, QImage::Format_Grayscale8);
    return qimage;
}

QImage Processing::My_blur()
{
    QImage qimage;
    Mat mat_blur = Open_blur();
    cvtColor(mat_blur, mat_blur, COLOR_BGR2RGB);
    qimage = mat2qimage(mat_blur);
    return qimage;
}

QImage Processing::My_edges()
{
    QImage qimage; 
    Mat mat_edges = Open_edges();
    if(mat_edges.empty())
    {
        qDebug()<<"edges is null";
    }
    qimage = QImage(mat_edges.data, mat_edges.cols, mat_edges.rows, mat_edges.step, QImage::Format_Grayscale8);
    return qimage;
}

QImage Processing::My_gamma()
{
    Mat mat_gamma = Open_gamma();
    cvtColor(mat_gamma, mat_gamma, COLOR_BGR2RGB);
    QImage qimage;
    qimage = QImage(mat_gamma.data, mat_gamma.cols, mat_gamma.rows, mat_gamma.step, QImage::Format_RGB888);
    QImage img = qimage.copy();
    return img;
    
}

QImage Processing::My_contrast()
{
    Mat mat_contrast = Open_contrast();
    cvtColor(mat_contrast, mat_contrast, cv::COLOR_BGR2RGB);
    QImage qimage = mat2qimage(mat_contrast);
    QImage img = qimage.copy();
    return img;
}

QImage Processing::My_saturation()
{
    Mat mat_saturation = Open_saturation();
    cvtColor(mat_saturation, mat_saturation, cv::COLOR_BGR2RGB);
    QImage qimage = mat2qimage(mat_saturation);
    QImage img = qimage.copy();
    return img;
}

QImage Processing::My_calcHist()
{
    Mat hist = Open_calcHist();
    int histWidth = 256;   // 直方图宽度
    int histHeight = 150;  // 直方图高度
    int histSize = 256;  // 灰度级数
    QImage qimage(histWidth, histHeight, QImage::Format_RGB32);
    qimage.fill(Qt::white);
    
    QPainter painter(&qimage);
    painter.setPen(Qt::black);
    
    for (int i = 0; i < histSize - 1; i++) {
        // 计算直方图中每个灰度级的高度
        int binHeight = cvRound(hist.at<float>(i) * histHeight / 255);
    
        // 绘制直方图
        painter.drawLine(i, histHeight - binHeight, i, histHeight - 1);
    }
    return qimage;
}

QImage Processing::My_Otsu()
{
    Mat mat_Otus = Open_Otsu();
    QImage qimage = mat2qimage(mat_Otus);
    QImage img = qimage.copy();
    //qimage.save("D://_Resource//picture//test.png");
    return img;
}

Mat Processing::Open_read()
{
    return img_1;
}

Mat Processing::Open_gray()
{
    Mat img_gray = img_0;
    if(img_gray.empty())
    {
        qDebug() << "gray read faild";
    }
    return img_gray;
}

Mat Processing::Open_blur()
{
    Mat img_blur;
    img_blur = img_1;
    if(img_blur.empty())
    {
        qDebug() << "blur read faild";
    }
    GaussianBlur(img_blur, img_blur, cv::Size(argu_i, argu_i), 0, 0);
    return img_blur;
}

Mat Processing::Open_edges()
{
    qDebug()<<"edge start";
    Mat edges;
    edges = img_0;
    Sobel(edges, edges, CV_8U, 1, 1, argu_i);
    if(edges.empty())
    {
        qDebug() << "edges read faild";
    }
    qDebug()<<"edge end";
    return edges;
}

Mat Processing::Open_gamma()
{
    Mat img_gamma = img_1;
    Mat gammaImage;
    img_gamma.convertTo(img_gamma, CV_32F);
    double gamma = argu_d;
    pow(img_gamma,gamma,gammaImage);
    Mat img_out;
    gammaImage.convertTo(img_out, CV_8U);
    return img_out;
}

Mat Processing::Open_contrast()
{
    Mat inputImage = img_1;
    double contrast;
    contrast = argu_d;
    Mat outputImage;
    inputImage.convertTo(outputImage, -1 , contrast ,0);
    return outputImage;
}

Mat Processing::Open_saturation()
{
    int saturation_factor = argu_i;
    Mat img_saturation = img_1;
    Mat hsv_image;
    cvtColor(img_saturation, hsv_image, cv::COLOR_BGR2HSV);
    std::vector<cv::Mat> channels;
    split(hsv_image, channels);
    // 对饱和度通道进行线性缩放
    channels[1] = channels[1] * saturation_factor / 100.0;
    // 将饱和度通道值限制在 [0, 255] 范围内
    cv::threshold(channels[1], channels[1], 255, 255, cv::THRESH_TRUNC);
    cv::threshold(channels[1], channels[1], 0, 0, cv::THRESH_TOZERO);
    
    cv::merge(channels, hsv_image);
    cv::Mat output_image;
    cv::cvtColor(hsv_image, output_image, cv::COLOR_HSV2BGR);
    
    return output_image;
}

Mat Processing::Open_calcHist()
{
    Mat image = img_0;
    cv::Mat hist;
    int histSize = 256;  // 灰度级数
    float range[] = {0, 256};
    const float* histRange = {range};
    cv::calcHist(&image, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, true, false);
    
    // 归一化处理
    cv::normalize(hist, hist, 0, 255, cv::NORM_MINMAX, -1, cv::Mat());
    return hist;
}

Mat Processing::Open_Otsu()
{
    Mat img_gray = img_0;
    if(img_gray.empty())
    {
        qDebug() << "gray read faild";
    }
    Mat binaryImage;
    //double thresholdValue = threshold(img_gray, binaryImage, 0, 255, THRESH_BINARY | THRESH_OTSU);  接收返回值
    threshold(img_gray, binaryImage, 0, 255, THRESH_BINARY | THRESH_OTSU);      //不接收返回值
    return binaryImage;
}


