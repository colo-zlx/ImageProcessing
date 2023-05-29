#ifndef DATA_BY_STRUCT_H
#define DATA_BY_STRUCT_H

#include <QString>

typedef struct MyData
{
    QString file_path;
    int open_type;
    int argu_i;
    double argu_d;
}Data;

#endif // DATA_BY_STRUCT_H
