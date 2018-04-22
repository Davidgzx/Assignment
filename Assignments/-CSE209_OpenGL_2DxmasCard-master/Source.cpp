#include <time.h>
#include <string.h>
#include <GL/glut.h>
#include <math.h>
#include <windows.h>
#include <stack>
#define MAX_CHAR	128
#define PI		3.141592f
using namespace std;
struct point {
	GLfloat x;
	GLfloat y;
};
stack<point>	Stack;
static int	scale = 5;
static int	day = -10;
int		flag = 1;
int		flag1 = 0;
int		n = 0;
GLfloat		R, G, B;
GLfloat		R2, G2, B2;


void mouse_input(int button, int state, int x, int y)
{
	if ((state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)&& (flag == 2 || flag==3))
	{
		if (flag1 < 2)
			flag1 += 1;
		glutPostRedisplay();
	}
	else if ((state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON)&& (flag == 2 || flag==3))
	{
		if (flag1 > 0)
			flag1 -= 1;
		glutPostRedisplay();
	}
}

void drawString(const char* str)
{
	static GLuint lists;
	lists = glGenLists(MAX_CHAR);

	wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);

	for (; *str != '\0'; ++str)
	{
		glCallList(lists + *str);
	}

	glDeleteLists(lists, MAX_CHAR);
}

void selectFont(int size, int charset, int Escapement, const char* face)
{
	HFONT	hFont = CreateFontA(size, 0, Escapement, 0, FW_BLACK, 0, 0, 0, charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT	hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}

void welcome_screen()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
	glColor3f(0, 1, 1);
	selectFont(72, ANSI_CHARSET, 0, "Comic Sans MS");
	glRasterPos2f(-2.0, 1.0);
	drawString("MERRY CHRISTMAS");
	selectFont(48, ANSI_CHARSET, 0, "Comic Sans MS");
	glRasterPos2f(-1.15, 0.6);
	drawString("ZiXuan.Guo15");
	glRasterPos2f(-1.15, 0.25);
	drawString("1508672");
	glColor3f(0, 0.5, 1);
	selectFont(24, ANSI_CHARSET, 0, "Comic Sans MS");
	glRasterPos2f(-1.15, -0.25);
	drawString("Press'w' to continue");
	glRasterPos2f(-1.15, -0.55);
	drawString("Press'g' to enable snow");	
	glRasterPos2f(-1.15, -0.85);
	drawString("Click mouse button to control lights");
}

void reshape(int w, int h)
{
	GLdouble	size;
	GLdouble	aspect;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat ar = (GLfloat)w / h;
	gluPerspective(90, ar, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void background()
{
	GLfloat posY = -2.0f;
	GLfloat posX = -3.0f;
	glPushMatrix();
	glTranslatef(0.0, 0.0, -2.3);
	glShadeModel(GL_SMOOTH);
	glBegin(GL_QUADS);
	glColor3f(217.0 / 255.0, 33.0 / 255.0, 13.0 / 255.0);
	glVertex2f(posX, posY);
	glColor3f(217.0 / 255.0, 33.0 / 255.0, 13.0 / 255.0);
	glVertex2f(-posX, posY);
	glColor3f(176 / 255.0, 196 / 255.0, 222 / 255.0);
	glVertex2f(-posX, -posY);
	glColor3f(176 / 255.0, 196 / 255.0, 222 / 255.0);
	glVertex2f(posX, -posY);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -1.87, -2.29999);
	glBegin(GL_QUADS);
	glColor3f(245 / 255.0, 255 / 255.0, 255 / 255.0);
	glVertex2f(posX, posY / 15);
	glColor3f(245 / 255.0, 255 / 255.0, 255 / 255.0);
	glVertex2f(-posX, posY / 15);
	glColor3f(255 / 255.0, 250 / 255.0, 250 / 255.0);
	glVertex2f(-posX, -posY / 15);
	glColor3f(255 / 255.0, 250 / 255.0, 250 / 255.0);
	glVertex2f(posX, -posY / 15);
	glEnd();
	glPopMatrix();
}

void draw_solid_circle(float x, float y, float radius)
{
	int	count;
	int	sections = 200;

	GLfloat ;
	glPushMatrix();
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);

	for (count = 0; count <= sections; count++)
	{
		glVertex2f(x + radius * cos(count * 2.0f * PI / sections), y + radius * sin(count * 2.0f * PI / sections));
	}
	glEnd();
	glPopMatrix();
}

void drawgift()
{	// The gift at right
	glPushMatrix();
	glTranslated(0.0, -1.7367, -2.2999);
	glScalef(2.0, 2.0, 2.0);
	glBegin(GL_POLYGON);
	glColor3f(254 / 255.0, 202 / 255.0, 13 / 255.0);
	glVertex2f(0.07, 0.0);
	glVertex2f(0.07, 0.21);
	glVertex2f(0.11, 0.21);
	glVertex2f(0.11, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(254 / 255.0, 202 / 255.0, 13 / 255.0);
	glVertex2f(0.00, 0.06);
	glVertex2f(0.00, 0.1);
	glVertex2f(0.18, 0.1);
	glVertex2f(0.18, 0.06);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(253 / 255.0, 90 / 255.0, 101 / 255.0);
	glVertex2f(0.0, 0.0);
	glVertex2f(0.0, 0.16);
	glVertex2f(0.18, 0.16);
	glVertex2f(0.18, -0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(208 / 255.0, 232 / 255.0, 36 / 255.0);
	glVertex2f(-0.025, 0.21);
	glVertex2f(0.205, 0.21);
	glVertex2f(0.205, 0.16);
	glVertex2f(-0.025, 0.16);
	glEnd();
	glPopMatrix();

	//The gift at left
	glPushMatrix();
	glTranslated(-0.3, -1.7367, -2.2999);
	glScalef(2.0, 1.2, 2.0);
	glBegin(GL_POLYGON);
	glColor3f(255 / 255.0, 147 / 255.0, 54 / 255.0);
	glVertex2f(0.07, 0.0);
	glVertex2f(0.07, 0.21);
	glVertex2f(0.11, 0.21);
	glVertex2f(0.11, 0.0);
	glEnd();


	glBegin(GL_POLYGON);
	glColor3f(49 / 255.0, 160 / 255.0, 151 / 255.0);
	glVertex2f(0.0, 0.0);
	glVertex2f(0.0, 0.16);
	glVertex2f(0.18, 0.16);
	glVertex2f(0.18, -0.0);
	glEnd();


	glBegin(GL_POLYGON);
	glColor3f(34 / 255.0, 167 / 255.0, 164 / 255.0);
	glVertex2f(-0.02, 0.21);
	glVertex2f(0.20, 0.21);
	glVertex2f(0.20, 0.16);
	glVertex2f(-0.02, 0.16);
	glEnd();
	glPopMatrix();
}

void koch(point pt1, point pt2, int n)
{
	point p1, p2, p3, p4, p5;
	glColor3f(0.0, 0.0, 0.0);
	p1.x = pt1.x;
	p1.y = pt1.y;
	p2.x = pt2.x;
	p2.y = pt2.y;
	Stack.push(pt1);
	if (n > 1)
	{
		p3.x = p1.x + (-p1.x + p2.x) / 3;
		p3.y = p1.y + (-p1.y + p2.y) / 3;
		p4.x = p1.x + 2 * (-p1.x + p2.x) / 3;
		p4.y = p1.y + 2 * (-p1.y + p2.y) / 3;
		p5.x = ((p1.x + 2 * (-p1.x + p2.x) / 3) - (p1.x + (-p1.x + p2.x) / 3)) / 2 - ((p1.y + 2 * (-p1.y + p2.y) / 3) - (p1.y + (-p1.y + p2.y) / 3)) * sqrt(3) / 2 + (p1.x + (-p1.x + p2.x) / 3);
		p5.y = ((p1.y + 2 * (-p1.y + p2.y) / 3) - (p1.y + (-p1.y + p2.y) / 3)) / 2 + ((p1.x + 2 * (-p1.x + p2.x) / 3) - (p1.x + (-p1.x + p2.x) / 3)) * sqrt(3) / 2 + (p1.y + (-p1.y + p2.y) / 3);
		koch(p1, p3, n - 1);
		koch(p3, p5, n - 1);
		koch(p5, p4, n - 1);
		koch(p4, p2, n - 1);
	}
	Stack.push(pt2);
}

void drawkoch()
{
	point p1, p2, p3;
	p1.x = -0.5;
	p1.y = 0.0;
	p2.x = 0.0;
	p2.y = 0.5 * sqrt(3);
	p3.x = 0.5;
	p3.y = 0.0;

	koch(p1, p2, 4);
	koch(p2, p3, 4);
	koch(p3, p1, 4);

	glPushMatrix();
	glTranslatef(0.0, 0.0, -2.299);
	glScalef(0.5, 0.5, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	glPointSize(2);
	// change the shape of snowflake
	if (flag1 > 1)
	{
		glBegin(GL_LINE_LOOP);
	}
	else { glBegin(GL_POINTS); }

	for (int i = 0; i <= Stack.size(); i++)
	{
		point pt = Stack.top();
		Stack.pop();
		// fade RGB mode
		if (flag1 >= 1)
		{
			glColor3f(rand() % 255 / 255.0, rand() % 255 / 255.0, rand() % 255 / 255.0);
		}

		glVertex2f(pt.x, pt.y);
	}
	glEnd();
	glColor3f(255 / 255.0, 255 / 255.0, 224 / 255.0);
	draw_solid_circle(0.0, 0.3, 0.8);
	glPopMatrix();
}

void drawcloud()
{	// Each cloud consists 6 solid circular
	glPushMatrix();
	glTranslatef(-0.05, 1.2, -2.29);
	glColor3f(240 / 255.0, 240 / 255.0, 240 / 255.0);

	draw_solid_circle(1.275 - 3.2, 0.25, 0.2);
	draw_solid_circle(1.075 - 3.2, 0.25, 0.1);
	draw_solid_circle(1.0 - 3.2, 0.1, 0.125);
	draw_solid_circle(1.15 - 3.2, 0.1, 0.125);
	draw_solid_circle(1.3 - 3.2, 0.1, 0.125);
	draw_solid_circle(1.45 - 3.2, 0.1, 0.125);

	draw_solid_circle(1.275 - 2.2, 0.25, 0.2);
	draw_solid_circle(1.075 - 2.2, 0.25, 0.1);
	draw_solid_circle(1.0 - 2.2, 0.1, 0.125);
	draw_solid_circle(1.15 - 2.2, 0.1, 0.125);
	draw_solid_circle(1.3 - 2.2, 0.1, 0.125);
	draw_solid_circle(1.45 - 2.2, 0.1, 0.125);


	draw_solid_circle(0.275 - 0.2, 0.25, 0.2);
	draw_solid_circle(0.075 - 0.2, 0.25, 0.1);
	draw_solid_circle(0.0 - 0.2, 0.1, 0.125);
	draw_solid_circle(0.15 - 0.2, 0.1, 0.125);
	draw_solid_circle(0.3 - 0.2, 0.1, 0.125);
	draw_solid_circle(0.45 - 0.2, 0.1, 0.125);


	draw_solid_circle(1.275 - 0.2, 0.25, 0.2);
	draw_solid_circle(1.075 - 0.2, 0.25, 0.1);
	draw_solid_circle(1.0 - 0.2, 0.1, 0.125);
	draw_solid_circle(1.15 - 0.2, 0.1, 0.125);
	draw_solid_circle(1.3 - 0.2, 0.1, 0.125);
	draw_solid_circle(1.45 - 0.2, 0.1, 0.125);


	draw_solid_circle(2.075, 0.25, 0.2);
	draw_solid_circle(1.875, 0.25, 0.1);
	draw_solid_circle(1.8, 0.1, 0.125);
	draw_solid_circle(1.95, 0.1, 0.125);
	draw_solid_circle(2.1, 0.1, 0.125);
	draw_solid_circle(2.25, 0.1, 0.125);
	glPopMatrix();
}

void drawsnow()
{
	glPushMatrix();
	glTranslatef(-0.0, -0.5, -2.29);
	glColor3f(255 / 255.0, 250 / 255.0, 250 / 255.0);
	GLfloat dy = ((rand() % 50) / 1000.0);
	GLfloat y = 1.6 - dy;
	for (int i = 0; i < 2000; i++)
	{
		y = y - dy;
		dy = ((rand() % 100) / 1000.0);
		GLfloat x = (rand() % 540 - 270) / 100.0;
		if (y > -1.0)
			draw_solid_circle(x, y, ((25 + rand() % 75) / 4000.0));
	}
	glPopMatrix();
}

void drawTree()
{
	glPushMatrix();
	glTranslated(1.9, -0.2, -2.2);
	glBegin(GL_POLYGON);
	glColor3f(27 / 255.0, 166 / 255.0, 58 / 255.0);
	glVertex2f(-0.05, 0.10);
	glVertex2f(0.0, 0.15);
	glVertex2f(0.05, 0.10);
	glColor3f(70 / 255.0, 113 / 255.0, 48 / 255.0);
	glVertex2f(0.05, -0.85);
	glVertex2f(-0.0, -0.9);
	glVertex2f(-0.05, -0.85);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslated(1.9, -0.2, -2.2);
	glBegin(GL_POLYGON);
	glColor3f(120 / 255.0, 190 / 255.0, 120 / 255.0);
	glVertex2f(-0.2, -0.05);
	glVertex2f(0.0, 0.15);
	glColor3f(18 / 255.0, 139 / 255.0, 58 / 255.0);
	glVertex2f(0.2, -0.05);
	glVertex2f(-0.0, -0.25);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslated(1.9, -0.4, -2.2);
	glScalef(1.5, 1.5, 1.5);
	glBegin(GL_POLYGON);
	glColor3f(120 / 255.0, 190 / 255.0, 120 / 255.0);
	glVertex2f(-0.2, -0.05);
	glColor3f(120 / 255.0, 190 / 255.0, 120 / 255.0);
	glVertex2f(0.0, 0.15);
	glColor3f(18 / 255.0, 139 / 255.0, 58 / 255.0);
	glVertex2f(0.2, -0.05);
	glVertex2f(-0.0, -0.25);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslated(1.9, -0.6, -2.2);
	glScalef(2.0, 2.0, 0.0);
	glBegin(GL_POLYGON);
	glColor3f(100 / 255.0, 180 / 255.0, 110 / 255.0);
	glVertex2f(-0.2, -0.05);
	glColor3f(120 / 255.0, 190 / 255.0, 120 / 255.0);
	glVertex2f(0.0, 0.15);
	glColor3f(18 / 255.0, 139 / 255.0, 58 / 255.0);
	glVertex2f(0.2, -0.05);
	glVertex2f(-0.0, -0.25);
	glEnd();
	glPopMatrix();


	glColor3d(140 / 255.0, 100 / 255.0, 90 / 255.0);
	glPushMatrix();
	glTranslated(1.9, -1.45, -2.2);
	glBegin(GL_POLYGON);
	glVertex2f(-0.10, -0.20);
	glVertex2f(-0.10, 0.30);
	glVertex2f(-0.05, 0.40);
	glVertex2f(0.0, 0.35);
	glVertex2f(0.05, 0.40);
	glVertex2f(0.10, 0.30);
	glVertex2f(0.10, -0.20);
	glVertex2f(0.0, -0.30);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslated(1.9, -0.8, -2.2);
	glScalef(2.5, 2.5, 0.0);
	glBegin(GL_POLYGON);
	glColor3f(110 / 255.0, 179 / 255.0, 110 / 255.0);
	glVertex2f(-0.2, -0.05);
	glColor3f(110 / 255.0, 180 / 255.0, 110 / 255.0);
	glVertex2f(0.0, 0.15);
	glColor3f(10 / 255.0, 130 / 255.0, 50 / 255.0);
	glVertex2f(0.2, -0.05);
	glColor3f(0.0, 100 / 255.0, 0.0);
	glVertex2f(-0.0, -0.25);
	glEnd();
	glPopMatrix();


	glPushMatrix();
	glTranslated(1.9, -0.9, -2.2);
	glScalef(2.5, 2.5, 0.0);
	glBegin(GL_POLYGON);
	glColor3f(60 / 255.0, 155 / 255.0, 80 / 255.0);
	glVertex2f(-0.25, -0.15);
	glColor3f(120 / 255.0, 190 / 255.0, 120 / 255.0);
	glVertex2f(0.0, 0.15);
	glColor3f(0.0, 100 / 255.0, 0.0);
	glVertex2f(0.25, -0.15);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslated(1.55, -1.25, -2.2);
	glBegin(GL_POLYGON);
	glColor3f(34 / 255.0, 139 / 255.0, 34 / 255.0);
	glVertex2f(-0.15, 0.0);
	glVertex2f(0.0, 0.15);
	glColor3f(18 / 255.0, 143 / 255.0, 58 / 255.0);
	glVertex2f(0.15, 0.0);
	glVertex2f(0.0, -0.15);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslated(2.25, -1.25, -2.2);
	glBegin(GL_POLYGON);
	glColor3f(20.0 / 255.0, 120 / 255.0, 30 / 255.0);
	glVertex2f(-0.15, 0.0);
	glVertex2f(0.0, 0.15);
	glVertex2f(0.15, 0.0);
	glVertex2f(0.0, -0.15);
	glEnd();
	glPopMatrix();
}

void drawHouse() {
	glPushMatrix();
	glTranslatef(-2.0, -1.7, -2.29);
	glScalef(0.3, 0.3, 0.0);

	glColor3f(236 / 255.0, 100 / 255.0, 85 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2f(-1.8, 1.75);
	glVertex2f(-1.8, 2.15);
	glVertex2f(0, 3.6);
	glVertex2f(0, 3.2);
	glEnd();


	glColor3f(236 / 255.0, 100 / 255.0, 85 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2f(0, 3.6);
	glVertex2f(0, 3.2);
	glVertex2f(1.8, 1.75);
	glVertex2f(1.8, 2.15);
	glEnd();

	glColor3f(231 / 255.0, 161 / 255.0, 106 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2f(-1.8, -0.1);
	glVertex2f(-1.8, 0.1);
	glVertex2f(1.8, 0.1);
	glVertex2f(1.8, -0.1);
	glEnd();

	glColor3f(85 / 255.0, 62 / 255.0, 29 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2f(-1.1, 0.5);
	glVertex2f(-1.1, 1.0);
	glVertex2f(-1.05, 1.0);
	glVertex2f(-1.05, 0.5);
	glEnd();

	glColor3f(105 / 255.0, 74 / 255.0, 47 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2f(-1.2, 0.1);
	glVertex2f(-1.2, 1.1);
	glVertex2f(-0.6, 1.1);
	glVertex2f(-0.6, 0.1);
	glEnd();

	glColor3f(85 / 255.0, 62 / 255.0, 29 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2f(-0.025, 1.8);
	glVertex2f(-0.025, 2.4);
	glVertex2f(0.025, 2.4);
	glVertex2f(0.025, 1.8);
	glEnd();

	glColor3f(85 / 255.0, 62 / 255.0, 29 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2f(-0.3, 2.075);
	glVertex2f(-0.3, 2.125);
	glVertex2f(0.3, 2.125);
	glVertex2f(0.3, 2.075);
	glEnd();



	glColor3f(173 / 255.0, 203 / 255.0, 236 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2f(-0.3, 1.8);
	glVertex2f(-0.3, 2.4);
	glVertex2f(0.3, 2.4);
	glVertex2f(0.3, 1.8);
	glEnd();

	glColor3f(219 / 255.0, 178 / 255.0, 149 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2f(-0.35, 1.75);
	glVertex2f(-0.35, 2.45);
	glVertex2f(0.35, 2.45);
	glVertex2f(0.35, 1.75);
	glEnd();
	//


	glColor3f(173 / 255.0, 203 / 255.0, 236 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2f(-0.3 - 0.9, 1.8 - 0.5);
	glVertex2f(-0.3 - 0.9, 2.4 - 0.5);
	glVertex2f(0.3 - 0.9, 2.4 - 0.5);
	glVertex2f(0.3 - 0.9, 1.8 - 0.5);
	glEnd();

	glColor3f(219 / 255.0, 178 / 255.0, 149 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2f(-0.35 - 0.9, 1.75 - 0.5);
	glVertex2f(-0.35 - 0.9, 2.45 - 0.5);
	glVertex2f(0.35 - 0.9, 2.45 - 0.5);
	glVertex2f(0.35 - 0.9, 1.75 - 0.5);
	glEnd();
	//
	glColor3f(85 / 255.0, 62 / 255.0, 29 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2f(-0.025 + 0.6, 1.8 - 1.0);
	glVertex2f(-0.025 + 0.6, 2.4 - 1.0);
	glVertex2f(0.025 + 0.6, 2.4 - 1.0);
	glVertex2f(0.025 + 0.6, 1.8 - 1.0);
	glEnd();

	glColor3f(85 / 255.0, 62 / 255.0, 29 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2f(-0.3 + 0.6, 2.075 - 1.0);
	glVertex2f(-0.3 + 0.6, 2.125 - 1.0);
	glVertex2f(0.3 + 0.6, 2.125 - 1.0);
	glVertex2f(0.3 + 0.6, 2.075 - 1.0);
	glEnd();

	glColor3f(173 / 255.0, 203 / 255.0, 236 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2f(-0.3 + 0.6, 1.8 - 1.0);
	glVertex2f(-0.3 + 0.6, 2.4 - 1.0);
	glVertex2f(0.3 + 0.6, 2.4 - 1.0);
	glVertex2f(0.3 + 0.6, 1.8 - 1.0);
	glEnd();

	glColor3f(219 / 255.0, 178 / 255.0, 149 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2f(-0.35 + 0.6, 1.75 - 1.0);
	glVertex2f(-0.35 + 0.6, 2.45 - 1.0);
	glVertex2f(0.35 + 0.6, 2.45 - 1.0);
	glVertex2f(0.35 + 0.6, 1.75 - 1.0);
	glEnd();
	//
	glColor3f(240 / 255.0, 219 / 255.0, 205 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2f(-1.5, 0);
	glVertex2f(-1.5, 2.0);
	glVertex2f(0, 3.2);
	glVertex2f(1.5, 2.0);
	glVertex2f(1.5, 0);
	glEnd();
	glPopMatrix();

}

void drawSantasFace() {
	glPushMatrix();
	glTranslatef(-2.0, -1.75, -2.29);
	glRotatef(rand() % 10 - 5, 0.0, 0.0, 1.7);
	glScalef(0.3, 0.3, 0.0);
	glColor3f(190 / 255.0, 29 / 255.0, 45 / 255.0);
	glBegin(GL_POLYGON);
	glVertex2f(-1.2, 1.8);
	glVertex2f(-0.95, 2.3);
	glVertex2f(-0.7, 1.8);
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	draw_solid_circle(-1.05, 1.75, 0.05);
	draw_solid_circle(-0.85, 1.75, 0.05);

	glColor3f(190 / 255.0, 29 / 255.0, 45 / 255.0);
	draw_solid_circle(-0.95, 1.575, 0.025);
	draw_solid_circle(-0.925, 1.575, 0.025);
	draw_solid_circle(-0.975, 1.575, 0.025);

	glColor3f(219 / 255.0, 178 / 255.0, 149 / 255.0);
	draw_solid_circle(-0.95, 1.55, 0.1);

	glColor3f(1.0, 1.0, 1.0);
	draw_solid_circle(-0.95, 1.5, 0.2);

	glColor3f(219 / 255.0, 178 / 255.0, 149 / 255.0);
	draw_solid_circle(-0.95, 1.7, 0.25);
	glPopMatrix();
}

void drawWord()
{	//Fade RGB mode
	//Randomly generate two colors
	glPushMatrix();
	glTranslatef(-0.0, 0.025, -2.29);
	glScalef(0.5, 0.5, 0.0);
	R = 240;
	G = 40 + rand() % 88;
	B = 40 + rand() % 88;
	R2 = R + rand() % 15;
	G2 = G + rand() % 127;
	B2 = B + rand() % 127;

	//Caculate the differencial
	float deltaR, deltaG, deltaB;
	deltaR = (R2 - R) / 4;
	deltaG = (G2 - G) / 4;
	deltaB = (B2 - B) / 4;

	if (flag1 >= 1)
	{
		glColor3f((R + 1.5*deltaR) / 255.0, (G + 1.5*deltaG) / 255.0, (B + 1.5*deltaB) / 255.0);
	}
	selectFont(48, ANSI_CHARSET, 0, "Blackadder ITC");
	glRasterPos2f(-0.9, 0.9);
	drawString("M");
	if (flag1 > 1)
	{
		glColor3f((R + deltaR) / 255.0, (G + deltaG) / 255.0, (B + deltaB) / 255.0);
	}
	selectFont(48 , ANSI_CHARSET, 0, "Blackadder ITC");
	glRasterPos2f(-0.4, 1.1);
	drawString("E");
	if (flag1 >= 1)
	{
		glColor3f((R + 0.5*deltaR) / 255.0, (G + 0.5*deltaG) / 255.0, (B + 0.5*deltaB) / 255.0);
	}
	selectFont(48 , ANSI_CHARSET, 0, "Blackadder ITC");
	glRasterPos2f(-0.1, 1.15);
	drawString("R");
	if (flag1 >= 1)
	{
		glColor3f((R + deltaR) / 255.0, (G + deltaG) / 255.0, (B + deltaB) / 255.0);
	}
	selectFont(48 , ANSI_CHARSET, 0, "Blackadder ITC");
	glRasterPos2f(0.3, 1.1);
	drawString("R");
	if (flag1 >= 1)
	{
		glColor3f((R + 1.5 * deltaR) / 255.0, (G + 1.5 * deltaG) / 255.0, (B + 1.5 * deltaB) / 255.0);
	}
	selectFont(48 , ANSI_CHARSET, 0, "Blackadder ITC");
	glRasterPos2f(0.65, 0.9);
	drawString("Y");
	glColor3f(255.0 / 255.0, 228 / 255.0, 225 / 255.0);

	selectFont(72 , ANSI_CHARSET, 0, "Script MT Bold");
	glRasterPos2f(-1.1, -0.9);
	drawString("Christmas");
	glPopMatrix();
	glutPostRedisplay();
}

void display(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	background();
	drawTree();
	drawgift();
	drawSantasFace();
	drawcloud();
	drawkoch();
	drawWord();
	drawHouse();

}

void KeyboardFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
	case 'W': flag = 2;
		glutPostRedisplay();
		break;
	case 'g':
	case 'G':
		if (flag == 1) {
			break;
		}
		if (flag == 2) {
			flag = 3;
			glutPostRedisplay();
			break;
		}		
		if (flag == 3) {
			flag = 2;
			glutPostRedisplay();
			break;
		}
		
		// 'esc' button to exit the program
	case 27: {
		exit(0);
	}
	}
}

void Idle(void)
{
	if (day < 10)
	{
		++day;
	}
	else {
		day = 9999;
	}
	srand(time(NULL));
	glutPostRedisplay();
}

void scene()
{
	if (flag == 1)
	{
		welcome_screen();
	}

	if (flag == 2)
	{
		glutIdleFunc(Idle);
		glRotatef(10 / exp2f(day), 0.0f, 0.0f, -1.0f);
		display();
		glFlush();
		glLoadIdentity();
		glutPostRedisplay();
	}
	if (flag == 3)
	{
		glutIdleFunc(Idle);
		display();
		drawsnow();
		glFlush();
		glLoadIdentity();
		glutPostRedisplay();
	}

	glFlush();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(1280, 960);
	glutInitWindowPosition(50, 40);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
	glutCreateWindow("Christmas Card");


	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glutReshapeFunc(reshape);
	glutDisplayFunc(scene);
	glDepthFunc(GL_NOTEQUAL);

	glutKeyboardFunc(KeyboardFunc);
	glutMouseFunc(mouse_input);
	glutMainLoop();
	return(0);
}