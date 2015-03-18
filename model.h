#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <face.h>
#include <QList>
#include <QFile>
#include <QMessageBox>
#include <QtOpenGL>
#include <GL/glu.h>
class stlModel : public QObject
{
    Q_OBJECT
public:
    explicit stlModel(QObject *parent = 0);
private:
    QList<face*> model;
public:
    void model_load(QString path);
    void model_readText(QString path);
    void model_readBinary(QString path);
    void model_clear();
    void model_draw();

    void model_test();
private:
    QVector3D getCoordinateFromString(QString line);
signals:
    void signal_finishLoad();
    
public slots:
    
};

#endif // MODEL_H
