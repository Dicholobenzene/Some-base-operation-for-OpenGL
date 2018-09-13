#include <GL/glut.h>
#include <math.h>
#include "3d_loader.h"
#include "HalfEdge.h"
int WindowSizeX;
int WindowSizeY;
double tempx, tempy,theX,theY,newx,newy;
GLdouble TriPoint[3][2] = {0,0,-0.5,0,-0.5,-0.5};
GLdouble QuaPoint[4][2] = {0,0,1,0,1,1,0,1};
GLdouble PenPoint[7][2] = {-0.5,0.7,-0.7,0.6,-0.6,0.5,-0.4,0.5,-0.3,0.6};
GLdouble color[3][3] = { 1,0,0,0,1,0,0,0,1 };
int check,boarder,colorc=1;

void Show() {
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POLYGON);
	glColor3f(color[0][0], color[0][1], color[0][2]);
	for (int i = 0; i < 3; i++) {
		glVertex2d(TriPoint[i][0], TriPoint[i][1]);
	}
	glEnd();
	glFlush();
	glBegin(GL_POLYGON);
	glColor3f(color[1][0], color[1][1], color[1][2]);
	for (int i = 0; i < 4; ++i) {
		glVertex2d(QuaPoint[i][0], QuaPoint[i][1]);
	}
	glEnd();
	glFlush();
	glBegin(GL_POLYGON);
	glColor3f(color[2][0], color[2][1], color[2][2]);
	for (int i = 0; i < 5; i++) {
		glVertex2d(PenPoint[i][0], PenPoint[i][1]);
	}
	glEnd();
	glFlush();
}

void sofang(double propotion) {
	switch (check)
	{
	case 1:
		for (int i = 0; i < 3; i++) {
			TriPoint[i][0] *= propotion;
			TriPoint[i][1] *= propotion;
		}
		break;
	case 2:
		for (int i = 0; i < 4; i++) {
			QuaPoint[i][0] *= propotion;
			QuaPoint[i][1] *= propotion;
		}
		break;
	case 3:
		for (int i = 0; i < 5; i++) {
			PenPoint[i][0] *= propotion;
			PenPoint[i][1] *= propotion;
		}
	default:
		break;
	}
}

int checkinside(double x, double y) {
	if (x > QuaPoint[0][0] && x<QuaPoint[1][0] && y>QuaPoint[1][1] && y < QuaPoint[2][1])
		return 2;
	if (y > TriPoint[2][1] && y<TriPoint[0][1] && x>TriPoint[2][0] && x < TriPoint[0][0] && x - TriPoint[1][0] - y + TriPoint[1][1] < 0.5)
		return 1;
	if (y > PenPoint[3][1] && y<PenPoint[1][1] && x>PenPoint[2][0] && x < PenPoint[3][0])
		return 3;
	return 0;
};
void mouse_process(int button, int state, int x, int y) {
	colorc = 1;
	check = 0;
	y = WindowSizeY - y;
	tempx = (x - theX) / theX;
	tempy = (y - theY) / theY;
	check = checkinside(tempx, tempy);
	if (check == 0)return;
}
void mouse_process_active(int x, int y) {
	colorc = 1;
	boarder = 0;
	y = WindowSizeY - y;
	newx = (x - theX) / theX;
	newy = (y - theY) / theY;
	if (checkinside(newx , newy) == check) {
		return;
	}
	else 
		switch (check) {
			case 0:
				break;
			case 1:
				newx = newx - tempx;
				newy = newy - tempy;
				for (int i = 0; i < 3; i++) {
					TriPoint[i][0] += newx;
					TriPoint[i][1] += newy;
					if (TriPoint[i][0] < -1 || TriPoint[i][0]>1 || TriPoint[i][1] < -1 || TriPoint[i][1]>1)
						boarder = 1;
				}
				if(boarder)
					for (int i = 0; i < 3; i++) {
						TriPoint[i][0] -= newx;
						TriPoint[i][1] -= newy;
					}
				break;
			case 2:
				newx = newx - tempx;
				newy = newy - tempy;
				for (int i = 0; i < 4; i++) {
					QuaPoint[i][0] += newx;
					QuaPoint[i][1] += newy; 
					if (QuaPoint[i][0] < -1 || QuaPoint[i][0]>1 || QuaPoint[i][1] < -1 || QuaPoint[i][1]>1)
						boarder = 1;
				}
				if (boarder)
					for (int i = 0; i < 4; i++) {
						QuaPoint[i][0] -= newx;
						QuaPoint[i][1] -= newy;
					}
				break;
			case 3:
				newx = newx - tempx;
				newy = newy - tempy;
				for (int i = 0; i < 5; i++) {
					PenPoint[i][0] += newx;
					PenPoint[i][1] += newy;
					if (PenPoint[i][0] < -1 || PenPoint[i][0]>1 || PenPoint[i][1] < -1 || PenPoint[i][1]>1)
						boarder = 1;
				}
				if (boarder)
					for (int i = 0; i < 5; i++) {
						PenPoint[i][0] -= newx;
						PenPoint[i][1] -= newy;
					}
				break;
			}
	Show();
	colorc = 1;
}

void keyboard_function(unsigned char a,int x, int y) {
	switch (a)
	{
	case 27:
		exit(0);
	case 'i':
		sofang(0.5);
		break;
	case 'o':
		sofang(1.5);
		break;
	default:
		double rand1 = rand() % 1000;
		double rand2 = rand() % 1000;
		double rand3 = rand() % 1000;
		if (check != 1 && check != 2 && check != 0)
			check = 3;
		color[check - 1][0] = rand1 / 1000;
		color[check - 1][1] = rand2 / 1000;
		color[check - 1][2] = rand3 / 1000;
		break;
	}
	Show();
}

int main(int argc,char *argv[]) {
	/*{
		WindowSizeX = 500;
		WindowSizeY = 500;
		theX = WindowSizeX / 2;
		theY = WindowSizeY / 2;
		glutInit(&argc, argv);
		gluOrtho2D(0, WindowSizeX, 0, WindowSizeY);
		glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
		glutInitWindowSize(WindowSizeX, WindowSizeY);
		glutCreateWindow("test");
		glClearColor(1, 1, 1, 0);
		glMatrixMode(GL_PROJECTION);

		glutDisplayFunc(Show);
		glutMouseFunc(mouse_process);
		glutMotionFunc(mouse_process_active);
		glutKeyboardFunc(keyboard_function);

		glutMainLoop();
		return 0;
	}//test1_display_3_2d_and_opeation
	*/

	{
		readfile("C:\\Users\\aria\\Documents\\o.off");
		WindowSizeX = 500;
		WindowSizeY = 500;
		theX = WindowSizeX / 2;
		theY = WindowSizeY / 2;
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE |GLUT_DEPTH);
		glutInitWindowSize(WindowSizeX, WindowSizeY);
		glutCreateWindow("test");
		glClearColor(0, 0, 0, 0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glutDisplayFunc(show_3d);
		glutKeyboardFunc(new_keyboard_function);	
		glutMouseFunc(new_mouse_function);
		glutMotionFunc(new_mouse_function_active);
		glutMainLoop();
		return 0;
	}//test2_load_3d_model

}