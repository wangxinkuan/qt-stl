#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QGLWidget>
#include <model.h>

typedef struct lightingStruct{
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
}lightingStruct;

typedef struct materialStruct{
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess;
}materialStruct;


class openglWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit openglWidget(QWidget *parent = 0);
private:
    stlModel *model;
private:
    float rotateX;
    float rotateY;
    float rotateZ;
public:
    void initialOpengl();
protected:
    void paintGL();
    void resizeGL(int w, int h);
    void initializeGL();
    
signals:
    
public slots:
    void slot_recieveStl(QString path);
    void slot_finishLoadStl();

    void slot_moveLeft();
    void slot_moveRight();
    void slot_moveForward();
    void slot_moveBackWard();
    void slot_moveUp();
    void slot_moveDown();

public:
    void setMaterial(materialStruct *material);

    
};

#endif // OPENGLWIDGET_H
