#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QGLWidget>
#include <model.h>
#include <camera.h>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

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

    Camera cam;

    STLModel *model;
    QVector3D centerInOpenGlWid;
    QVector3D cameraInOpenGlWid;

    QPointF lastPos;



    float *matrixForMouseRotate;

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
    void slot_moveUp();
    void slot_moveDown();

public:
    void setMaterial(materialStruct *material);

    void RotateY(float angle);
    void RotateX(float angle);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // OPENGLWIDGET_H
