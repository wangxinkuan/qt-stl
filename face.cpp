#include "face.h"

face::face(QObject *parent) :
    QObject(parent)
{
}
QVector<QVector3D> face::getTriAngle() const
{
    return triAngle;
}

void face::setTriAngle(const QVector<QVector3D > &value)
{
    triAngle = value;
}

QVector3D face::getNormalVector() const
{
    return normalVector;
}

void face::setNormalVector(const QVector3D &value)
{
    normalVector = value;
}

