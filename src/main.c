#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#define TIMER_ID 0
#define BR 3
#define TIMER_INTERVAL 10


//Staticke promenljive.
static float change;
static float gd;
static float speed;
static float param;
static float down_param;
static float size_floor;
static float parts[BR];
//Uglovi tela trkaca.
static float lower_arm;
static float upper_arm;
static float lowerr_leg;
static float lowerl_leg;
static float upperl_leg;
static float upperr_leg;
static int animation_ongoing;
static float animation_parameter;

//Deklaracija callback funkcija.
static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int);

int main(int argc, char** argv){

    //Inicijalizuje se GLUT.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    //Kreira se prozor.
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100,100);
    glutCreateWindow(argv[0]);

    //Inicijalizacija parametara

    //Promena smera kretanja delova tela.
    change = 0;

    //Uglovi delova tela.
    lower_arm = 90;
    upper_arm = 0;
    lowerr_leg = 0;
    lowerl_leg = 0;
    upperr_leg = 0;
    upperl_leg = 0;

    //Parametar za pomeranje gore-dole.
    gd = 0.02;


    //Parametar koji odredjuje brzinu animacije
    speed = 1;
    down_param=0;

    //Velicina ploca u sceni.
    size_floor = 300;
    param = 0.2;

    //Parametri animacije.
    animation_ongoing = 0;
    animation_parameter = 0;

    //Registruju se callback funkcije.
    glutKeyboardFunc(on_keyboard);
    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);

    //Pravi se niz koji sadrzi delove scene
    int i;
    for(i=0;i<BR;i++){
        parts[i] = -i*size_floor;
    }

    //Obavlja se OpenGL inicijalizacija.
    glClearColor(0,0,0,0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    //Pozicija svetla
    GLfloat light_position[] = {1, 2, 1, 0};

    //Ambijentalna boja svetla.
    GLfloat light_ambient[] = {0.1, 0.1, 0.1, 1};

    //Difuzna boja svetla
    GLfloat light_diffuse[] = {0.7,0.7,0.7,0.7,1};

    //Spekularna boja svetla
    GLfloat light_specular[] = {0.9, 0.9, 0.9, 1};

    //Koeficijenti ambijentalne refleksije materijala.
    GLfloat ambient_coeffs[] = {0.3, 0.7, 0.3, 1};

    //Koeficijenti difuzne refleksije materijala.
    GLfloat diffuse_coeffs[] = {0.2, 1, 0.2, 1};

    //Koeficijenti spekularne refleksije materijala.
    GLfloat specular_coeffs[] = {1,1,1,1};

    //Koeficijent glatkosti materijala.
    GLfloat shininess = 30;

    //Podesavaju se parametri svetla.
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    //Podesavaju se parametri materijala
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    glEnable(GL_COLOR_MATERIAL);

    //Program ulazi u glavnu petlju.
    glutMainLoop();

    return 0;
}


static void on_keyboard(unsigned char key, int x, int y){
    switch(key){
        case 27:
            //Zavrsava se program.
            exit(0);
            break;
        case 'g':
        case 'G':
            //Pogrece se animacija.
            if(!animation_ongoing){
                animation_ongoing = 1;
                glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
            }
            break;
        case 's':
        case 'S':
            //Zaustavlja se animacija
            animation_ongoing = 0;
            break;
    }
}


static void on_reshape(int width, int height){
    //Podesava se viewport.
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);

    //Podesava se projekcija.
    glLoadIdentity();
    gluPerspective(60, (float)width/height, 1, 1000);
    glutPostRedisplay();
}

static void draw_scene(){

}

static void on_timer(int id){

}

//Funkcija koja crta trkaca.
static void draw_robo(void){
    GLUquadricObj *quad;
    quad = gluNewQuadric();
    glPushMatrix();

    //glRotatef(180,0,1,0);
    glTranslatef(0,2.85,0);

    //gd pomera trkaca gore-dole za male vrednosti.
    glTranslatef(0, gd, 0);

    //glRotatef(180,0,1,0);

    {//Gornji deo.
        glPushMatrix();
            //Gornji deo tela.
            glPushMatrix();
                glColor4f(1,0,0,1);
                glScalef(1.3,1.6,0.6);
                glutSolidCube(1);
            glPopMatrix();
            //Glava.
            glPushMatrix();
                glColor4f(1,1,0,1);
                glTranslatef(0, 1.2,0);
                glutSolidSphere(0.5, 40, 40);
            glPopMatrix();
            //Ruke.
            //Leva ruka.
            glPushMatrix();
            //Rame.
                glColor4f(1,0,0,1);
                glTranslatef(-0.85,0.7,0);
                glRotatef(upper_arm, 1, -1, 0);
                glutSolidSphere(0.2, 40, 40);
                glPushMatrix();
                //Nadlaktica.
                    glTranslatef(0, -0.19, 0);
                    glRotatef(90, 1, 0, 0);
                    gluCylinder(quad, 0.1, 0.1, 0.5, 50, 50);
                glPopMatrix();
                //Lakat.
                    glTranslatef(0, -0.75, 0);
                    glRotatef(-lower_arm, 1, 0, 0);
                    glutSolidSphere(0.13, 40, 40);

                glPushMatrix();
                //Podlaktica.
                    glTranslatef(0, -0.1,0);
                    glRotatef(90, 1, 0, 0);
                    gluCylinder(quad, 0.1, 0.1, 0.46, 50, 50);
                glPopMatrix();
                glPushMatrix();
                    glColor4f(1,1,0,1);
                    glTranslatef(0, -0.65, 0);
                    glScalef(1.2, 1, 1.4);
                    glutSolidCube(0.2);
                glPopMatrix();
            glPopMatrix();
            //Desna ruka.
            glPushMatrix();
            //Rame.
                glColor4f(1,0,0,1);
                glTranslatef(0.85,0.7,0);
                glRotatef(-upper_arm, 1, 1, 0);
                glutSolidSphere(0.2, 40, 40);
                glPushMatrix();
            //Nadlaktica.
                    glTranslatef(0, -0.19, 0);
                    glRotatef(90, 1, 0, 0);
                    gluCylinder(quad, 0.1, 0.1, 0.5, 50, 50);
                glPopMatrix();
                //glPushMatrix();
            //Lakat.
                    glTranslatef(0, -0.75, 0);
                    glRotatef(-lower_arm, 1, 0, 0);
                    glutSolidSphere(0.13, 40, 40);
                //glPopMatrix();

                glPushMatrix();
            //Podlaktica.
                    glTranslatef(0, -0.1,0);
                    glRotatef(90, 1, 0, 0);
                    gluCylinder(quad, 0.1, 0.1, 0.46, 50, 50);
                glPopMatrix();
                glPushMatrix();
                    glColor4f(1,1,0,1);
                    glTranslatef(0, -0.65, 0);
                    glScalef(1.2, 1, 1.4);
                    glutSolidCube(0.2);
                glPopMatrix();
            glPopMatrix();

        glPopMatrix();

    }
    //Donji deo tela.
    {
    glPushMatrix();
    glColor4f(0,0,1,1);
    glPushMatrix();
            glTranslatef(0, -1.1, 0);
            glScalef(0.5, 0.4, 0.6);
            glutSolidCube(1);
    glPopMatrix();
    //Leva noga.
        glPushMatrix();
        glColor4f(0,0,1,1);
            glTranslatef(-0.450, -1.1, 0);
            glRotatef(-upperl_leg, 1,0,0);
            glutSolidSphere(0.2, 40, 40);
                glPushMatrix();
                //Iznad kolena.
                glTranslatef(0, -0.15, 0);
                glRotatef(90, 1, 0, 0);
                gluCylinder(quad, 0.15,0.15, 0.7,50,50);
                glPopMatrix();
                glPushMatrix();
                //Koleno + potkolenica.
                glTranslatef(0, -0.95, 0);
                glRotatef(lowerl_leg, 1, 0, 0);
                glutSolidSphere(0.16, 40, 40);
                    glPushMatrix();
                        glTranslatef(0, -0.12,0);
                        glRotatef(90, 1, 0, 0);
                        gluCylinder(quad, 0.12,0.12, 0.575,50,50);
                    glPopMatrix();
                    glPushMatrix();
                        glColor4f(1,1,1,1);
                        glTranslatef(0,-0.8,0);
                        glScalef(1.2, 1, 2);
                        glutSolidCube(0.2);
                    glPopMatrix();
                glPopMatrix();
        glPopMatrix();

    //Desna noga.
        glPushMatrix();
        glColor4f(0,0,1,1);
            glTranslatef(0.450, -1.1, 0);
            glRotatef(-upperr_leg, 1,0,0);
            glutSolidSphere(0.2, 40, 40);
                glPushMatrix();
                //Iznad kolena.
                glTranslatef(0, -0.15, 0);
                glRotatef(90, 1, 0, 0);
                gluCylinder(quad, 0.15,0.15, 0.7,50,50);
                glPopMatrix();
                glPushMatrix();
                //Koleno + potkolenica.
                glTranslatef(0, -0.95, 0);
                glRotatef(lowerr_leg, 1, 0, 0);
                glutSolidSphere(0.16, 40, 40);
                    glPushMatrix();
                        glTranslatef(0, -0.12,0);
                        glRotatef(90, 1, 0, 0);
                        gluCylinder(quad, 0.12,0.12, 0.575,50,50);
                    glPopMatrix();
                    glPushMatrix();
                        glColor4f(1,1,1,1);
                        glTranslatef(0,-0.8,0);
                        glScalef(1.2, 1, 2);
                        glutSolidCube(0.2);
                    glPopMatrix();
                glPopMatrix();
        glPopMatrix();
    glPopMatrix();
    }
    glPopMatrix();
}

static void on_display(void){

    //Brise se prethodni sadrzaj prozora
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Podesava se vidna tacka
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(5,7,5,0,0,0,0,1,0);
	//gluLookAt(7,20,-10,0,0,0,0,1,0);

    //glRotatef(180, 0,1,0);

    /* Crtanje robota. */
    draw_robo();
//    draw_scene();
//     glPushMatrix();
//         glColor4f(0.545098,0.270588,0.0745098,1);
//         glScalef(1,0.001,1);
//         glutSolidCube(3);
//     glPopMatrix();
    glPointSize(15);
//

    //Crtam koordinatni sistem
    glPushMatrix();
        glDisable(GL_LIGHTING);
        glBegin(GL_POINTS);
        glColor3f(1,1,1);
        glVertex3f(10, 0, 0);
        glVertex3f(0, 10, 0);
        glVertex3f(0, 0, 10);
        glEnd();
        glEnable(GL_LIGHTING);
    glPopMatrix();

    //Nova slika se salje na ekran.
    glutSwapBuffers();
}
