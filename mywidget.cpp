#include "mywidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
myWidget::myWidget(QWidget *parent) :
    QWidget(parent)
{
    opengl = new openglWidget(this);
    openstl = new QPushButton(this);
    openstl->setText(tr("打开stl文件"));
    moveLeft = new QPushButton(tr("左移"));
    moveRight = new QPushButton(tr("右移"));
    moveForward = new QPushButton(tr("前移"));
    moveBackward = new QPushButton(tr("后移"));
    moveUp = new QPushButton(tr("上移"));
    moveDown = new QPushButton(tr("下移"));

    connect(moveLeft,SIGNAL(clicked()),
            opengl,SLOT(slot_moveLeft()));
    connect(moveRight,SIGNAL(clicked()),
            opengl,SLOT(slot_moveRight()));
    connect(moveForward,SIGNAL(clicked()),
            opengl,SLOT(slot_moveForward()));
    connect(moveBackward,SIGNAL(clicked()),
            opengl,SLOT(slot_moveBackWard()));
    connect(moveUp,SIGNAL(clicked()),
            opengl,SLOT(slot_moveUp()));
    connect(moveDown,SIGNAL(clicked()),
            opengl,SLOT(slot_moveDown()));


    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(openstl);
    hbox->addWidget(moveLeft);
    hbox->addWidget(moveRight);
    hbox->addWidget(moveForward);
    hbox->addWidget(moveBackward);
    hbox->addWidget(moveUp);
    hbox->addWidget(moveDown);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(opengl);
    vbox->addLayout(hbox);
    setLayout(vbox);


    connect(openstl,SIGNAL(clicked()),
            this,SLOT(slot_openStl()));

    connect(this,SIGNAL(signal_loadStl(QString)),
            opengl,SLOT(slot_recieveStl(QString)));


}

void myWidget::slot_openStl()
{
    //加载一个stl文件  设置打开文件的过滤器
    QString path;
    path = QFileDialog::getOpenFileName(this,"open file dialog",
                                                        "/","STL files (*.stl)");
    //然后将路径传递给  opengl，opengl里面在调用model->load
    if(!path.isEmpty())
    {
        emit signal_loadStl(path);
    }

}
