#include "model.h"
#include <QTextStream>
#include <QDebug>
#include <QDateTime>


STLModel::STLModel(QObject *parent) :
    QObject(parent)
{   
    model_clear();
}

void STLModel::model_load(QString path)
{
    //将这个路径的path载入咱们的QList<face *> model中
    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray header = file.read(80);
        file.close();

        if (header.trimmed().startsWith("solid") && header.contains("\n"))
        {
            qDebug("start to read text");
            model_readText(path);
        }
        else
        {
            qDebug("huaile");
            model_readBinary(path);
        }
    }
    else
    {
        QMessageBox::information(0,tr("错误"),tr("未能成功加载文件"));
    }
}

void STLModel::model_readText(QString path)
{
    model_clear();

    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间

//    qDebug()<< time.toString("yyyy-MM-dd hh:mm:ss ddd"); //设置显示格式

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
        //以下三句是更新模型的大小
        model_getSizeWhileReadPoint(point1);
        model_getSizeWhileReadPoint(point2);
        model_getSizeWhileReadPoint(point3);

        tempFace = new face;
        tempFace->setNormalVector(vn);
        tempFace->setTriAngle(triAngel);

        faceList.append(tempFace);
        count++;
    }


    time = QDateTime::currentDateTime();//获取系统现在的时间
//    qDebug()<< time.toString("yyyy-MM-dd hh:mm:ss ddd"); //设置显示格式
    file.close();
    emit signal_finishLoad();
}


void STLModel::model_readBinary(QString path)
{
    model_clear();
    //http://qt-project.org/forums/viewthread/14726
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
        return;
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

            //以下三句是更新模型的大小
            model_getSizeWhileReadPoint(point1);
            model_getSizeWhileReadPoint(point2);
            model_getSizeWhileReadPoint(point3);

            tempFace = new face;
            tempFace->setNormalVector(vn);
            tempFace->setTriAngle(triAngel);

            faceList.append(tempFace);

            count++;
        }
        else
        {
            qDebug() << "File read error when readStlBinaryFormat"
                     << (int) in.status();

        }
    }
    file.close();
    qDebug()<<tr("finish load");
    qDebug()<<faceList.size();
    emit signal_finishLoad();
}

void STLModel::model_clear()
{
    faceList.clear();
    size.smallX = 0;
    size.largeX = 0;
    size.smallY = 0;
    size.largeY = 0;
    size.smallZ = 0;
    size.largeZ = 0;
}

void STLModel::model_draw()
{
    QVector<QVector3D> triAngle;
    QVector3D normal;
    GLfloat normalVector[3];
    for(int i=0; i<faceList.size(); i++)
    {
        normal = faceList.at(i)->getNormalVector();
        normal.normalize();
        normalVector[0] = normal.x();
        normalVector[1] = normal.y();
        normalVector[2] = normal.z();
        triAngle = faceList.at(i)->getTriAngle();
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

void STLModel::model_test()
{
    QVector<QVector3D> test;
    if(faceList.isEmpty())
        return;
    for(int i=0;i<10;i++)
    {
        test = faceList.at(i)->getTriAngle();
    }
}

modelSize STLModel::model_getSize()
{
    return size;
}

void STLModel::model_getSizeWhileReadPoint(QVector3D point)
{
    float pointX = point.x();
    float pointY = point.y();
    float pointZ = point.z();
   //处理X
   if(pointX < size.smallX)
       size.smallX = pointX;
   else if(pointX > size.largeX)
       size.largeX = pointX;
   //处理Y
   if(pointY < size.smallY)
       size.smallY = pointY;
   else if(pointY > size.largeY)
       size.largeY = pointY;
   //处理Z
   if(pointZ < size.smallZ)
       size.smallZ = pointZ;
   else if(pointZ > size.largeZ)
       size.largeZ = pointZ;
}

QVector3D STLModel::model_getCenter(modelSize size)
{
    QVector3D center;
    center.setX( (size.largeX + size.smallX)/2 );
    center.setY( (size.largeY + size.smallY)/2 );
    center.setZ( (size.largeZ + size.smallZ)/2 );
    return center;
}

int STLModel::model_getFacesCount()
{
    return faceList.size();
}

QVector3D STLModel::getCoordinateFromString(QString line)
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
