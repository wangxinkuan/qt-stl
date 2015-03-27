#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <openglwidget.h>
#include <QPushButton>
#include <QFileDialog>
class myWidget : public QWidget
{
    Q_OBJECT
public:
    explicit myWidget(QWidget *parent = 0);
private:
    openglWidget *opengl;
    QPushButton *openstl;
    QPushButton *moveLeft;
    QPushButton *moveRight;
    QPushButton *moveUp;
    QPushButton *moveDown;
signals:
    void signal_loadStl(QString path);
public slots:
    void slot_openStl();
public:
    void openOutside(QString path);
    
};

#endif // MYWIDGET_H
