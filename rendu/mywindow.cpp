#include "myWindow.h"

myWindow::myWindow(QWidget *parent)
    : myGLWidget(60, parent, "TP2 Voxels")
{
}

void myWindow::initializeGL()
{
    _fx = 0.0;
    _speed =0.1;

    loadTexture(":/textures/herbe");
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void myWindow::loadTexture(QString textureName){
    QImage qim_Texture;
    QImage qim_TempTexture;
    qim_TempTexture.load(textureName);
    qim_Texture = QGLWidget::convertToGLFormat( qim_TempTexture );
    glGenTextures( 1, &texture[0] );
    glBindTexture( GL_TEXTURE_2D, texture[0] );
    glTexImage2D( GL_TEXTURE_2D, 0, 3, qim_Texture.width(), qim_Texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qim_Texture.bits() );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}

void myWindow::keyPressEvent(QKeyEvent *keyEvent)
{
    switch(keyEvent->key())
    {
        case Qt::Key_Escape:
            close();
            break;
        case Qt::Key_Plus:
            _speed += 0.05;
        break;
        case Qt::Key_Minus:
            _speed -= 0.05;
        break;
        case Qt::Key_Z:
            _zoom += 1.0;
        break;
        case Qt::Key_S:
            _zoom -= 1.0;
        break;
        case Qt::Key_Up:
            _hauteur += 1.0;
        break;
        case Qt::Key_Down:
            _hauteur -= 1.0;
        break;
    }
}


// Replaces gluPerspective. Sets the frustum to perspective mode.
// fovY     - Field of vision in degrees in the y direction
// aspect   - Aspect ratio of the viewport
// zNear    - The near clipping distance
// zFar     - The far clipping distance
void perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar )
{
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;
    fH = tan( (fovY / 2) / 180 * pi ) * zNear;
    fH = tan( fovY / 360 * pi ) * zNear;
    fW = fH * aspect;
    glFrustum( -fW, fW, -fH, fH, zNear, zFar );

}


void myWindow::resizeGL(int width, int height)
{
    if(height == 0)
        height = 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    perspectiveGL(90.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void myWindow::paintGL()
{
    int LightPos[4] = {-5,0,5,0};
    _fx += _speed;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    glLightiv(GL_LIGHT0,GL_POSITION,LightPos);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 10);

    glTranslatef(0.f, _hauteur, _zoom);
    glRotated(10,1,0,0);
    glRotated(_fx,0,1,0);

    for(float i2=-vox.get_n()/2.0; i2<vox.get_n()/2; i2++){
        for(float j2=-vox.get_n()/2.0; j2<vox.get_n()/2; j2++){
            for(float k2=-vox.get_n()/2.0; k2<vox.get_n()/2; k2++){
                if(vox(i2+vox.get_n()/2,j2+vox.get_n()/2,k2+vox.get_n()/2)==1){
                    float scale = 0.1;
                    float i = i2*scale;
                    float j = j2*scale;
                    float k = k2*scale;/*
                    glBegin(GL_QUADS);
                        glColor3ub(255,100,100); //face rouge
                        glVertex3d(i,j,k);
                        glVertex3d(i,j,k-scale);
                        glVertex3d(i-scale,j,k-scale);
                        glVertex3d(i-scale,j,k);

                        glColor3ub(255,255,100); //face jaune
                        glVertex3d(i,j-scale,k);
                        glVertex3d(i,j-scale,k-scale);
                        glVertex3d(i,j,k-scale);
                        glVertex3d(i,j,k);

                        glColor3ub(100,100,255); //face bleue
                        glVertex3d(i-scale,j-scale,k);
                        glVertex3d(i-scale,j-scale,k-scale);
                        glVertex3d(i,j-scale,k-scale);
                        glVertex3d(i,j-scale,k);

                        glColor3ub(255,100,100); //face rouge
                        glVertex3d(i-scale,j,k);
                        glVertex3d(i-scale,j,k-scale);
                        glVertex3d(i-scale,j-scale,k-scale);
                        glVertex3d(i-scale,j-scale,k);

                        glColor3ub(255,255,100); //face jaune
                        glVertex3d(i,j,k-scale);
                        glVertex3d(i,j-scale,k-scale);
                        glVertex3d(i-scale,j-scale,k-scale);
                        glVertex3d(i-scale,j,k-scale);

                        glColor3ub(100,100,255); //face bleue
                        glVertex3d(i,j-scale,k);
                        glVertex3d(i,j,k);
                        glVertex3d(i-scale,j,k);
                        glVertex3d(i-scale,j-scale,k);
                    glEnd();*/

                    glBindTexture(GL_TEXTURE_2D, texture[0]);

                    glBegin(GL_QUADS);
                        // Face Avant
                        glNormal3d(-1, 0, 0);
                        glTexCoord2f(0.0f, 0.0f); glVertex3f(i-scale, j-scale, k);
                        glTexCoord2f(1.0f, 0.0f); glVertex3f(i, j-scale, k);
                        glTexCoord2f(1.0f, 1.0f); glVertex3f(i, j,  k);
                        glTexCoord2f(0.0f, 1.0f); glVertex3f(i-scale, j,  k);
                        // Face Arrière
                        glNormal3d(1, 0, 0);
                        glTexCoord2f(1.0f, 0.0f); glVertex3f(i-scale, j-scale, k-scale);
                        glTexCoord2f(1.0f, 1.0f); glVertex3f(i-scale,  j, k-scale);
                        glTexCoord2f(0.0f, 1.0f); glVertex3f(i, j, k-scale);
                        glTexCoord2f(0.0f, 0.0f); glVertex3f(i, j-scale, k-scale);
                        // Face Haut
                        glNormal3d(0, 1, 0);
                        glTexCoord2f(0.0f, 1.0f); glVertex3f(i-scale, j, k-scale);
                        glTexCoord2f(0.0f, 0.0f); glVertex3f(i-scale, j, k);
                        glTexCoord2f(1.0f, 0.0f); glVertex3f(i, j, k);
                        glTexCoord2f(1.0f, 1.0f); glVertex3f(i, j, k-scale);
                        // Face Bas
                        glNormal3d(0, -1, 0);
                        glTexCoord2f(1.0f, 1.0f); glVertex3f(i-scale,j -scale, k-scale);
                        glTexCoord2f(0.0f, 1.0f); glVertex3f(i,j -scale, k-scale);
                        glTexCoord2f(0.0f, 0.0f); glVertex3f(i,j -scale,  k);
                        glTexCoord2f(1.0f, 0.0f); glVertex3f(i-scale, j-scale,  k);
                        // Face Droite
                        glNormal3d(0, 0, 1);
                        glTexCoord2f(1.0f, 0.0f); glVertex3f( i, j-scale, k-scale);
                        glTexCoord2f(1.0f, 1.0f); glVertex3f( i, j, k-scale);
                        glTexCoord2f(0.0f, 1.0f); glVertex3f( i, j, k);
                        glTexCoord2f(0.0f, 0.0f); glVertex3f( i, j-scale, k);
                        // Face Gauche
                        glNormal3d(0, 0, -1);
                        glTexCoord2f(0.0f, 0.0f); glVertex3f(i-scale, j-scale, k-scale);
                        glTexCoord2f(1.0f, 0.0f); glVertex3f(i-scale,j -scale,  k);
                        glTexCoord2f(1.0f, 1.0f); glVertex3f(i-scale,  j, k);
                        glTexCoord2f(0.0f, 1.0f); glVertex3f(i-scale,  j, k-scale);
                    glEnd();
                }
            }
        }
    }

/*
    glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();

    glTranslatef(3.0f, 0.0f, -6.0f);

    glBegin(GL_QUADS);
        glVertex3f(-1.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 0.0f);
        glVertex3d(1.0f, 1.0f, 0.0f);
    glEnd();*/
}
