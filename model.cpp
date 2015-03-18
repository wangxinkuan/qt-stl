#include "model.h"
#include <QTextStream>
#include <QDebug>
#include <QDateTime>


stlModel::stlModel(QObject *parent) :
    QObject(parent)
{
}

void stlModel::model_load(QString path)
{
    //将这个路径的path载入咱们的QList<face *> model中
    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray header = file.read(80);
       // qDebug() << "here comes header:" << endl << header;
        file.close();

        if (header.trimmed().startsWith("solid") && header.contains("\n"))
        {
            model_readText(path);
        }
        else
        {
            model_readBinary(path);
        }
    }
    else
    {
        QMessageBox::information(0,tr("错误"),tr("未能成功加载文件"));
    }



}

void stlModel::model_readText(QString path)
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间

    qDebug()<< time.toString("yyyy-MM-dd hh:mm:ss ddd"); //设置显示格式
    model_clear();
    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QVector3D vn;
    QVector3D point1;
    QVector3D point2;
    QVector3D point3;
    QVector<QVector3D> triAngel;
    face *tempFace ;


    int count=0;

    QString line;
    QTextStream in(&file);
    line = in.readLine();                   // solid

    while(true)
    {
        line = in.readLine().trimmed();     //facet or endsolid
        if(line.trimmed().startsWith("endsolid"))
        {
            break;
        }
        vn = getCoordinateFromString(line);
        line = in.readLine();               //outer loop
        line = in.readLine().trimmed();     //vertex 1
        point1 = getCoordinateFromString(line);
        line = in.readLine().trimmed();     //vertex 2
        point2 = getCoordinateFromString(line);
        line = in.readLine().trimmed();     //vertex 3
        point3 = getCoordinateFromString(line);
        line = in.readLine();               //endloop
        line = in.readLine();               //endfacet

        triAngel.clear();
        triAngel.append(point1);
        triAngel.append(point2);
        triAngel.append(point3);

        tempFace = new face;
        tempFace->setNormalVector(vn);
        tempFace->setTriAngle(triAngel);

        model.append(tempFace);
        count++;

        if(count <10)
        {
            qDebug()<<model.at(count-1)->getTriAngle();
            qDebug()<<count;
        }
    }


    time = QDateTime::currentDateTime();//获取系统现在的时间
    qDebug()<< time.toString("yyyy-MM-dd hh:mm:ss ddd"); //设置显示格式
    file.close();
    emit signal_finishLoad();
}


void stlModel::model_readBinary(QString path)
{
    //http://qt-project.org/forums/viewthread/14726
    QFile file(path);
    file.open(QIODevice::ReadOnly);

    QVector3D vn;
    QVector3D point1;
    QVector3D point2;
    QVector3D point3;
    QVector<QVector3D> triAngel;
    face *tempFace;

    file.seek(0);
    QDataStream in(&file);
    in.setByteOrder(QDataStream::LittleEndian);
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);

    quint32 n;
    quint16 control_bytes;
    quint32 count = 0;

    file.seek(80);
    in >> n;
    qDebug() << n << "triangles in the model.";

    float nx, ny, nz, x1, x2, x3, y1, y2, y3, z1, z2, z3;

    while (count < n)
    {
        file.seek(84+count*50+0+0);
        in >> nx;
        file.seek(84+count*50+0+4);
        in >> ny;
        file.seek(84+count*50+0+8);
        in >> nz;
        file.seek(84+count*50+12+0);
        in >> x1;
        file.seek(84+count*50+12+4);
        in >> y1;
        file.seek(84+count*50+12+8);
        in >> z1;
        file.seek(84+count*50+24+0);
        in >> x2;
        file.seek(84+count*50+24+4);
        in >> y2;
        file.seek(84+count*50+24+8);
        in >> z2;
        file.seek(84+count*50+36+0);
        in >> x3;
        file.seek(84+count*50+36+4);
        in >> y3;
        file.seek(84+count*50+36+8);
        in >> z3;
        file.seek(84+count*50+48);
        in >> control_bytes;

        if (control_bytes == 0 && in.status() == QDataStream::Ok)
        {
            vn = QVector3D(nx,ny,nz);
            point1= QVector3D(x1,y1,z1);
            point2= QVector3D(x2,y2,z2);
            point3= QVector3D(x3,y3,z3);

            triAngel.clear();
            triAngel.append(point1);
            triAngel.append(point2);
            triAngel.append(point3);
            tempFace = new face;
            tempFace->setNormalVector(vn);
            tempFace->setTriAngle(triAngel);

            model.append(tempFace);

            count++;
        }
        else
        {
            qDebug() << "File read error when readStlBinaryFormat"
                     << (int) in.status();

        }
    }

    //qDebug() << m_model.size() << "facets read in readStlBinaryFormat";
    file.close();

}

void stlModel::model_clear()
{
    model.clear();
}

void stlModel::model_draw()
{
    QVector<QVector3D> triAngle;
    QVector3D normal;
    GLfloat normalVector[3];
    qDebug()<<"da xiao shi"<<model.size();
    for(int i=0; i<model.size(); i++)
    {
        normal = model.at(i)->getNormalVector();
        normal.normalize();
        normalVector[0] = normal.x();
        normalVector[1] = normal.y();
        normalVector[2] = normal.z();
        triAngle = model.at(i)->getTriAngle();
      glBegin(GL_TRIANGLES);//绘制三角面片
        glNormal3fv(normalVector);
        glVertex3f(triAngle.at(0).x(),triAngle.at(0).y(),triAngle.at(0).z() );
        glVertex3f(triAngle.at(1).x(),triAngle.at(1).y(),triAngle.at(1).z() );
        glVertex3f(triAngle.at(2).x(),triAngle.at(2).y(),triAngle.at(2).z() );
      glEnd();
    }

//        glBegin(GL_TRIANGLES);								// Drawing Using Triangles
//            glVertex3f( 0.0f, 1.0f, 0.0f);					// Top
//            glVertex3f(-1.0f,-1.0f, 0.0f);					// Bottom Left
//            glVertex3f( 1.0f,-1.0f, 0.0f);					// Bottom Right
//        glEnd();
}

void stlModel::model_test()
{
    QVector<QVector3D> test;
    if(model.isEmpty())
        return;
    qDebug()<<tr("大小是")<<model.size();
    for(int i=0;i<10;i++)
    {
        test = model.at(i)->getTriAngle();
        qDebug()<<test;
    }
}

QVector3D stlModel::getCoordinateFromString(QString line)
{
     QVector3D coordinate;
     QVector<float> point;

     QStringList strList = line.split(" ");

    int count = strList.size();
    for(int i=count-3;i<count;i++)
    {
        point.append(strList[i].toDouble());

    }
    coordinate.setX(point[0]);
    coordinate.setY(point[1]);
    coordinate.setZ(point[2]);
    return coordinate;
}
