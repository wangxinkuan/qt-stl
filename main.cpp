#include <mywidget.h>
#include <QApplication>

int main(int argc,char *argv[])
{
    QApplication app(argc,argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    myWidget mydialog;
    mydialog.show();
    return app.exec();
}
