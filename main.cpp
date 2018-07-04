#include <mywidget.h>
#include <QApplication>

int main(int argc,char *argv[])
{
    QApplication app(argc,argv);
    const QStringList arguments = app.arguments();
    myWidget mywidget;
    mywidget.resize(800, 600);
    mywidget.show();
    // The final argument is assumed to be the file to open.
    if (arguments.size() > 1 && QFile::exists(arguments.last())) {
      mywidget.openOutside(arguments.last());
    }
    return app.exec();
}
