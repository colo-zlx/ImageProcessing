#include "mythread.h"

MyThread::MyThread(QObject *parent)
    : QThread{parent}
{
    
}

void MyThread::Set_data(Data data)
{   
    this->data = data;
    qDebug()<<this->data.file_path;
}

void MyThread::run()
{
    QImage* img = new QImage();
    QImage qimage;
    //读取图片
    if(this->data.open_type == 0)
    {
        Processing pro(this->data.file_path);
        *img = pro.My_read();
        if(img->isNull())
        {
            qDebug()<<"img is null";
        }
        qDebug()<<"img pro end";
        qimage = img->copy();
    }
    //均值滤波
    else if(this->data.open_type == 1)
    {
        Processing pro(this->data.file_path , this->data.argu_i);
        *img = pro.My_blur();
        qimage = img->copy();
    }
    //灰度化
    else if(this->data.open_type == 2)
    {
        Processing pro(this->data.file_path);
        *img = pro.My_gray();
        qimage = img->copy();
    }
    //边缘检测
    else if(this->data.open_type == 3)
    {
        Processing pro(this->data.file_path , this->data.argu_i);
        *img = pro.My_edges();
        qimage = img->copy();
    }
    //伽马变换
    else if(this->data.open_type == 4)
    {
        Processing pro(this->data.file_path , this->data.argu_d);
        *img = pro.My_gamma();
        qimage = img->copy();
    }
    //对比度调整
    else if(this->data.open_type == 5)
    {        
        Processing pro(this->data.file_path , this->data.argu_d);
        *img = pro.My_contrast();
        qimage = img->copy();
    }
    //饱和度调整
    else if(this->data.open_type == 6)
    {        
        Processing pro(this->data.file_path , this->data.argu_i);
        *img = pro.My_saturation();
        qimage = img->copy();
    }
    //灰度直方图
    else if(this->data.open_type == 7)
    {
        Processing pro(this->data.file_path);
        *img = pro.My_calcHist();
        qimage = img->copy();
    }
    //二值化
    else if(this->data.open_type == 8)
    {
        Processing pro(this->data.file_path);
        *img = pro.My_Otsu();
        qimage = img->copy();
    }
    
    delete img;
    emit Open_finish(qimage);
    qDebug()<<"run end";
}

