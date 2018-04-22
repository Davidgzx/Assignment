/*
1508672
Guo.Zixuan
*/
#include<math.h>
#include<vector>
#include<stdio.h>
#include <GL/glut.h>
#include<time.h>
#include <windows.h>

using namespace std;
int flag =0;
int flag1 = 1;
int w = 1600;
int h = 900;
double cameraAngle;
double cameraSpeed;
double cameraHeight;
double cameraRadius;
float speed=180;
GLuint  up;
GLuint  down;
GLuint  right;
GLuint  left;
GLuint  foward;
GLuint  backward;
GLuint  board;
struct point3 {

	GLfloat  x;
	GLfloat  y;
	GLfloat  z;
};

point3 mid(point3 a, point3 b) {
	point3 v;
	v.x = (a.x + b.x) / 2.0;
	v.y = (a.y + b.y) / 2.0;
	v.z = (a.z + b.z) / 2.0;
	return v;
}

class Terrain {
public:
	vector< vector<float> > points;
	int cols, rows;
	float y;
	Terrain(int rows, int cols) {
		this->rows = rows;
		this->cols = cols;
		srand((unsigned)time(NULL));
		for (int i = 0;i < rows; i++) {
			points.push_back(vector<float>()); 
			for (int j = 0; j < cols; j++) {
				if ((j > 0 &&j<100 && i > 45 && i<55)) {
					y = float(0.7*sin((j+i)/4)*cos((i + j)/4))- float(rand()% 15 / 10.0);
					points[i].push_back(y);
				}else{float y = float(rand()%5/10.0+0.5); points[i].push_back(y);
				}

			}
		}
	}
};
Terrain *t = new Terrain(100, 100);
void drawString(const char* str)
{
	static GLuint lists;
	lists = glGenLists(128);

	wglUseFontBitmaps(wglGetCurrentDC(), 0, 128, lists);

	for (; *str != '\0'; ++str)
	{
		glCallList(lists + *str);
	}

	glDeleteLists(lists, 128);
}

void selectFont(int size, int charset, int Escapement, const char* face)
{
	HFONT	hFont = CreateFontA(size, 0, Escapement, 0, FW_BLACK, 0, 0, 0, charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT	hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}


void keyboardListener(unsigned char key,int x,int y) {
	switch (key) {

	case '1':	//reverse the rotation of camera
		cameraSpeed = -cameraSpeed;
		break;

	case '2':	//increase rotation speed of camera by 10%
		cameraSpeed *= 1.1;
		break;

	case '3':	//decrease rotation
		cameraSpeed /= 1.1;
		break;
	case '4':	//stop rotation
		if (cameraSpeed > 0.000001)
			cameraSpeed = 0.000000;
		else
			cameraSpeed = 0.004;
		break;
	case 'g':
		if (flag<=2)
		flag += 1;
		break;
	case 'b':
		if (flag>=1)
		flag -= 1;
		break;
	case 'q':
		if (flag1 == 1)
			flag1 = 2;
		else if (flag1 == 2)
			flag1 = 1;
		break;
	case 's':
		if (cameraHeight < 1500)
			cameraHeight += 10;
		if (cameraRadius<3000)
			cameraRadius += 10;
		break;
	case 'a':
		cameraAngle -= 0.05;
		break;
	case 'w':
		if (cameraHeight>-1500)
			cameraHeight -= 10;
		if (cameraRadius > 10)
			cameraRadius -= 10;
		break;
	case 'd':
		cameraAngle += 0.05;
		break;

	case 27:	
		exit(0);
		break;

	default:
		break;
	}
}

void reshape(int w, int h)
{
	GLdouble	size;
	GLdouble	aspect;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat ar = (GLfloat)w / h;
	if (ar>1)
		gluPerspective(70, ar, 0.1, 10000.0); 
	else
		gluPerspective(70, 1.0/ar, 0.1, 10000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
}

void specialKeyListener(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_DOWN:
		if (cameraRadius<3000)	//down arrow key
		cameraRadius += 10;
		break;
	case GLUT_KEY_UP:		// up arrow key
		if (cameraRadius > 10)
			cameraRadius -= 10;
		break;

	case GLUT_KEY_RIGHT:
		cameraAngle += 0.05;
		break;
	case GLUT_KEY_LEFT:
		cameraAngle -= 0.05;
		break;

	case GLUT_KEY_PAGE_UP:
		if (cameraHeight<1500)
			cameraHeight += 10;
		break;
	case GLUT_KEY_PAGE_DOWN:
		if (cameraHeight>-1500)
		cameraHeight -= 10;
		break;

	default:
		break;
	}
}

void mouseListener(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {		
			if (flag <= 3)
				flag += 1;
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			if (flag >= 1)
				flag -= 1;
		}
		break;
	default:
		break;
	}
}
//The load_texture function are rewritten from the codes in http://blog.csdn.net/dcrmg/article/details/53180369
GLuint load_texture(const char* file_name)
{
	GLint width, height, total_bytes;
	GLubyte* pixels = 0;
	GLuint last_texture_ID = 0, texture_ID = 0;
	FILE* pFile = fopen(file_name, "rb");
	if (pFile == 0)
		return 0;
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, 4, 1, pFile);
	fread(&height, 4, 1, pFile);
	fseek(pFile, 54, SEEK_SET);
	GLint line_bytes = width * 3;
	while (line_bytes % 4 != 0)+line_bytes;
	total_bytes = line_bytes * height;
	pixels = (GLubyte*)malloc(total_bytes);
	if (pixels == 0)
	{
		fclose(pFile);
		return 0;
	}
	if (fread(pixels, total_bytes, 1, pFile) <= 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}
	glGenTextures(1, &texture_ID);
	if (texture_ID == 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}
	GLint lastTextureID = last_texture_ID;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, lastTextureID); 
	free(pixels);
	return texture_ID;
}

void welcome_screen()
{
	glClearColor(0.5, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
	GLfloat posY = -5.0f;
	GLfloat posX = -5.0f;
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0);
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
	glColor3f(1, 1, 1);
	selectFont(72, ANSI_CHARSET, 0, "Comic Sans MS");
	glRasterPos2f(-2.0, 1.0);
	drawString("Three-dimention opengl program");
	selectFont(28, ANSI_CHARSET, 0, "Comic Sans MS");
	glRasterPos2f(-1.05, 0.8);
	drawString("--Features: terrain generation, Skybox, Sierpinski gasket ");
	selectFont(48, ANSI_CHARSET, 0, "Comic Sans MS");
	glRasterPos2f(-1.15, 0.4);
	drawString("ZiXuan.Guo15");
	glRasterPos2f(-1.15, 0.05);
	drawString("1508672");
	glColor3f(0.15, 0.28, 0.76);
	selectFont(48, ANSI_CHARSET, 0, "Calibri");
	glRasterPos2f(-1.15, -0.35);
	drawString("Press'g' or mouse button to continue");
	selectFont(24, ANSI_CHARSET, 0, "Calibri");
	glRasterPos2f(-0.95, -0.55);
	drawString("Press'w''a''s''b''Page_up''Page_Down' and the direction keys to control camera");
	glRasterPos2f(-0.95, -0.75);
	drawString("Press'1''2''3''4' to control rotation");
	glRasterPos2f(-0.95, -0.95);
	drawString("Press'q' to enable lighting");
}

void init() {
	cameraAngle = 0;	
	cameraSpeed = 0.004;
	cameraHeight = 500;
	cameraRadius = 1000;
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);//enable Depth Testing
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_2D);
	foward = load_texture("skyrender0001.bmp");
	backward = load_texture("skyrender0004.bmp");
	left = load_texture("skyrender0005.bmp");
	right = load_texture("skyrender0002.bmp");
	up = load_texture("skyrender0003.bmp");
	down = load_texture("skyrender0006.bmp");
	board = load_texture("board.bmp");
	glutReshapeFunc(reshape);

}

void colorchoose(GLfloat x, GLfloat y, GLfloat height) {
	GLfloat white[] = { 1.0,1.0,1.0 };
	if (height < -0.5) {
		glColor3f(0 / 255.0, 229.0 / 255.0, 238.0 / 255.0);
		if (flag1 > 1) {
			float color[] = { 0 / 255.0, 229.0 / 255.0, 238.0 / 255.0 };
			glMaterialfv(GL_FRONT, GL_SPECULAR, white);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);

		}
		
	}
		

	if (height <-0.2&&height >-0.3) {
		glColor3f(0 / 255.0, 225 / 255.0, 232 / 255.0);
		if (flag1 >1) {
			float color[] = { 0 / 255.0, 225 / 255.0, 232 / 255.0 };
			glMaterialfv(GL_FRONT, GL_SPECULAR, white);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);

		}

	}


	if (height <0.3&&height >-0.3) {
		glColor3f(0 / 255.0, 205 / 255.0, 225 / 255.0);
		if (flag1 > 1) {
			float color[] = { 0 / 255.0, 205 / 255.0, 225 / 255.0};
			glMaterialfv(GL_FRONT, GL_SPECULAR, white);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);

		}
	}

	if (height < 0.6&&height >0.3) {
		glColor3f(0.65, 0.7, 0.1);
		if (flag1 > 1) {
			float color[] = { 0.65, 0.7, 0.1 };
			glMaterialfv(GL_FRONT, GL_SPECULAR, white);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);

		}
	}
	if (height > 0.5&&height < 0.65) {
		glColor3f(0.4, 0.7, 0.1);
		if (flag1 > 1) {
			float color[] = { 0.4, 0.7, 0.1 };
			glMaterialfv(GL_FRONT, GL_SPECULAR, white);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);

		}

	}

	if (height > 0.65&&height < 0.7) {
		glColor3f(0.5, 0.8, 0.5);
		if (flag1 > 1) {
			float color[] = { 0.5, 0.8, 0.5 };
			glMaterialfv(GL_FRONT, GL_SPECULAR, white);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);

		}

	}
	if (height > 0.85) {
		glColor3f(0.7, 0.85, 0.7);
		if (flag1 > 1) {
			float color[] = { 0.7, 0.85, 0.7 };
			glMaterialfv(GL_FRONT, GL_SPECULAR, white);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);

		}

	}

	if (height > 0.885) {
		glColor3f(0.85, 0.875, 0.8);
		if (flag1 > 1) {
			float color[] = { 0.85, 0.875, 0.8 };
			glMaterialfv(GL_FRONT, GL_SPECULAR, white);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);

		}
	}

	}

void ground() {
	if (flag1 > 1)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
	glPushMatrix();
	glScalef(25, 25, 25);
	glShadeModel(GL_SMOOTH);
	glBegin(GL_QUADS);
	int r, c;
	r = -(t->rows / 2);
	for (int i = 0; i < t->rows - 1; i++) {
		c = -(t->cols / 2);
		for (int j = 0; j < t->cols - 1; j++) {
			colorchoose(r, c, (t->points[i][j]));
			glVertex3f(r, c, t->points[i][j]);

			colorchoose(0.8, 0.8, t->points[i + 1][j]);
			glVertex3f(r + 1, c, t->points[i + 1][j]);

			glVertex3f(r + 1, c + 1, t->points[i + 1][j + 1]);

			colorchoose(0.2, 0.2, t->points[i][j + 1]);
			glVertex3f(r, c + 1, t->points[i][j + 1]);
			c++;
		}
		r++;
	}
	glEnd();
	glPopMatrix();
}
// The DrawChess function is queted from http://www.cnblogs.com/acloud/archive/2011/12/12/DrawChess.html
void DrawChess(GLfloat a, GLfloat b, GLfloat c, GLint nSlice, GLint nStack)
{	
	const GLfloat PI = (GLfloat)(3.141592653589);               
	GLfloat fYRotStep = 2.0f * PI / nStack;                        
	GLfloat fRange = atan(a / (b + c));                             
	GLfloat R = sqrt(a * a + (b + c) * (b + c)) + c;            

	GLint nSlice1 = nSlice;                                          
	GLint nSlice2 = (GLint)(nSlice * (PI - fRange) * c / a);     
	GLint nSlice3 = (GLint)(nSlice * R * fRange / a);            

	GLfloat fStep1 = a / nSlice1;                                
	GLfloat fStep2 = (PI - fRange) / nSlice2;                    
	GLfloat fStep3 = fRange / nSlice3;                            

	GLfloat dr = -0.5f / (nSlice1 + nSlice2 + nSlice3);           

	GLint i = 0, j = 0;
	for (i = 0; i<nStack; i++)
	{
		GLfloat fYR = i * fYRotStep;                           
		GLfloat fZ = -sin(fYR);                                   
		GLfloat fX = cos(fYR);                                    
		GLfloat fZ1 = -sin(fYR + fYRotStep);                    
		GLfloat fX1 = cos(fYR + fYRotStep);                        
		GLfloat rs = 0.5f;                                       
		glBegin(GL_TRIANGLE_STRIP);

		
		for (j = 0; j<nSlice1; j++)
		{
			GLfloat r = fStep1 * j;

			glTexCoord2f(0.5f + rs * fX, 0.5f + rs * fZ);
			glNormal3f(0.0f, -1.0f, 0.0f);
			glVertex3f(r * fX, b, r * fZ);

			glTexCoord2f(0.5f + rs * fX1, 0.5f + rs * fZ1);
			glNormal3f(0.0f, -1.0f, 0.0f);
			glVertex3f(r * fX1, b, r * fZ1);

			rs += dr;
		}

	
		for (j = 0; j<nSlice2; j++)
		{
			GLfloat r = a + c * sin(fStep2 * j);
			GLfloat y = b + c - c * cos(fStep2 * j);
			GLfloat nr = sin(fStep2 * j);
			GLfloat nY = -cos(fStep2 * j);

			glTexCoord2f(0.5f + rs * fX, 0.5f + rs * fZ);
			glNormal3f(nr * fX, nY, nr * fZ);
			glVertex3f(r * fX, y, r * fZ);

			glTexCoord2f(0.5f + rs * fX1, 0.5f + rs * fZ1);
			glNormal3f(nr * fX1, nY, nr * fZ1);
			glVertex3f(r * fX1, y, r * fZ1);

			rs += dr;
		}


		for (j = 0; j <= nSlice3; j++)
		{
			GLfloat r = R * sin(fRange - j * fStep3);
			GLfloat y = R * cos(fRange - j * fStep3);
			GLfloat nr = sin(fRange - j * fStep3);
			GLfloat nY = cos(fRange - j * fStep3);

			glTexCoord2f(0.5f + rs * fX, 0.5f + rs * fZ);
			glNormal3f(nr * fX, nY, nr * fZ);
			glVertex3f(r * fX, y, r * fZ);

			glTexCoord2f(0.5f + rs * fX1, 0.5f + rs * fZ1);
			glNormal3f(nr * fX1, nY, nr * fZ1);
			glVertex3f(r * fX1, y, r * fZ1);

			rs += dr;
		}

		glEnd();
	}
}

void drawchessboard() {
	glPushMatrix();
	glTranslatef(0.0, -40.0, 15);
	glRotatef(90, 1.0, 0, 0);

	glPushMatrix();
	
	glScalef(100, 1, 100);
	glColor3f(238 / 255.0, 220 / 255.0, 130 / 255.0);
	// to alleviate the shaking of texture
	glTranslatef(-0.0, 125-0.0026*cameraHeight, -0.35);
	glutSolidCube(5);
	glPopMatrix();


	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, board);
	glScalef(250, 50, 250);
	glTranslatef(-0.0,1.57,-0.14);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	//Begin to draw chesses
	//Firstly, set up the light parameters
	GLfloat light_position[] = { cos(cameraAngle) + 1, sin(cameraAngle)+1,cos(cameraAngle) + 1, 0.0 };
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 0.5, 0.5, 0.5, 0.5 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//Then, set up the Material parameters
	float white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float black[] = { 0.01f, 0.01f, 0.01f, 0.01f };
	GLfloat mat_diffuse[] = { 0.015, 0.015, 0.015, 1.00 };
	GLfloat mat_shininess[] = { 26.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, black);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glPushMatrix();

	glTranslatef(-0.5, 122, 14);
	DrawChess(9, 6, 2, 16,32);

	glPopMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, white);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
	glPushMatrix();

	glTranslatef(25.5, 122, 40);
	DrawChess(9, 6, 2, 16, 32);
	glPopMatrix();
	glDisable(GL_LIGHTING);
	glPopMatrix();
}

void idle() {

	cameraAngle += cameraSpeed;	// camera will rotate at 0.004 radians per frame
	glutPostRedisplay();
	if (flag >1)
	speed +=10.0;
	
}
// The following two functions implement the  Sierpinski gasket
// The source codes is rewritten from the codes in the text book chapter two
void drawTriangles(point3 a, point3 b, point3 c, point3 d) {

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_SMOOTH);
	glPushMatrix();
	glRotatef(speed, 0, 0, -0.5);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(a.x,a.y,a.z);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(b.x, b.y, b.z);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(c.x, c.y, c.z);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(a.x, a.y, a.z);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(b.x, b.y, b.z);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(d.x, d.y, d.z);
	glEnd();
	glPopMatrix();
	glPushMatrix();

	glRotatef(speed, 0, 0.0, 0.5);
	glBegin(GL_TRIANGLES);
	glColor3f(0.9f, 0.8f, 0.5f);
	glVertex3f(a.x, a.y, a.z);
	glColor3f(0.9f, 0.8f, 0.5f);
	glVertex3f(c.x, c.y, c.z);
	glColor3f(0.9f, 0.8f, 0.5f);
	glVertex3f(d.x, d.y, d.z);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(b.x, b.y, b.z);
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(c.x, c.y, c.z);
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(d.x, d.y, d.z);
	glEnd();

	glPopMatrix();
}

void divide_Pyramid(point3 a, point3 b, point3 c, point3 d, int iteraciones) {
	point3 v[6];
	int j;
	if (iteraciones > 0) {

		v[0] = mid(a, b);
		v[1] = mid(a, c);
		v[2] = mid(b, c);
		v[3] = mid(a, d);
		v[4] = mid(b, d);
		v[5] = mid(c, d);
		
		divide_Pyramid(a, v[0], v[1], v[3], iteraciones - 1);
		divide_Pyramid(v[0], b, v[2], v[4], iteraciones - 1);
		divide_Pyramid(v[1], v[2], c, v[5], iteraciones - 1);
		divide_Pyramid(v[3], v[4], v[5], d, iteraciones - 1);
		// commenting this will create a Sierpinski Triangle
		//divideTriangle(v[0], v[1], v[2], iteraciones-1);

	}
	else {
		//draw Triangles at end of recursion
		drawTriangles(a, b, c, d);
	}
}

void pillars() {
	point3 pyramid1;
	point3 pyramid2;
	point3 pyramid3;
	point3 pyramid4;
	pyramid1.x = 0.0;
	pyramid1.y = 0.0;
	pyramid1.z = 0.0;
	pyramid2.x = 0.0;
	pyramid2.y = 2.0;
	pyramid2.z = 0.0;
	pyramid3.x = 1.7302;
	pyramid3.y = 1.0;
	pyramid3.z = 0.0;
	pyramid4.x = 0.866;
	pyramid4.y = 1.0;
	pyramid4.z = 0.866;
	glPushMatrix();
	glScalef(40.0, 40.0, 150.0);
	glTranslatef(5.0, -5.0, 0.085);
	divide_Pyramid(pyramid1, pyramid2, pyramid3, pyramid4, 3);
	glPopMatrix();
	glPushMatrix();
	glScalef(40.0, 40.0, 150.0);
	glTranslatef(-5.0, 5.0, 0.085);
	divide_Pyramid(pyramid1, pyramid2, pyramid3, pyramid4, 3);
	glPopMatrix();
	glPushMatrix();
	glScalef(40.0, 40.0, 150.0);
	glTranslatef(5.0, 5.0, 0.085);
	divide_Pyramid(pyramid1, pyramid2, pyramid3, pyramid4, 3);
	glPopMatrix();
	glPushMatrix();
	glScalef(40.0, 40.0, 150.0);
	glTranslatef(-5.0, -5.0, 0.085);
	divide_Pyramid(pyramid1, pyramid2, pyramid3, pyramid4, 3);
	glPopMatrix();
}
// The bmp picture files are from https://www.turbosquid.com/FullPreview/Index.cfm/ID/348109
void skybox() {
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();

	glTranslatef(0, 0, cameraHeight);
	glScalef(4200, 4200,4200);
	// up
	glBindTexture(GL_TEXTURE_2D, up);
	glBegin(GL_QUADS); 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glEnd();
	//down
	glBindTexture(GL_TEXTURE_2D, down);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();
	//right
	glBindTexture(GL_TEXTURE_2D, right);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glEnd();
	// left
	glBindTexture(GL_TEXTURE_2D, left);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();
	// foward
	glBindTexture(GL_TEXTURE_2D, foward);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glEnd();
	// backward
	glBindTexture(GL_TEXTURE_2D, backward);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();

	glPopMatrix(); 

	glDisable(GL_TEXTURE_2D);
}

void display() {
	if (flag == 0)
	{

		glLoadIdentity();
		glOrtho(-5, 5, -5, 5, -5, 5);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		welcome_screen();
	}
	if (flag >= 1)

	{	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT| GL_ACCUM_BUFFER_BIT| GL_STENCIL_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClearColor(0, 0, 0, 0);
		gluLookAt(cameraRadius*cos(cameraAngle), cameraRadius*sin(cameraAngle), cameraHeight, 0, 0, 0, 0, 0, 1);	
		if (flag > 2) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			ground();
		}
		else {
			ground();
		pillars();
		drawchessboard();
		skybox();
		
		}	
	}

	glutSwapBuffers();

}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(w, h);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("My OpenGL Program");

	init();
	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(idle);		
	//ADD listeners:
	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		
	return 0;
}