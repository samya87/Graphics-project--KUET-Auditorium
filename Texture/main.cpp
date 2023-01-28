#include<GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include<math.h>
#include <ctype.h>
#include <bits/stdc++.h>
#include "BmpLoader.h"

//-3.08, -6
double windowHeight=2000, windowWidth=2000; const double PI = 3.14159265389;
double eyex=20, eyey=5, eyez=10, centerx = 0, centery = 2, centerz = -20, upx = 0, upy = 1, upz = 0,increment = 1, zoom=1.3;//28,2.3,-4
GLboolean  porda_k= false,door_open=false, bird= false, fanSwitch = false, switchOne = false, switchLamp=false, amb1=true, diff1=true, spec1=true, amb3=true, diff3=true, spec3=true;
double  x = 1.0, z = 7.0, a=2;
int flag = 1; GLfloat angle=0.0f, angle2=0.0f, door_angle=0.0f, sky_angle=0.0f;
GLfloat yaw_angle = 270.0, pitch_angle = 270.0, roll_angle = 90.0;
const int ntheta = 20; const int nt = 40;//number of slices along x-direction
float xa=1.0, ya=1.0, xb=0.0, yb=0.0, xc=0.03, yc=0.0, xd=-0.03, yd=0.0, xe=0.0, ye=0.0, xf=0.03, yf=0.0, xg=-0.03, yg=0.0, xh=0.0, yh=0.0, xi=0.03, yi=0.0, xj=-0.03, yj=0.0, xk=-0.03, yk=0.0,xl=0.0, yl=0.0;
unsigned int ID1, ID2;

long long nCr(int n, int r)
{
    if(r > n / 2) r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;

    for(i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}
static GLfloat v_cube[8][3] =
{
    {0.0, 0.0, 0.0}, //0
    {0.0, 0.0, 3.0}, //1
    {3.0, 0.0, 3.0}, //2
    {3.0, 0.0, 0.0}, //3
    {0.0, 3.0, 0.0}, //4
    {0.0, 3.0, 3.0}, //5
    {3.0, 3.0, 3.0}, //6
    {3.0, 3.0, 0.0}  //7
};
static GLfloat v_tra[8][3] =
{
    {0.0, 0.0, 0.0}, //0
    {0.0, 0.0, 1.5}, //1
    {3.0, 0.0, 3.0}, //2
    {3.0, 0.0, 0.0}, //3
    {0.0, 3.0, 0.0}, //4
    {0.0, 3.0, 1.5}, //5
    {3.0, 3.0, 3.0}, //6
    {3.0, 3.0, 0.0}  //7
};

GLfloat ctrlpoints3[13+1][3] =
{
    { 1.0, 0.0, 0.0}, { -1.3, 0.5, 0.0},
    { 1.1, 1.7, 0.0},{ 1.5, 1.5, 0.0},
    {3.0, 2.5, 0.0}, {3.4, 1.4, 0.0},
    {2.8, 0.4, 0.0},{3.2, 0.4, 0.0},
    {2.8, 1.5, 0.0}, {3.7, 1.4, 0.0},
    {4.4, 1.4, 2.5},
    {4.8, 1.4, 2.5},
    {5.8, 0.5, 0.0},
    {9.2, 0.5, 0.0},
};

static GLubyte quadIndices[6][4] =
{
    {0, 1, 2, 3}, //bottom
    {4, 5, 6, 7}, //top
    {5, 1, 2, 6}, //front
    {0, 4, 7, 3}, // back is clockwise
    {2, 3, 7, 6}, //right
    {1, 5, 4, 0}  //left is clockwise
};

static void getNormal3p (GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(Nx,Ny,Nz);
}

void LoadTexture1(const char*filename)
{
    glGenTextures(1, &ID1);
    glBindTexture(GL_TEXTURE_2D, ID1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, ID1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    BmpLoader bl(filename);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData );
}
void draw_sphere(float r, float g, float b, float radi)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { r, g, b, 1.0 };
    GLfloat mat_diffuse[] = { r, g, b, 1.0 };
    GLfloat mat_specular[] = { 1, 1,1 , 1.0 };
    GLfloat mat_shininess[] = {60};
//front phase niye kaaj hoche
    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    //glutSolidSphere (radi, 90, 120);
    GLUquadric *qd2;
    qd2 = gluNewQuadric();
    gluQuadricTexture(qd2, TRUE);
    gluSphere(qd2,radi,120,120);
}
void sphere(float r, float g, float b, float radi)
{
    glPushMatrix(); glTranslatef(0,5,7.0); draw_sphere(1,1,1, radi);glPopMatrix();//glScalef(.4, .1, .9); .4,.7,.96,
}
void draw_chaka(float r, float g, float b)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { r, g, b, 1.0 };
    GLfloat mat_diffuse[] = { r, g, b, 1.0 };
    GLfloat mat_specular[] = { 1, 1,1 , 1.0 };
    GLfloat mat_shininess[] = {60};
//front phase niye kaaj hoche
    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    glutSolidTorus(0.4,1,6,16);
}

void drawCube1(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX=0, GLfloat ambY=0, GLfloat ambZ=0, GLfloat shine=50)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {shine};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

    glBegin(GL_QUADS);

    for (GLint i = 0; i <6; i++)
    {
        getNormal3p(v_cube[quadIndices[i][0]][0], v_cube[quadIndices[i][0]][1], v_cube[quadIndices[i][0]][2],
                    v_cube[quadIndices[i][1]][0], v_cube[quadIndices[i][1]][1], v_cube[quadIndices[i][1]][2],
                    v_cube[quadIndices[i][2]][0], v_cube[quadIndices[i][2]][1], v_cube[quadIndices[i][2]][2]);
        glVertex3fv(&v_cube[quadIndices[i][0]][0]); glTexCoord2f(1,1);
        glVertex3fv(&v_cube[quadIndices[i][1]][0]); glTexCoord2f(0,1);
        glVertex3fv(&v_cube[quadIndices[i][2]][0]); glTexCoord2f(0,0);
        glVertex3fv(&v_cube[quadIndices[i][3]][0]); glTexCoord2f(1,0);
    }
    glEnd();
}

void drawCube2(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX=0, GLfloat ambY=0, GLfloat ambZ=0, GLfloat shine=50)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {shine};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

    glBegin(GL_QUADS);

    for (GLint i = 0; i <6; i++)
    {
        getNormal3p(v_cube[quadIndices[i][0]][0], v_cube[quadIndices[i][0]][1], v_cube[quadIndices[i][0]][2],
                    v_cube[quadIndices[i][1]][0], v_cube[quadIndices[i][1]][1], v_cube[quadIndices[i][1]][2],
                    v_cube[quadIndices[i][2]][0], v_cube[quadIndices[i][2]][1], v_cube[quadIndices[i][2]][2]);
        glVertex3fv(&v_cube[quadIndices[i][0]][0]); glTexCoord2f(1,1);
        glVertex3fv(&v_cube[quadIndices[i][1]][0]); glTexCoord2f(0,1);
        glVertex3fv(&v_cube[quadIndices[i][2]][0]); glTexCoord2f(0,0);
        glVertex3fv(&v_cube[quadIndices[i][3]][0]); glTexCoord2f(1,0);
    }
    glEnd();
}

void draw_tra(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX=0, GLfloat ambY=0, GLfloat ambZ=0, GLfloat shine=128)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {shine};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

    glBegin(GL_QUADS);

    for (GLint i = 0; i <6; i++)
    {
        getNormal3p(v_tra[quadIndices[i][0]][0], v_tra[quadIndices[i][0]][1], v_tra[quadIndices[i][0]][2],
                    v_tra[quadIndices[i][1]][0], v_tra[quadIndices[i][1]][1], v_tra[quadIndices[i][1]][2],
                    v_tra[quadIndices[i][2]][0], v_tra[quadIndices[i][2]][1], v_tra[quadIndices[i][2]][2]);
        glVertex3fv(&v_tra[quadIndices[i][0]][0]); glTexCoord2f(1,1);
        glVertex3fv(&v_tra[quadIndices[i][1]][0]); glTexCoord2f(0,1);
        glVertex3fv(&v_tra[quadIndices[i][2]][0]); glTexCoord2f(0,0);
        glVertex3fv(&v_tra[quadIndices[i][3]][0]); glTexCoord2f(1,0);
    }
    glEnd();
}


void LoadTexture2(const char*filename)
{
    glGenTextures(1, &ID2);
    glBindTexture(GL_TEXTURE_2D, ID2);
    glPixelStorei(GL_UNPACK_ALIGNMENT, ID2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    BmpLoader bl(filename);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData );
}


void car()
{
    glTranslatef(3.7,0,1.0);
    glPushMatrix();glScalef(.3, .08, .65); glTranslatef(21,4.8,7.0);drawCube1(0.03, 0.0, 0.5, 0.0,0.03,0.2);glPopMatrix();//base
    glPushMatrix();glScalef(.3, .15, .4); glTranslatef(21,4,12.0);drawCube1(0.23, 0.0, 0.9, 0.0,0.03,0.288);glPopMatrix();//upper cube

    glPushMatrix();glScalef(.2, .2, .2);glTranslated(36,1,31);glRotated(90,0,1,0);draw_chaka(0,0,0);glPopMatrix(); //front_right chaka glutSolidTorus
    glPushMatrix();glScalef(.2, .2, .2);glTranslated(31.5,1,31);glRotated(90,0,1,0);draw_chaka(0,0,0);glPopMatrix(); //front_left chaka glutSolidTorus
    glPushMatrix();glScalef(.2, .2, .2);glTranslated(36,1,25.2);glRotated(90,0,1,0);draw_chaka(0,0,0);glPopMatrix(); //back_right chaka glutSolidTorus
    glPushMatrix();glScalef(.2, .2, .2);glTranslated(31.5,1,25.2);glRotated(90,0,1,0);draw_chaka(0,0,0);glPopMatrix(); //back_left chaka glutSolidTorus
}

void fountain()
{
    glTranslatef(-1,0,-1);
    glEnable(GL_TEXTURE_2D);    LoadTexture1("D:\\Texture\\copp.bmp"); //water
    glPushMatrix();glScalef(1.1, 1, 1.1);glTranslated(12,-.1,3);glRotated(90,1,0,0);draw_chaka(.3,.3,.6);glPopMatrix(); //char pasher bera
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);    LoadTexture1("D:\\Texture\\water4.bmp"); //water
    glPushMatrix();
    glTranslatef(13.15,.15,3.2);
    glScalef(0.5, 3, 0.5);
    glRotatef(90,1,0,0);
    GLfloat no_matt[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambientt[] = { 1, 1, 1.0, 1 };
    GLfloat mat_diffusett[] = { 1, 1, 1.0, 1.0 };
    GLfloat mat_speculartt[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininesstt[] = {60};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambientt);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffusett);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_speculartt);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininesstt);

    GLUquadric *qd1s;
    qd1s = gluNewQuadric();
    gluQuadricTexture(qd1s, TRUE);
    gluDisk(qd1s,0,2,20,40);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

static GLfloat v_pyramid[5][3] =
{
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 3.0},
    {3.0, 0.0, 3.0},
    {3.0, 0.0, 0.0},
    {1.5, 3.0, 1.5}
};

static GLubyte p_Indices[4][3] =
{
    {4, 1, 2},
    {4, 2, 3},
    {4, 3, 0},
    {4, 0, 1}
};

static GLubyte PquadIndices[1][4] =
{
    {0, 3, 2, 1}
};


void drawpyramid(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX, GLfloat ambY, GLfloat ambZ, GLfloat shine)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {shine};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    glBegin(GL_TRIANGLES);
    for (GLint i = 0; i <4; i++)
    {
        getNormal3p(v_pyramid[p_Indices[i][0]][0], v_pyramid[p_Indices[i][0]][1], v_pyramid[p_Indices[i][0]][2],
                    v_pyramid[p_Indices[i][1]][0], v_pyramid[p_Indices[i][1]][1], v_pyramid[p_Indices[i][1]][2],
                    v_pyramid[p_Indices[i][2]][0], v_pyramid[p_Indices[i][2]][1], v_pyramid[p_Indices[i][2]][2]);
        glVertex3fv(&v_pyramid[p_Indices[i][0]][0]);
        glVertex3fv(&v_pyramid[p_Indices[i][1]][0]);
        glVertex3fv(&v_pyramid[p_Indices[i][2]][0]);
    }
    glEnd();

    glBegin(GL_QUADS);
    for (GLint i = 0; i <1; i++)
    {
        getNormal3p(v_pyramid[PquadIndices[i][0]][0], v_pyramid[PquadIndices[i][0]][1], v_pyramid[PquadIndices[i][0]][2],
                    v_pyramid[PquadIndices[i][1]][0], v_pyramid[PquadIndices[i][1]][1], v_pyramid[PquadIndices[i][1]][2],
                    v_pyramid[PquadIndices[i][2]][0], v_pyramid[PquadIndices[i][2]][1], v_pyramid[PquadIndices[i][2]][2]);
        glVertex3fv(&v_pyramid[PquadIndices[i][0]][0]);
        glVertex3fv(&v_pyramid[PquadIndices[i][1]][0]);
        glVertex3fv(&v_pyramid[PquadIndices[i][2]][0]);
        glVertex3fv(&v_pyramid[PquadIndices[i][3]][0]);
    }
    glEnd();

}

void drawCylinder(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX, GLfloat ambY, GLfloat ambZ, GLfloat shine=40, GLdouble b_rad=.2, GLdouble t_rad=.2, GLdouble height=6)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {shine};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv( GL_FRONT, GL_EMISSION, no_mat);

    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    gluCylinder(quadratic, b_rad, t_rad, height, 32, 32);
     //gluCylinder()
}

void Curve ( double t,  float xy[2])
{
    double y=0;
    double x=0;
    t=t>1.0?1.0:t;
    for(int i=0; i<=13; i++)
    {
        int ncr=nCr(13,i);
        double oneMinusTpow=pow(1-t,double(13-i));
        double tPow=pow(t,double(i));
        double coef=oneMinusTpow*tPow*ncr;
        x+=coef*ctrlpoints3[i][0];
        y+=coef*ctrlpoints3[i][1];

    }
    xy[0] = float(x);
    xy[1] = float(y);

    //return y;
}

void bottleBezier3()
{
    int i, j;
    float x, y, z, r;				//current coordinates
    float x1, y1, z1, r1;			//next coordinates
    float theta;
    const float startx = 0, endx = ctrlpoints3[13][0];
    //number of angular slices
    const float dx = (endx - startx) / nt;	//x step size
    const float dtheta = 2*PI / ntheta;		//angular step size

    float t=0;
    float dt=1.0/nt;
    float xy[2];
    Curve( t,  xy);
    x = xy[0];
    r = xy[1];
   // rotate about z-axis

    float p1x,p1y,p1z,p2x,p2y,p2z;
    for ( i = 0; i < nt; ++i )  			//step through x
    {
        theta = 0;
        t+=dt;
        Curve( t,  xy);
        x1 = xy[0];
        r1 = xy[1];

        glBegin( GL_QUAD_STRIP );
        for ( j = 0; j <= ntheta; ++j )
        {
            theta += dtheta;
            double cosa = cos( theta );
            double sina = sin ( theta );
            y = r * cosa;
            y1 = r1 * cosa;	//current and next y
            z = r * sina;
            z1 = r1 * sina;	//current and next z

        glTexCoord2f((cos(i)*r) * 0.5 + 0.5, (sin(i)*r) * 0.5 + 0.5);
            glVertex3f (x, y, z);
            if(j>0)
            {
                getNormal3p(p1x,p1y,p1z,p2x,p2y,p2z,x, y, z);
            }
            else
            {
                p1x=x; p1y=y; p1z=z; p2x=x1; p2y=y1; p2z=z1;
            }
            glTexCoord2f((cos(i)*r1) * 0.5 + 0.5, (sin(i)*r1) * 0.5 + 0.5);
            glVertex3f (x1, y1, z1);

            //forms quad with next pair of points with incremented theta value
        }
        glEnd();
        x = x1;
        r = r1;
    } //for i

}

void pianobalbar(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX=0, GLfloat ambY=0, GLfloat ambZ=0, GLfloat shine=100)
{
    glPushMatrix();
    //glViewport(0, 0, 400, 400);
    //glTranslatef(56.8,49.7,94.5);
    glTranslatef(-5,2.3,0);
    glScalef(1.5, 2, 1.5);
    glRotatef(90,0,0,1);

    GLfloat no_matbz[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambientbz[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffusebz[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specularbz[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininessbz[] = {100};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambientbz);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffusebz);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specularbz);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininessbz);

    bottleBezier3();
    glPopMatrix();
}
void piano()
{
    glTranslatef(-2.5,0,-1);
    glEnable(GL_TEXTURE_2D);    LoadTexture1("D:\\Texture\\piano.bmp");
    glPushMatrix();glScalef(.6, .06, .3); glTranslatef(-.715,20,-17);glRotatef(-5,0,1,0); drawCube1(.3,.4,.4, .3,.4,.4);glPopMatrix(); //base
    glPushMatrix();glRotatef(-20,0,0,1);glScalef(.6, .008, .3); glTranslatef(-1.9,210,-16.7);draw_tra(.3,.4,.4, .3,.4,.4);glPopMatrix(); //uporer baka ota
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);    LoadTexture1("D:\\Texture\\piano_key.bmp");
    glPushMatrix();glScalef(.59, .03, .07); glTranslatef(-.64,40,-75);glRotatef(-3,0,1,0); drawCube1(1, 1.0, 1.0, 1.0,1.0,1.0);glPopMatrix(); //key
    glDisable(GL_TEXTURE_2D);
    //glPushMatrix();glRotatef(90, 0, 1, 1);glScalef(.5, .1, .2); glTranslatef(20,150,-140);drawCylinder(0.5, 0.2, 0.2,  0.2,0.2,0.2   ,40, .16,.2,6);glPopMatrix();

    glEnable(GL_TEXTURE_2D);    LoadTexture1("D:\\Texture\\piano.bmp");
    glPushMatrix();glScalef(.02, .2, .2); glTranslatef(33,6,-25);glRotatef(90, 1, 0, 0);drawCube1(1,1,1,1,1,1); glPopMatrix();
    glPushMatrix();glScalef(.02, .2, .2); glTranslatef(-10,6,-25);glRotatef(90, 1, 0, 0);drawCube1(1,1,1,1,1,1); glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    //glPushMatrix(); glScalef(.05,.05,.05); glTranslatef(1,4,-100); pianobalbar(.01,.04,.04,.01,.04,.04); glPopMatrix();// leg back left
    //glPushMatrix(); glScalef(.05,.05,.05); glTranslatef(27,4,-100); pianobalbar(.01,.04,.04,.01,.04,.04); glPopMatrix();// leg back right
    //glPushMatrix(); glScalef(.05,.05,.05); glTranslatef(-1,4,-86); pianobalbar(.01,.04,.04,.01,.04,.04); glPopMatrix();// leg front left
    //glPushMatrix(); glScalef(.05,.05,.05); glTranslatef(26,4,-86); pianobalbar(.01,.04,.04,.01,.04,.04); glPopMatrix();// leg front right


}

void car_ceiling_balbar()
{
    //glPushMatrix();
    //glTranslatef(220,-1.5,70);
    //glScalef(5, 5.4, 5);
    //glRotatef(90,0,0,1);

    GLfloat no_matbz[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambientbz[] = { 1, 0.9, 0.9, 0.5 };
    GLfloat mat_diffusebz[] = { 1, 0.9, 0.9, 1.0 };
    GLfloat mat_specularbz[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininessbz[] = {100};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambientbz);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffusebz);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specularbz);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininessbz);

    bottleBezier3();
}
void grass()
{
    glEnable(GL_TEXTURE_2D); LoadTexture1("D:\\Texture\\ceiling2.bmp"); //ceiling vitorer
    glPushMatrix(); glScalef(3.5, .01, 6.0); glTranslatef(-.7,360,-1.5); drawCube1(1,1,1, 1,1,1);glPopMatrix(); //ceiling
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D); LoadTexture1("D:\\Texture\\light.bmp"); //ceiling bairer
    glPushMatrix(); glScalef(3.5, .01, 6.0); glTranslatef(-.7,361,-1.5); drawCube1(1,1,1, 1,1,1);glPopMatrix(); //ceiling
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D); //car ceiling
    LoadTexture1("D:\\Texture\\light.bmp");
    glPushMatrix();glScalef(1.5, .1, 3.5); glTranslatef(5,25,-.5);drawCube1(1, 1, 1, 1, 1, 1);glPopMatrix(); //car ceiling
    glDisable(GL_TEXTURE_2D);

    /*glPushMatrix(); glScalef(.05,.05,.05); glTranslatef(1,4,-100);  // left thamba
    glPushMatrix(); glTranslatef(220,-1.5,70); glScalef(5, 5.4, 5); glRotatef(90,0,0,1); car_ceiling_balbar(); glPopMatrix(); glPopMatrix();// left thamba
    glPushMatrix(); glScalef(.05,.05,.05); glTranslatef(1,4,-100);
    glPushMatrix(); glTranslatef(220,-1.5,270); glScalef(5, 5.4, 5); glRotatef(90,0,0,1); car_ceiling_balbar(); glPopMatrix(); glPopMatrix();// right thamba*/

    glPushMatrix();glScalef(3.5, .01, 6.0); glTranslatef(-.7,-0.35,-1.18);drawCube1(0.07, .07, 0.07, 0.07, .07, 0.07);glPopMatrix(); //floor

    glEnable(GL_TEXTURE_2D); //bairer floor
    LoadTexture1("D:\\Texture\\floor2.bmp");
    glPushMatrix();glScalef(4.5, .01, 6.0); glTranslatef(-1,-0.4,-1.3);drawCube1(1,1,1, 1,1,1);glPopMatrix(); //floor
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    LoadTexture2("D:\\Texture\\download (1).bmp");  //grass
    glPushMatrix();glScalef(3.9, .01, 5.9); glTranslatef(-.9,-0.4,1.51);drawCube1(.5,.6,.5, .3,1,.3);glPopMatrix();//front
    glPushMatrix();glScalef(3.9, .01, 5.9); glTranslatef(-3,-0.4,1.51);drawCube1(.5,.6,.5, .3,1,.3);glPopMatrix();//front left
    glPushMatrix();glScalef(4.3, .01, 5.9); glTranslatef(1.9,-0.4,1.51);drawCube1(.5,.6,.5, .3,1,.3);glPopMatrix();//front right

    glPushMatrix();glScalef(10, .01, 5.9); glTranslatef(0,-0.4,-3.75);drawCube1(.5,.6,.5, .3,1,.3);glPopMatrix();//back right
    glPushMatrix();glScalef(10, .01, 5.9); glTranslatef(-3,-0.4,-3.75);drawCube1(.5,.6,.5, .3,1,.3);glPopMatrix();//back left
    glPushMatrix();glScalef(3.9, .01, 5.9); glTranslatef(-3.6,-0.4,-.9);drawCube1(.5,.6,.5, .3,1,.3);glPopMatrix();//left

    glPushMatrix();glScalef(10, .01, 5.9); glTranslatef(0,-0.4,-.9);drawCube1(.5,.6,.5, .3,1,.3);glPopMatrix();//right
    glDisable(GL_TEXTURE_2D);
}

void wall()
{
    //room er vitore
    glPushMatrix();glScalef(.06, 1.2, 5.25); glTranslatef(-40,0,-1.35);drawCube1(1,1,1, 1,1,1);glPopMatrix();//left
    glPushMatrix();glScalef(.06, 1.2, 1.2); glTranslatef(123,0,-6.05);drawCube1(1,1,1, 1,1,1);glPopMatrix();//right of gate
    glPushMatrix();glScalef(.06, 1.2, 3.5); glTranslatef(123,0,-.5);drawCube1(1,1,1, 1,1,1);glPopMatrix();//left of gate
    glPushMatrix();glScalef(3.3, 1.2, .06); glTranslatef(-.73,0,145);drawCube1(1,1,1, 1,1,1);glPopMatrix();//front
    glPushMatrix();glScalef(3.3, 1.2, .06); glTranslatef(-.73,0,-120);drawCube1(1,1,1, 1,1,1);glPopMatrix();//back


    glEnable(GL_TEXTURE_2D);    LoadTexture1("D:\\Texture\\wall1.bmp"); //room er baire
    glPushMatrix();glScalef(.06, 1.2, 5.25); glTranslatef(-39.5,0,-1.35);drawCube1(.5,.5,.5, .5,.5,.7);glPopMatrix();//left
    glPushMatrix();glScalef(.065, 1.2, 1.2); glTranslatef(112.7,0,-6.05);drawCube1(.5,.5,.5, .5,.5,.7);glPopMatrix();//right of gate
    glPushMatrix();glScalef(.065, 1.2, 3.5); glTranslatef(112.7,0,-.5);drawCube1(.5,.5,.5, .5,.5,.7);glPopMatrix();//left of gate
    glPushMatrix();glScalef(3.3, 1.2, .06); glTranslatef(-.73,0,144);drawCube1(1,1,1, 1,1,1);glPopMatrix();//front
    glPushMatrix();glScalef(3.3, 1.2, .06); glTranslatef(-.73,0,-119);drawCube1(1,1,1, 1,1,1);glPopMatrix();//back
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);    LoadTexture2("D:\\Texture\\audi1.bmp");
    glPushMatrix();glScalef(1.6,.5, .03); glTranslatef(3.2,6,300); glRotatef(180,0,0,1);drawCube2(1, 1, 1, 1.0,1.0,1.0);glPopMatrix(); //poster
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);    LoadTexture1("D:\\Texture\\bench.bmp");
    glPushMatrix();glScalef(.04, 1.2, .63); glTranslatef(185,0,-5.8);glRotatef(door_angle,0,1,0);drawCube1(0.7, 0.5, 0.5, 0.7,0.4,0.4);glPopMatrix();// door
    glDisable(GL_TEXTURE_2D);
}

void sky()
 {
     glEnable(GL_TEXTURE_2D); //sky
    LoadTexture1("D:\\Texture\\sky13.bmp");
    glPushMatrix(); glTranslatef(6,5,-9); glRotatef(sky_angle,0,1,0); sphere(1,1,1, 50); glPopMatrix();
    glDisable(GL_TEXTURE_2D);
 }

void seat()
{
    float a=-.5,b=-4,c=-1,d=-1,e=-.5,f=-4;
    for (int i=0;i<10;i++)
    {
    glEnable(GL_TEXTURE_2D); LoadTexture1("D:\\Texture\\sofa6.bmp");    //sky
    glPushMatrix();glScalef(1.4, .1, .2); glTranslatef(-1.5,a,b);if(i%2==0)drawCube1(1,0,0, 1,0,0); else drawCube1(1,.5,.2, 1,.5,.2); glPopMatrix();//left
    glPushMatrix();glScalef(1.4, .2, .1); glTranslatef(-1.5,e,f);if(i%2==0)drawCube1(1,0,0, 1,0,0); else drawCube1(1,.5,.2, 1,.5,.2); glPopMatrix();//left helan deyar place

    glPushMatrix();glScalef(1.4, .1, .2); glTranslatef(2.2,a,b);if(i%2==0)drawCube1(1,0,0, 1,0,0); else drawCube1(1,.2,.2, 1,.2,.2);glPopMatrix();//1 right
    glPushMatrix();glScalef(1.4, .2, .1); glTranslatef(2.2,e,f);if(i%2==0)drawCube1(1,0,0, 1,0,0); else drawCube1(1,.2,.2, 1,.2,.2); glPopMatrix();//left helan deyar place
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();glScalef(3.2, .08, .25); glTranslatef(-.7,c,d);if(i%2==0)drawCube1(0.2, .2, 0.2, 0.2, .2, 0.2);else drawCube1(0.15, .15, 0.15, 0.15, .15, 0.2);glPopMatrix();//stair
    a+= 1.8; b+=5; c+=1.6; d+=3.2; e+=.9; f+=10;
    }
    glPushMatrix();glScalef(3.2, .08, .25); glTranslatef(-.7,c,d);drawCube1(0.15, .15, 0.15, 0.15, .15, 0.2);glPopMatrix();//stair
}

float k1=-15,k2=-12,k3=-9,k4=-6,k5=-3,k6=0,k7=3,k8=6,k9=9,k10=12,k11=15;
float k12=18,k13=21,k14=24,k15=27,k16=30,k17=33,k18=36,k19=39,k20=42,k21=45,k22=48,k23=51;
void stage()
{
    glEnable(GL_TEXTURE_2D);    LoadTexture2("D:\\Texture\\stage_floor.bmp");
    glPushMatrix();glScalef(3.3, .3, 1); glTranslatef(-.715,-1,-7);drawCube1(.3,.25, .08, .3,.25, .08);glPopMatrix();//stage
    glPushMatrix();glScalef(.7, .14, .3); glTranslatef(2.5,-1.5,-15.555);drawCube1(.3,.2, .05, .3,.2, .06);glPopMatrix(); //stage stair
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);    LoadTexture2("D:\\Texture\\poster.bmp");
    glPushMatrix();glScalef(1,.8, .03); glTranslatef(5,5,-217); glRotatef(180,0,0,1);drawCube1(1, 1, 1, 1.0,1.0,1.0);glPopMatrix(); //poster
    glDisable(GL_TEXTURE_2D);

    //for (GLint i=-15;i<=15;i+=3)    ///kaj kore na
        //glPushMatrix();glScalef(.15,1, .005); glTranslatef(i,.63,-750); drawCube1(1, 0, 0, 1.0,0.0,0.0);glPopMatrix();
    glEnable(GL_TEXTURE_2D);    LoadTexture2("D:\\Texture\\curtain2.bmp");
    glPushMatrix();glScalef(.15,1, .005); glTranslatef(k1,.63,-750); drawCube1(1, 0, 0, 1.0,0.0,0.0);glPopMatrix(); //porda 1
    glPushMatrix();glScalef(.15,1, .005); glTranslatef(k2,.63,-750); drawCube1(1, 0.5, 0, 1.0,0.0,0.0);glPopMatrix();
    glPushMatrix();glScalef(.15,1, .005); glTranslatef(k3,.63,-750); drawCube1(1, 0.0, 0, 1.0,0.0,0.0);glPopMatrix(); //porda 1
    glPushMatrix();glScalef(.15,1, .005); glTranslatef(k4,.63,-750); drawCube1(1, 0.5, 0, 1.0,0.0,0.0);glPopMatrix();
    glPushMatrix();glScalef(.15,1, .005); glTranslatef(k5,.63,-750); drawCube1(1, 0, 0, 1.0,0.0,0.0);glPopMatrix(); //porda 1
    glPushMatrix();glScalef(.15,1, .005); glTranslatef(k6,.63,-750); drawCube1(1, 0.5, 0, 1.0,0.0,0.0);glPopMatrix();

    glPushMatrix();glScalef(.15,1, .005); glTranslatef(k7,.63,-750); drawCube1(1, 0, 0, 1.0,0.0,0.0);glPopMatrix(); //porda 1
    glPushMatrix();glScalef(.15,1, .005); glTranslatef(k8,.63,-750); drawCube1(1, 0.5, 0, 1.0,0.0,0.0);glPopMatrix();
    glPushMatrix();glScalef(.15,1, .005); glTranslatef(k9,.63,-750); drawCube1(1, 0, 0, 1.0,0.0,0.0);glPopMatrix(); //porda 1
    glPushMatrix();glScalef(.15,1, .005); glTranslatef(k10,.63,-750); drawCube1(1, 0.5, 0, 1.0,0.0,0.0);glPopMatrix();
    glPushMatrix();glScalef(.15,1, .005); glTranslatef(k11,.63,-750); drawCube1(1, 0, 0, 1.0,0.0,0.0);glPopMatrix(); //porda 1

    ///.......................................................
    glPushMatrix();glScalef(.15,1, .005); glTranslatef(k12,.63,-750); drawCube1(1, 0.5, 0, 1.0,0.0,0.0);glPopMatrix();

    glPushMatrix();glScalef(.15,1, .005); glTranslatef(k13,.63,-750); drawCube1(1, 0, 0, 1.0,0.0,0.0);glPopMatrix(); //porda 1
    glPushMatrix();glScalef(.15,1, .005); glTranslatef(k14,.63,-750); drawCube1(1, 0.5, 0, 1.0,0.0,0.0);glPopMatrix();
    glPushMatrix();glScalef(.15,1, .005); glTranslatef(k15,.63,-750); drawCube1(1, 0, 0, 1.0,0.0,0.0);glPopMatrix(); //porda 1
    glPushMatrix();glScalef(.15,1, .005); glTranslatef(k16,.63,-750); drawCube1(1, 0.5, 0, 1.0,0.0,0.0);glPopMatrix();
    glPushMatrix();glScalef(.15,1, .005); glTranslatef(k17,.63,-750); drawCube1(1, 0, 0, 1.0,0.0,0.0);glPopMatrix(); //porda 1
    glPushMatrix();glScalef(.15,1, .005); glTranslatef(k18,.63,-750); drawCube1(1, 0.5, 0, 1.0,0.0,0.0);glPopMatrix();

    glPushMatrix();glScalef(.15,1, .005); glTranslatef(k19,.63,-750); drawCube1(1, 0, 0, 1.0,0.0,0.0);glPopMatrix(); //porda 1
    glPushMatrix();glScalef(.15,1, .005); glTranslatef(k20,.63,-750); drawCube1(1, 0.5, 0, 1.0,0.0,0.0);glPopMatrix();
    glPushMatrix();glScalef(.15,1, .005); glTranslatef(k21,.63,-750); drawCube1(1, 0, 0, 1.0,0.0,0.0);glPopMatrix(); //porda 1
    glPushMatrix();glScalef(.15,1, .005); glTranslatef(k22,.63,-750); drawCube1(1, 0.5, 0, 1.0,0.0,0.0);glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}


static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-12.0, 12.0, -1.0, 20.0, -15.0, 15.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void makecylinder(float height,float Base)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { .6,.4,.2, 1.0 };
    GLfloat mat_diffuse[] = { .6,.4,.2, 1.0 };
    GLfloat mat_specular[] = { 1, 1,1 , 1.0 };
    GLfloat mat_shininess[] = {60};
//front phase niye kaaj hoche
    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

	GLUquadricObj *qobj;
	qobj = gluNewQuadric();
	//glColor3f(247/255, 187/255, 96/255);
	glPushMatrix();
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	gluCylinder(qobj, Base, Base - (0.2 * Base), height, 20, 20);
	glPopMatrix();
}
void maketree(float height,float Base)
{

	glPushMatrix();
	float angle;
	makecylinder(height, Base);
	glTranslatef(0.0f, height,0.0f);
	height -= height * 0.2f;
	Base -= Base * 0.3f;

	if (height > 1)
	{
		angle = 22.5f;
		glPushMatrix();
		glRotatef(angle, -1.0f, 0.0f, 0.0f);
		maketree(height, Base);
		glPopMatrix();
		glPushMatrix();
		glRotatef(angle, 0.5f, 0.0f, 0.866f);
		maketree(height, Base);
		glPopMatrix();
		glPushMatrix();
		glRotatef(angle, 0.5f, 0.0f, -0.866f);
		maketree(height, Base);
		glPopMatrix();
	}
	glPopMatrix();
}

void tree()
{
    glPushMatrix(); glRotatef(angle, 0, 1, 0); glRotatef(angle2, 0, 1, 0); glTranslatef(5,0,-11); maketree(4.0f,0.1f); glPopMatrix();
    glPushMatrix(); glRotatef(angle+2, 0, 1, 0); glRotatef(angle2+4, 0, 1, 0); glTranslatef(3,-2,-12); maketree(4.0f,0.1f); glPopMatrix();
}

thamba()
{
    glPushMatrix();glScalef(.5, .45, .5); glTranslatef(21,6,16);glRotatef(90, 1, 0, 0);drawCylinder(1, 1, 1,  0.8,0.8,0.8   ,40, .4,.5,6);glPopMatrix();
    glPushMatrix();glScalef(.5, .45, .5); glTranslatef(21,6,-2);glRotatef(90, 1, 0, 0);drawCylinder(1, 1, 1,  0.8,0.8,0.8   ,40, .4,.5,6);glPopMatrix();
}
void lightOne()
{
    glPushMatrix();
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {0.5, 0.5, 0.5, 1.0};
    GLfloat light_diffuse[]  = { .80, .80, .80, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 4.0, 20, 10.0, 1 }; //5 5 10

    //glEnable( GL_LIGHT0);

    if(amb1 == true){glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);}
    else{glLightfv( GL_LIGHT0, GL_AMBIENT, no_light);}

    if(diff1 == true){glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse);}
    else{glLightfv( GL_LIGHT0, GL_DIFFUSE, no_light);}

    if(spec1 == true){glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);}
    else{glLightfv( GL_LIGHT0, GL_SPECULAR, no_light);}

    glLightfv( GL_LIGHT0, GL_POSITION, light_position);
    glPopMatrix();
}

void lampLight()
{
    glPushMatrix();
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {0.5, 0.5, 0.5, 1.0};
    GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 10, 50, 100, 1.0 };  //0.7, 4.5, 9.0

    //glEnable( GL_LIGHT2);

    if(amb3 == true){glLightfv( GL_LIGHT2, GL_AMBIENT, light_ambient);}
    else{glLightfv( GL_LIGHT2, GL_AMBIENT, no_light);}

    if(diff3 == true){glLightfv( GL_LIGHT2, GL_DIFFUSE, light_diffuse);}
    else{glLightfv( GL_LIGHT2, GL_DIFFUSE, no_light);}

    if(spec3 == true){glLightfv( GL_LIGHT2, GL_SPECULAR, light_specular);}
    else{glLightfv( GL_LIGHT2, GL_SPECULAR, no_light);}

    glLightfv( GL_LIGHT2, GL_POSITION, light_position);
    GLfloat spot_direction[] = { 0.3, -1, -0.8 };
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
    glLightf( GL_LIGHT2, GL_SPOT_CUTOFF, 35.0);
    glPopMatrix();
}

char keyboardbuffer[20][40];
int keyboardindex_x[20];
int keyboardindex_y;

// I use this to put text on the screen
void Sprint( int x, int y, char *st) //display text
{
    int l,i;
    glRotatef(90,0,1,0);
    glColor3f(1.0, 1.0, 1.0);

    l=strlen( st ); // see how many characters are in text string.
    glRasterPos2i( x, y); // location to start printing text
    for( i=0; i < l; i++) // loop until i is greater then l
        {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]); // Print a character on the screen
        }

}

static void TimeEvent(int te) //display text
{
    glutPostRedisplay(); // Update screen with new data
    glutTimerFunc( 100, TimeEvent, 1); // Reset our timmer.
}

// Setup our Opengl world, called once at startup.
void init(void) //display text
{
    int i;
    glClearColor (0.0, 0.0, 0.0, 0.0); // When screen cleared, use black.
    glShadeModel (GL_SMOOTH); // How the object color will be rendered smooth or flat
    glEnable(GL_DEPTH_TEST); // Check depth when rendering
    glColor3f(1.0, 1.0, 1.0);
    for (i = 0; i < 20; i++)
        {
        keyboardindex_x[i] = 0;
        }
    keyboardindex_y = 8;
}


void update()  //fountain
{
    xb+=0.05; yb+=0.05;     if(xb>1.5) { xb=0.0; yb=0.0; }
    xc-=0.055; yc+=0.05;    if(xc<-2.5) { xc=0.0; yc=0.0; }
    xd+=0.052; yd+=0.05;    if(yd>2.0) { xd=0.0; yd=0.0; }
    xe-=0.042; ye+=0.05;    if(ye>2.0) { xe=0.0; ye=0.0; }
    xf+=0.052; yf+=0.05;    if(yf>2.0) { xf=0.0; yf=0.0; }
    xg-=0.032; yg+=0.05;    if(yg>1.0) { xg=0.0; yg=0.0; }
    xh+=0.052; yh+=0.05;    if(xh>1.5) { xh=0.0; yh=0.0; }
    xi-=0.25; yi+=0.05;    if(yi>2.0) { xi=0.0; yi=0.0; }
    xj+=0.052; yj+=0.04;    if(yj>2.2) { xj=0.0; yj=0.0; }
    //xk+=0.052;
     yk+=0.05;    if(yk>2.0) { xk=0.0; yk=0.0; }
    //xl+=0.052;
    yl+=0.05;    if(yl>2.5) { xl=0.0; yl=0.0; }
}

void makeBubbles()  //fountain
{
    glTranslatef(16,0,2.5);
    glPushMatrix(); glTranslatef(xb,yb-5,-5.0); sphere(.45,1,1, .1); glPopMatrix();
    glPushMatrix(); glTranslatef(xc+0.03,yc-5,-5.0); sphere(.45,1,1, .1); glPopMatrix();
    glPushMatrix(); glTranslatef(xd-0.03,yd-5,-5.0); sphere(.45,1,1, .1); glPopMatrix();

    glPushMatrix(); glTranslatef(xe,ye-5,-5.0); sphere(.45,1,1, .1); glPopMatrix();
    glPushMatrix(); glTranslatef(xf+0.03,yf-5,-5.0); sphere(.45,1,1, .1); glPopMatrix();
    glPushMatrix(); glTranslatef(xg-0.03,yf-5,-5.0); sphere(.45,1,1, .1); glPopMatrix();

    glPushMatrix(); glTranslatef(xh,yh-5,-5.0); sphere(.45,1,1, .1); glPopMatrix();
    glPushMatrix(); glTranslatef(xi+0.03,yi-5,-5.0); sphere(.45,1,1, .1); glPopMatrix();
    glPushMatrix(); glTranslatef(xj-0.03,yj-5,-5.0); sphere(.45,1,1, .1); glPopMatrix();

    glPushMatrix(); glTranslatef(xk+0.03,yk-5,-5.0); sphere(.45,1,1, .1); glPopMatrix();
    glPushMatrix(); glTranslatef(xl-0.03,yl-5,-5.0); sphere(.45,1,1, .1); glPopMatrix();
    update();
}


void Roll() // move around z axis ,r e
{
    upx = 280.0*(cos(roll_angle*3.1416/180.0));
    upy = 280.0*(sin(roll_angle*3.1416/180.0));
}

void Pitch() // move around x axis , p o
{   centery = 280.0*(cos(pitch_angle*3.1416/180.0));}

void Yaw() // move around y axis , y t
{   centerx = 280.0*(cos(yaw_angle*3.1416/180.0));}


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    //glFrustum(-5,5,-5,5,1,500);
    gluPerspective(90,1.5,1,500);
    //gluPerspective()
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    //gluLookAt(eyeX,eyeY,eyeZ,  refX,refY,refZ,  0,1,0); //7,2,13, -3,-1,0, 0,1,0
    gluLookAt(eyex,eyey,eyez, centerx,centery,centerz, upx,upy,upz);
    glViewport(0, 0, 1500, 1000);
    glScalef(zoom,zoom,zoom);
    glColor3f(1.0, 1.0, 1.0);
    for (int i = 0; i < 20; i++)
        {
        Sprint(-8, 8 - i, keyboardbuffer[i]);
        }
    glEnable(GL_LIGHTING);
    lightOne();
    lampLight();


    grass();
    thamba();
    stage();
    seat();
    wall();
    tree();
    car();
    fountain();
    piano();
    //lightBulb1();
    //glDisable(GL_LIGHTING);................
    sky();
    makeBubbles();

    glFlush();
    glutSwapBuffers();
}

void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 'w': // move eye point upwards along Y axis
        eyey+=1.0;break;
    case 's': // move eye point downwards along Y axis
        eyey-=1.0;break;
    case 'a': // move eye point left along X axis
        eyez-=1.0;break;
    case 'd': // move eye point right along X axis
        eyez+=1.0;break;
    case 'y': // move eye point left along X axis
        eyex-=1.0;break;
    case 'u': // move eye point right along X axis
        eyex+=1.0;break;

    case 'K':
        porda_k = true; break;
    case 'D':
        if(door_open == false) {door_open=true; break;}
            else{door_open=false; break;}
    case '1': //to turn on and off light one
            if(amb1==false)
            {
                 amb1=true; diff1=true; spec1=true;
                glEnable( GL_LIGHT0); break;
            }
            else if( amb1==true)
            {
                 amb1=false; diff1=false; spec1=false; glDisable( GL_LIGHT0); break;
            }
    case '3': //to turn on and off light one
            if(switchLamp == false)
            {
                switchLamp = true; amb3=true; diff3=true; spec3=true;
                glEnable( GL_LIGHT2); break;
            }
            else if(switchLamp == true)
            {
                switchLamp = false; amb3=false; diff3=false; spec3=false;
                glDisable( GL_LIGHT2); break;
            }
    case'4': //turn on/off ambient light 1
            if(amb1 == false) {amb1=true; break;}
            else{amb1=false; break;}
    case'5':
            if(diff1 == false) {diff1=true; break;}
            else{diff1=false; break;}
    case'6':
            if(spec1 == false) {spec1=true; break;}
            else{spec1=false; break;}

    case'e': //turn on/off ambient lamp light
            if(amb3 == false) {amb3=true; break;}
            else{amb3=false; break;}
    case'r':
            if(diff3 == false) {diff3=true; break;}
            else{diff3=false; break;}
    case't':
            if(spec3 == false) {spec3=true; break;}
            else{spec3=false; break;}
     case 'f': //turn on/off fan
            if(fanSwitch == false) {
                fanSwitch = true; break;
            }
            else{
                fanSwitch = false; break;
            }
     case 'h':
        if(bird== false)
        {
            eyey= 10;centery=-30; bird=true;
        }
        else {eyey= 2.5;centery=2; bird=false;} break;
    case '8':
        zoom+=.1;break;
    case '9':
        zoom-=.1;break;

    case 'z':
        pitch_angle += increment;
        Pitch();break;

    case 'x':
        pitch_angle -= increment;
        Pitch();break;

    case 'c':
        yaw_angle +=  increment;
        Yaw(); break;

    case 'v':
        yaw_angle -=  increment;
        Yaw(); break;

    case 'b':
        roll_angle +=  increment;
        Roll(); break;

    case 'n':
        roll_angle -=  increment;
        Roll();break;
    case 'm':

    case 27:    // Escape key
        exit(1);
    default:
        break;
    }


    glutPostRedisplay();
}

void animate()
{
    sky_angle+= 2;
    if(sky_angle > 360) sky_angle -= 360;

    if(porda_k==true)
        {   k1=k1-.28;k2=k2-.3;k3=k3-.35;k4=k4-.4;k5=k5-.45;k7=k7-.65;k6=k6-.5;k8=k8-.7;k9=k9-.8;k10=k10-.9;k11--;
            if (k1<=-15)k1=-15; if (k2<=-15)k2=-15; if (k3<=-15)k3=-15; if (k4<=-15)k4=-15; if (k5<=-15)k5=-15;if (k6<=-15)k6=-15;if (k7<=-15)k7=-15;if (k8<=-15)k8=-15;if (k9<=-15)k9=-15;if (k10<=-15)k10=-15;if (k11<=-15)k11=-15;

            k12++;k13++;k14++;k15++;k17++;k16++;k18++;k19++;k20++;k21++;k22++;k23++;
            if (k12>=48){k12=48;porda_k=false;}
            if (k13>=48)k13=48; if (k14>=48)k14=48; if (k15>=48)k15=48; if (k16>=48)k16=48; if (k17>=48)k17=48; if (k18>=48)k18=48; if (k19>=48)k19=48; if (k20>=48)k20=48; if (k21>=48)k21=48; if (k22>=48)k22=48; if (k23>=48)k23=48;

        }
    if(door_open==true)
    {
        door_angle= door_angle+10;
        if(door_angle>=90) door_angle=90;
    }
    else if(door_open==false && door_angle>=00)
    {
        door_angle= door_angle-5;
        if(door_angle<=0) door_angle=0;
    }

    makeBubbles();
    glutPostRedisplay();

}


static void idle(void)
{
    glutPostRedisplay();
}

int main (int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(0,00);
    glutInitWindowSize(windowHeight, windowWidth);
    glutCreateWindow("1707087");

    glShadeModel( GL_SMOOTH );
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    //glClearColor(1,1,1,1);

    //init ();   ///display text
    glutDisplayFunc(display);
    glutKeyboardFunc(myKeyboardFunc);
    //glutTimerFunc( 10, TimeEvent, 1);  ///display text
    glutIdleFunc(animate);
    glutMainLoop();

    return 0;
}
