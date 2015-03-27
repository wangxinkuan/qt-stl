#include <mywidget.h>
#include <QApplication>

int main(int argc,char *argv[])
{
    QApplication app(argc,argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    const QStringList arguments = app.arguments();
    myWidget mywidget;
    mywidget.show();
    // The final argument is assumed to be the file to open.
    if (arguments.size() > 1 && QFile::exists(arguments.last())) {
      mywidget.openOutside(arguments.last());
    }
    return app.exec();
}
