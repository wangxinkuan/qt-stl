#ifndef FACE_H
#define FACE_H

#include <QObject>
#include <QVector>
#include <QVector3D>
//定义一个面
class face : public QObject
{
    Q_OBJECT
public:
    explicit face(QObject *parent = 0);
private:
    QVector3D normalVector;
    QVector<QVector3D> triAngle;
public:

    QVector3D getNormalVector() const;
    void setNormalVector(const QVector3D &value);

    QVector<QVector3D> getTriAngle() const;
    void setTriAngle(const QVector<QVector3D> &value);

signals:
    
public slots:
    
};

#endif // FACE_H
