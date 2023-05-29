#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QApplication::setWindowIcon(QIcon("://image//sofware_icon-1.png"));
    setWindowTitle("ImageProcessing");
    Init_widget();
    Init_styleSheet();
    Init_signals_slots();       //初始化信号和槽
    
    
    MyThread* my_thread = new MyThread;     //创建子线程
    
    connect(shortcut1, SIGNAL(activated()), ui->btn_sub, SIGNAL(clicked()));
    connect(shortcut2, SIGNAL(activated()), ui->btn_sub, SIGNAL(clicked()));
    
    connect(this,&MainWindow::Send_data,my_thread,&MyThread::Set_data); //链接主线程信号和子线程槽
    
    connect(ui->btn_sub,&QPushButton::clicked,this,[=](){       //链接控件信号和触发主程序信号 启动子线程
        if(this->data.open_type == 1)
        {
            this->data.argu_i=ui->spinBox->value();
            if(this->data.argu_i %2 == 0 )  //自动将偶数加1
            {
                ++this->data.argu_i;
            }
        }
        else if(this->data.open_type == 3)
        {
            this->data.argu_i=ui->spinBox->value();
            if(this->data.argu_i %2 == 0 )
            {
                ++this->data.argu_i;
            }
        }
        else if(this->data.open_type == 4)
        {
            this->data.argu_d = ui->doubleSpinBox->value()/100;
        }
        else if(this->data.open_type == 5)
        {
            this->data.argu_d = ui->doubleSpinBox->value()/100;
        }
        else if(this->data.open_type == 6)
        {
            this->data.argu_i = ui->spinBox->value();
        }
        ui->btn_effect->setEnabled(true);
        emit Send_data(data);
        my_thread->start();
    });
    connect(my_thread,&MyThread::Open_finish,this,&MainWindow::Updata_img);     //链接子线程信号和主线程槽
}


MainWindow::~MainWindow()
{
    delete ui;
}
//展示处理后的图像
void MainWindow::Updata_img(QImage img)
{
    this->qimage = img;
    if(qimage.isNull())
    {
        qDebug()<<"qimage is null";
    }
    ui->label_img1->setPixmap(QPixmap::fromImage(this->qimage).scaled(width(),height(),Qt::KeepAspectRatio));
    ui->stackedWidget->setCurrentWidget(ui->page_sw1);
}

/********初始化信号和槽********/
void MainWindow::Init_signals_slots()
{
    connect(ui->btn_open,&QPushButton::clicked,this,[&](){
        QString defaultDir = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);        //用户图片文件夹路径/
        QString file_name = QFileDialog::getOpenFileName(this, tr("Open Image"), defaultDir, tr("Image Files (*.png *.jpg)"));      //文件路径
        QImage img(file_name);
        if(file_name.isEmpty())     //判断文件名路径是否为空
        {
            return;
        }
        if(img.isNull())        //判断文件是否有效
        {
            QMessageBox::critical(nullptr, "Error", "目标文件错误！");
            return;
        }
        this->qimage = QImage(file_name);
        ui->stackedWidget->setCurrentWidget(ui->page_sw1);      //切换stackedWidget
        ui->label_img1->setPixmap(QPixmap::fromImage(QImage(file_name).scaled(width(),height(),Qt::KeepAspectRatio)));
        ui->label_img2->setPixmap(QPixmap::fromImage(QImage(file_name).scaled(width(),height(),Qt::KeepAspectRatio)));
        this->data.file_path = file_name;
        ui->btn_original->setEnabled(true);     //启用按钮btn_original
    });
    connect(ui->btn_original,&QPushButton::clicked,this,[&](){
        ui->stackedWidget->setCurrentWidget(ui->page_sw3);
    });
    connect(ui->btn_effect,&QPushButton::clicked,this,[&](){
        ui->stackedWidget->setCurrentWidget(ui->page_sw1);
    });

/*图像处理按钮相关信号和槽*/
    //均值滤波
    connect(ui->btn_blur,&QPushButton::clicked,this,[&](){
        if(!this->data.file_path.isNull())
        {
            ui->spinBox->show();
            ui->spinBox->setSingleStep(2);
            ui->spinBox->setMinimum(1);
            ui->spinBox->setMaximum(999);
            ui->doubleSpinBox->hide();
            ui->label_arguments->show();
            ui->label_arguments->setText("请输入奇数");
            ui->label_information->setText("当前选择: 均值滤波--GaussianBlur");
            this->data.open_type=1;
            ui->btn_sub->setEnabled(true);
        }
    });
    //灰度化
    connect(ui->btn_gray,&QPushButton::clicked,this,[&](){
        if(!this->data.file_path.isNull())
        {
            ui->spinBox->hide();
            ui->doubleSpinBox->hide();
            ui->label_arguments->hide();
            ui->label_information->setText("当前选择: 灰度化--cv::IMREAD_GRAYSCALE");
            this->data.open_type=2;
            ui->btn_sub->setEnabled(true);
        }
    });
    //边缘检测
    connect(ui->btn_edges,&QPushButton::clicked,this,[&](){
        if(!this->data.file_path.isNull())
        {
            ui->doubleSpinBox->hide();
            ui->label_arguments->show();
            ui->spinBox->show();
            ui->spinBox->setMaximum(7);
            ui->spinBox->setMinimum(3);
            ui->spinBox->setValue(5);
            ui->label_arguments->setText("算子大小:3|5|7");
            ui->label_information->setText("当前选择: 边缘检测--Sobel");
            this->data.open_type=3;
            ui->btn_sub->setEnabled(true);
        }
    });
    //伽马变换--亮度调节
    connect(ui->btn_gamma,&QPushButton::clicked,this,[&](){
        if(!this->data.file_path.isNull())
        {
            ui->spinBox->hide();
            ui->doubleSpinBox->show();
            ui->doubleSpinBox->setMaximum(999);
            ui->doubleSpinBox->setMinimum(0.0);
            ui->doubleSpinBox->setValue(100);
            ui->label_arguments->show();
            ui->label_arguments->setText("请输入参数");
            ui->label_information->setText("当前选择: 伽马变换--pow");
            this->data.open_type=4;
            ui->btn_sub->setEnabled(true);
        }
    });
    //对比度调节
    connect(ui->btn_contrast,&QPushButton::clicked,this,[&](){
        if(!this->data.file_path.isNull())
        {
            ui->spinBox->hide();
            ui->doubleSpinBox->show();
            ui->doubleSpinBox->setMaximum(999);
            ui->doubleSpinBox->setMinimum(0.0);
            ui->doubleSpinBox->setValue(100);
            ui->label_arguments->show();
            ui->label_arguments->setText("请输入参数");
            ui->label_information->setText("当前选择: 对比度调整--convertTo");
            this->data.open_type=5;
            ui->btn_sub->setEnabled(true);
        }
    });
    //饱和度调节
    connect(ui->btn_saturation,&QPushButton::clicked,this,[&](){
        if(!this->data.file_path.isNull())
        {
            ui->spinBox->show();
            ui->spinBox->setMinimum(0);
            ui->spinBox->setMaximum(200);
            ui->spinBox->setValue(100);
            ui->label_arguments->show();
            ui->doubleSpinBox->hide();
            ui->label_arguments->setText("请输入参数");
            ui->label_information->setText("当前选择: 饱和度调整--convertTo");
            this->data.open_type=6;
            ui->btn_sub->setEnabled(true);
        }
    });
    //灰度直方图
    connect(ui->btn_calcHist,&QPushButton::clicked,this,[&](){
        if(!this->data.file_path.isNull())
        {
            ui->label_arguments->hide();
            ui->spinBox->hide();
            ui->doubleSpinBox->hide();
            ui->label_information->setText("当前选择: 灰度直方图--calcHist");
            this->data.open_type=7;
            ui->btn_sub->setEnabled(true);
        }
    });
    //二值化
    connect(ui->btn_Otsu,&QPushButton::clicked,this,[&](){
        if(!this->data.file_path.isNull())
        {
            ui->spinBox->hide();
            ui->doubleSpinBox->hide();
            ui->label_arguments->hide();
            ui->label_information->setText("当前选择: 二值化--Otsu");
            this->data.open_type=8;
            ui->btn_sub->setEnabled(true);
        }
    });
    
    
}

/*********初始化控件**********/
void MainWindow::Init_widget()      
{
    ui->toolBar->setMovable(false);
    ui->btn_original->setEnabled(false);
    ui->btn_sub->setEnabled(false);
    ui->btn_effect->setEnabled(false);  
    ui->label_arguments->hide();
    ui->spinBox->hide();
    ui->doubleSpinBox->hide();
}

/*******初始化样式表*********/
void MainWindow::Init_styleSheet()
{
    ui->centralwidget->setStyleSheet(""
                                     "QWidget {background-color: rgb(70,70,70);color:white }"
                                     "QPushButton { background-color: rgb(120,120,120); color:white} "
                                     "QPushButton:pressed { border: 1px solid white; background-color: rgb(150,150,150); color:white}");
    ui->toolBar->setStyleSheet("QToolBar {background-color: rgb(50,50,50) }");
}

/********键盘监听***********/
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        QApplication::quit(); // 执行退出操作
    }
    else
    {
        QMainWindow::keyPressEvent(event); // 其他情况下，交由父类处理
    }
}

/*******工具栏 *********/
//action_open槽函数
void MainWindow::on_action_open_triggered()
{
    QString defaultDir = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);    //用户图片文件夹路径
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open Image"), defaultDir, tr("Image Files (*.png *.jpg)"));  //文件路径
    QImage img(file_name);
    if(file_name.isEmpty())     //判断文件名路径是否为空
    {
        return;
    }
    if(img.isNull())        //判断文件是否有效
    {
        QMessageBox::critical(nullptr, "Error", "目标文件错误！");
        return;
    }
    this->qimage = QImage(file_name);
    ui->stackedWidget->setCurrentWidget(ui->page_sw1);  //切换stackedWidget
    ui->label_img1->setPixmap(QPixmap::fromImage(QImage(file_name).scaled(width(),height(),Qt::KeepAspectRatio)));
    ui->label_img2->setPixmap(QPixmap::fromImage(QImage(file_name).scaled(width(),height(),Qt::KeepAspectRatio)));
    this->data.file_path = file_name;
    ui->btn_original->setEnabled(true);     //启用按钮btn_original
}

//action_save槽函数
void MainWindow::on_action_save_triggered()
{
    
    if(qimage.isNull())
    {
        ui->label_information->setText("原始文件无效");
        return;
    }
    QString file_name = QFileDialog::getSaveFileName(nullptr, "Save Image", "", "JPEG (*.jpg *.jpeg);;PNG (*.png)");
    if (file_name.isNull())    //判断文件名路径是否为空
    {
        return;
    }

    // 保存图像
    if (!qimage.save(file_name)) {
        ui->label_information->setText("文件保存失败");
    }
    ui->label_information->setText("保存成功！");
    //this->qimage.save("file_name");
}

//help信息
void MainWindow::on_action_help_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->page_sw4);
    ui->label_arguments->hide();
    ui->label_information->hide();
    ui->spinBox->hide();
    ui->doubleSpinBox->hide();
}

