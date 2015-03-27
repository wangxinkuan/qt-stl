#ifndef CAMERA_H
#define CAMERA_H

#include <QtOpenGL>
#include <math.h>

class Point3
{
public:
    float x,y,z;
    void set(float dx,float dy,float dz)
    {
        x=dx; y=dy; z=dz;
    }
    void set(Point3& p)
    {
        x=p.x; y=p.y; z=p.z;
    }
    Point3(float xx,float yy,float zz)
    {
        x=xx; y=yy; z=zz;
    }
    Point3()
    {
        x=y=z=0;
    }
    void build4tuple(float v[])
    {
        v[0]=x; v[1]=y; v[2]=z; v[3]=1.0f;
    }
};

class Vector3
{
public:
    float x,y,z;
    void set(float dx,float dy,float dz)
    {
        x=dx; y=dy; z=dz;
    }
    void set(Vector3& v)
    {
        x=v.x; y=v.y; z=v.z;
    }
    void flip()
    {
        x=-x; y=-y; z=-z;
    }
    void setDiff(Point3& a,Point3& b)
    {
        x=a.x-b.x; y=a.y-b.y; z=a.z-b.z;
    }
    void normalize()
    {
        float base=pow(x,2)+pow(y,2)+pow(z,2);
        x=x/pow(base,0.5);
        y=y/pow(base,0.5);
        z=z/pow(base,0.5);
    }
    Vector3(float xx, float yy, float zz)
    {
        x=xx; y=yy; z=zz;
    }
    Vector3(Vector3 &v)
    {
        x=v.x; y=v.y; z=v.z;
    }
    Vector3()
    {
        x=0; y=0; z=0;
    }

    Vector3 cross(Vector3 b)
    {
        float x1,y1,z1;
        x1=y*b.z-z*b.y;
        y1=z*b.x-x*b.z;
        z1=x*b.y-y*b.x;
        Vector3 c(x1,y1,z1);
        return c;
    }

    float dot(Vector3 b)
    {
        float d=x*b.x+y*b.y+z*b.z;
        return d;
    }
};

class Camera
{
public:
/* 构造函数和析构函数 */
Camera();
~Camera();

/* 设置摄像机的位置, 观察点和向上向量 */
void setCamera( float eyeX, float eyeY, float eyeZ,
                float lookX, float lookY, float lookZ,
                float upX, float upY, float upZ);
void roll(float angle);
void pitch(float angle);
void yaw(float angle);
void slide(float du, float dv, float dn);
float getDist();
void setShape(float viewAngle,float aspect,float Near,float Far);

private:
/* 摄像机属性 */
Point3         eye,look,up;
Vector3        u,v,n;
float          viewAngle, aspect, nearDist, farDist;
public:
    void      setModelViewMatrix();


};

#endif //__CAMERA_H__
