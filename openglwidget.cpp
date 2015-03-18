#include "openglwidget.h"

openglWidget::openglWidget(QWidget *parent) :
    QGLWidget(parent)
{
    model = new stlModel(this);
    connect(model,SIGNAL(signal_finishLoad()),
            this,SLOT(slot_finishLoadStl()));
    initialOpengl();
}

void openglWidget::initialOpengl()
{
    rotateX = rotateY = rotateZ =0;
}

void openglWidget::paintGL()
{
    qDebug()<<"paintGL";
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
   // glColor3f(255,0,0);
   // model->model_draw();
    glTranslatef(0.0f,0.0f,-200.0f);
    glRotatef(rotateX,1.0,0.0,0.0);
    glRotatef(rotateY,0.0,1.0,0.0);
    glRotatef(rotateZ,0.0,0.0,1.0);

    model->model_draw();
//    glBegin(GL_TRIANGLES);								// Drawing Using Triangles
//        glVertex3f( 0.0f, 1.0f, 0.0f);					// Top
//        glVertex3f(-1.0f,-1.0f, 0.0f);					// Bottom Left
//        glVertex3f( 1.0f,-1.0f, 0.0f);					// Bottom Right
//    glEnd();
}

void openglWidget::resizeGL(int w, int h)
{
    qDebug()<<"resize";

    if (h==0)										// Prevent A Divide By Zero By
    {
        h=1;										// Making Height Equal One
    }

    glViewport(0,0,w,h);						// Reset The Current Viewport

    glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
    glLoadIdentity();									// Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    gluPerspective(45.0f,(GLfloat)w/(GLfloat)h,0.1f,300.0f);

    glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
    glLoadIdentity();
}

void openglWidget::initializeGL()
{
    lightingStruct whiteLighting ={
        {0.0,0.0,0.0,1.0},
        {1.0,1.0,1.0,1.0},
        {1.0,1.0,1.0,1.0},
    };

    materialStruct whiteShinyMaterial={
        {1.0,1.0,1.0,1.0},
        {1.0,1.0,1.0,1.0},
        {1.0,1.0,1.0,1.0},
        100.0
    };

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glDisable( GL_CULL_FACE);       //加上上面俩句 就能把反面光也调出来了
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0,GL_AMBIENT,whiteLighting.ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,whiteLighting.diffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,whiteLighting.specular);
    //这里暂时没有指定光源的位置
    setMaterial(&whiteShinyMaterial);

  //  glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
    glClearDepth(1.0f);									// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
}

void openglWidget::slot_recieveStl(QString path)
{
    //文件读取 提取其中的各个面
    //载入咱们的model里面
    model->model_clear();
    model->model_load(path);
    //最后再刷新一下repaint

}

void openglWidget::slot_finishLoadStl()
{
    qDebug()<<"slot_finishLoadStl updateGl";
    initialOpengl();
    updateGL();
}

void openglWidget::slot_moveLeft()
{
    qDebug()<<rotateX;
    rotateX +=5.0;
    if(rotateX == 360)
        rotateX =0.0;
    updateGL();

}

void openglWidget::slot_moveRight()
{
    rotateX -=5.0;
    if(rotateX == -360)
        rotateX =0.0;
    updateGL();
}

void openglWidget::slot_moveForward()
{
    rotateY +=5.0;
    if(rotateY == 360)
        rotateY =0.0;
    updateGL();
}

void openglWidget::slot_moveBackWard()
{
    rotateY -=5.0;
    if(rotateY == -360)
        rotateY =0.0;
    updateGL();
}

void openglWidget::slot_moveUp()
{
    rotateZ +=5.0;
    if(rotateZ == 360)
        rotateZ =0.0;
    updateGL();
}

void openglWidget::slot_moveDown()
{
    rotateZ -=5.0;
    if(rotateZ == -360)
        rotateZ =0.0;
    updateGL();
}

void openglWidget::setMaterial(materialStruct *material)
{
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,material->ambient);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,material->diffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,material->specular);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,material->shininess);
}
