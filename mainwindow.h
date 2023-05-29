#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QMessageBox>      //输出提示信息
#include <QFileDialog>      //获取文件路径
#include <QStandardPaths>   //用于取用户图片文件夹路径 QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
#include <QShortcut>
#include <QKeyEvent>
#include "mythread.h"       //子线程类
#include "data_by_struct.h" //数据传递结构体

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
    
    void Updata_img(QImage img);
private:
    Ui::MainWindow *ui;
    
    QImage qimage;
    Data data;
    void Init_signals_slots();
    void Init_widget();
    void Init_styleSheet();
    
    //快捷键
    QShortcut *shortcut1 = new QShortcut(QKeySequence(Qt::Key_Return), this);
    QShortcut *shortcut2 = new QShortcut(QKeySequence(Qt::Key_Enter), this);
    //声明 keyPressEvent函数 键盘监听
    void keyPressEvent(QKeyEvent *event);
signals:
    void Send_data(Data data);
    
private slots:

    void on_action_open_triggered();
    void on_action_save_triggered();
    void on_action_help_triggered();
};
#endif // MAINWINDOW_H
