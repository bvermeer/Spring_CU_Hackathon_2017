#ifndef DEBUGDEFINES_H
#define DEBUGDEFINES_H

#include <QDebug>


#define myOut() qDebug() << "[" << __PRETTY_FUNCTION__ << "]"
#define myErr() qDebug() << "[" << __PRETTY_FUNCTION__ << "]" << "Error:"


#endif // DEBUGDEFINES_H
