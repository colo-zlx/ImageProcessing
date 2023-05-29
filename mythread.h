#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include "processing.h"
#include "data_by_struct.h"

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = nullptr);
    
    void Set_data(Data data);
protected:
    void run() override;
    
private:
    Data data;
    float a=1.1;
signals:
    void Open_finish(QImage);
    
};

#endif // MYTHREAD_H
