#pragma once

#include <OpenMesh\Core\IO\MeshIO.hh>
#include <OpenMesh\Core\Mesh\TriMesh_ArrayKernelT.hh>
#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include <Windows.h>
#include <string>
#include "HalfEdge.h"
using namespace std;

GLfloat **vertex_3d;
GLfloat **normal_3d;
int **face_t,vertexnum,facenum;
int showmode=1,transmode=0;
GLfloat xR, yR,zR, scale=2,tx,ty,tz;
double oldx, oldy, new_x, new_y;





void show_3d() {
	glRotatef(xR, 1.0f, 0.0f, 0.0f); // 让物体旋转的函数 第一个参数是角度大小，后面的参数是旋转的法向量
	glRotatef(yR, 0.0f, 1.0f, 0.0f);
	glRotatef(zR, 0.0f, 0.0f, 1.0f);
	glTranslatef(tx, ty, tz);
	glScalef(scale, scale, scale);
	glClear(GL_COLOR_BUFFER_BIT);
	glLineWidth(0.5);
	if (showmode == 0) {
		for (int i = 0; i < facenum; i++) {
			glBegin(GL_TRIANGLES);
			glColor3f(0.5, 0, 0);
			glVertex3f(vertex_3d[face_t[i][0]][0], vertex_3d[face_t[i][0]][1], vertex_3d[face_t[i][0]][2]);
			glVertex3f(vertex_3d[face_t[i][1]][0], vertex_3d[face_t[i][1]][1], vertex_3d[face_t[i][1]][2]);
			glVertex3f(vertex_3d[face_t[i][2]][0], vertex_3d[face_t[i][2]][1], vertex_3d[face_t[i][2]][2]);
			glEnd();
			glFlush();
		}
	}
	else if (showmode == 1) {
		for (int i = 0; i < facenum; i++) {
			glBegin(GL_LINE_LOOP);
			glColor3f(1, 1, 1);
			glVertex3f(vertex_3d[face_t[i][0]][0], vertex_3d[face_t[i][0]][1], vertex_3d[face_t[i][0]][2]);
			glVertex3f(vertex_3d[face_t[i][1]][0], vertex_3d[face_t[i][1]][1], vertex_3d[face_t[i][1]][2]);
			glVertex3f(vertex_3d[face_t[i][2]][0], vertex_3d[face_t[i][2]][1], vertex_3d[face_t[i][2]][2]);
			glEnd();
			glFlush();
		}
	}
	else if (showmode == 2) {
		for (int i = 0; i < facenum; i++) {
			glBegin(GL_LINE_LOOP);
			glColor3f(1, 1, 1);
			glVertex3f(vertex_3d[face_t[i][0]][0], vertex_3d[face_t[i][0]][1], vertex_3d[face_t[i][0]][2]);
			glVertex3f(vertex_3d[face_t[i][1]][0], vertex_3d[face_t[i][1]][1], vertex_3d[face_t[i][1]][2]);
			glVertex3f(vertex_3d[face_t[i][2]][0], vertex_3d[face_t[i][2]][1], vertex_3d[face_t[i][2]][2]);
			glEnd();
			glFlush(); 
			glBegin(GL_TRIANGLES);
			glColor3f(0.05*i, 0, 0);
			glVertex3f(vertex_3d[face_t[i][0]][0], vertex_3d[face_t[i][0]][1], vertex_3d[face_t[i][0]][2]);
			glVertex3f(vertex_3d[face_t[i][1]][0], vertex_3d[face_t[i][1]][1], vertex_3d[face_t[i][1]][2]);
			glVertex3f(vertex_3d[face_t[i][2]][0], vertex_3d[face_t[i][2]][1], vertex_3d[face_t[i][2]][2]);
			glEnd();
			glFlush();
		}
	}
}


void new_mouse_function(int button, int state, int x, int y) {
	xR = yR = zR = 0;
	oldx = x;
	oldy = y;
	y = 500 - y;
	oldx = oldx/500;
	oldy = oldy/500;
	switch (button) {
	case GLUT_LEFT_BUTTON:
		transmode = 1;
		break;
	case GLUT_MIDDLE_BUTTON:
		tx = ty = 0;
		scale = 1;
		transmode = 2;
		if(state == GLUT_DOWN)
			showmode = (showmode + 1) % 3;
		break;
	case GLUT_RIGHT_BUTTON:
		transmode = 3;
		break;
	default:
		break;
	}
}

void new_mouse_function_active(int x, int y) {
	y = 500 - y;
	new_x = x;
	new_y = y;
	new_x = new_x / 500;
	new_y = new_y / 500;
	switch (transmode) {
	case 0:
		break;
	case 1:
		scale = 1;
		tx = (new_x- oldx );
		ty = (new_y-oldy);
		Sleep(50);
		break;
	case 2:
		
		break;
	case 3:
		tx = ty = 0;
		scale = new_y - oldy;
		if (scale > 0)scale=1.11;
		else scale = 0.9;
		Sleep(100);
		break;
	}
	oldx = new_x;
	oldy = new_y;
	show_3d();
}

int checkEX(char* file) {
	int i = 0;
	while (file[i]!=0)
	{
		if (file[i] != '.')
			i++;
		else {
			if (file[i + 1] == 'o'&&file[i + 2] == 'f'&&file[i + 3] == 'f')
				return 1;
			if (file[i + 1] == 'o'&&file[i + 2] == 'b'&&file[i + 3] == 'j')
				return 2;
			if (file[i + 1] == 's'&&file[i + 2] == 't'&&file[i + 3] == 'l')
				return 3;
			if (file[i + 1] == '3'&&file[i + 2] == 'd'&&file[i + 3] == 's')
				return 4;
		}
	}
	return 0;
}

void readfile(char* file) {
	ifstream infile(file);
	
	/*
		.off's struction:
			OFF
			<num_of_vertex> <num_of_face> <num_of_edge>  //num_of_edge often to be 0
			<x> <y> <z>
			//till describe all vertex
			<used_vertex> <vertex_0> <vertex_1>.....  //often use triangle face
		
		.obj's struction:
			<prefix> <arg0> <arg1>.....
			//till describe all information
			prefix list:
				v -- vertex , args include 3 float
				vt --vertex of texture ,args include 2 float
				vn --vertex of normal(法线),args include 3 float
				f -- face(triangle), includes 3 args .args can be like <v> <v/vt> <v/vt/vn> <v//vn>
				# -- means this line is comments
			tip: in .obj , index begins with 1 not 0 ,which should be paid attention

		.stl's struction
			solid <filename>
				facet normal <x> <y> <z>
					outer loop
						vertex <x> <y> <z>
						vertex <x> <y> <z>
						vertex <x> <y> <z>
					end loop
				end facet
				//till describe all face
			endsolid <filename>
	*/
	string templine;
	istringstream tempstream;
	string token;
	float n0, n1, n2;
	int n_num=0,linenum=0;
	switch (checkEX(file)) {
	case 0:
		cout << "invalid path!";
		return;
	case 1:
		while (getline(infile, templine)) {
			if (linenum == 0);
			else {
				if (linenum == 1) {
					tempstream = istringstream(templine);
					tempstream >> vertexnum >> facenum >> n_num;
					vertex_3d = new GLfloat*[vertexnum];
					face_t = new int*[facenum];
					for (int i = 0; i < vertexnum; i++)
						vertex_3d[i] = new GLfloat[3];
					for (int i = 0; i < facenum; i++)
						face_t[i] = new int[3];
					n_num = 0;
				}
				else {
					tempstream = istringstream(templine);
					if (linenum - 2 < vertexnum)
						tempstream >> vertex_3d[linenum - 2][0] >> vertex_3d[linenum - 2][1] >> vertex_3d[linenum - 2][2];
					else {
						tempstream >> token >> face_t[n_num][0] >> face_t[n_num][1] >> face_t[n_num][2];
						n_num++;
					}
				}
			}
			linenum++;
		}
		break;
	case 2:
		facenum = 0;
		vertexnum = 0;
		vertex_3d = new GLfloat*[200];
		face_t = new int*[200];
		for (int i = 0; i < 200; i++)
			vertex_3d[i] = new GLfloat[3];
		for (int i = 0; i < 200; i++)
			face_t[i] = new int[3];
		while (getline(infile, templine)) {
			tempstream = istringstream(templine);
			tempstream >> token >> n0 >> n1>>n2;
			if (token.at(0)=='#') {}
			else if (token.at(0) == 'f') {
				face_t[facenum][0] = n0-1;
				face_t[facenum][1] = n1-1;
				face_t[facenum][2] = n2-1;
				facenum++;
			}
			else if (token.at(0) == 'v') {
				int c = token.length();
				if (c==1) {
					vertex_3d[vertexnum][0] = n0;
					vertex_3d[vertexnum][1] = n1;
					vertex_3d[vertexnum][2] = n2;
					vertexnum++  ;}
				else if (token.at(1) == 't'){}
				else if (token.at(1) == 'n') {}
			}
		}
		break;
	case 3:
		facenum = 0;
		vertexnum = 0;
		while (getline(infile, templine)) {
			tempstream = istringstream(templine);
			tempstream >> token;
			if (token.at(0) == 'f') {
				facenum++;
				vertexnum+=3;
			}
		}
		infile = ifstream(file);				
		vertex_3d = new GLfloat*[vertexnum];
		face_t = new int*[facenum];
		for (int i = 0; i < vertexnum; i++)
			vertex_3d[i] = new GLfloat[3];
		for (int i = 0; i < facenum; i++)
			face_t[i] = new int[3];
		getline(infile, templine);//read first sentence
		int n;
		for (int i = 0; i < facenum; i++) {
			n = i * 3;
			getline(infile, templine); // normal vector
			face_t[i][0] = n;
			face_t[i][1] = n+1;
			face_t[i][2] = n+2;
			getline(infile, templine); //"outer loop"
			getline(infile, templine); //1st vertex
			tempstream = istringstream(templine);
			tempstream >> token >> vertex_3d[n][0] >> vertex_3d[n][1] >> vertex_3d[n][2];
			getline(infile, templine); //2nd vertex
			tempstream = istringstream(templine);
			tempstream >> token >> vertex_3d[n+1][0] >> vertex_3d[n+1][1] >> vertex_3d[n+1][2];
			getline(infile, templine); //3rd vertex
			tempstream = istringstream(templine);
			tempstream >> token >> vertex_3d[n+2][0] >> vertex_3d[n+2][1] >> vertex_3d[n+2][2];
			getline(infile, templine); //"end loop"
			getline(infile, templine); //"end facet"
		}
		break;
	}
}

void subdivision() {
	for (int i = 0; i < vertexnum; i++) {
		add_vertex(vertex_3d[i][0], vertex_3d[i][1], vertex_3d[i][2]);
	}
	for (int i = 0; i < facenum; i++) {
		add_face(face_t[i][0], face_t[i][1], face_t[i][2]);
	}
	for (int i = 0; i < facenum; i++) {
		change_face(face_t[i][0], face_t[i][1], face_t[i][2]);
	}
	vertexnum = vNum+eNum;
	facenum =newfacenum;
	vertex_3d = new_vertexs;
	face_t = new_faces;
	reset();
}


void new_keyboard_function(unsigned char a, int x, int y) {
	tx = ty = tz = 0; scale = 1;
	switch (a)
	{
	case 27:
		exit(0);
	case '`':
		glLoadIdentity();
		break;
	case '1':
		readfile("C:\\Users\\aria\\Documents\\test.off");
		glLoadIdentity();
		break;
	case '2':
		readfile("C:\\Users\\aria\\Documents\\test.obj");
		glLoadIdentity();
		break;
	case '3':
		readfile("C:\\Users\\aria\\Documents\\test.stl");
		glLoadIdentity();
		break;
	case 'w':
		yR = 0; zR = 0;
		if (xR < 0)xR = 0;
		xR += 0.01;
		break;
	case 's':
		yR = 0; zR = 0;
		if (xR > 0)xR = 0;
		xR -= 0.01;
		break;
	case 'a':
		xR = 0; zR = 0;
		if (yR < 0)yR = 0;
		yR += 0.01;
		break;
	case 'd':
		xR = 0; zR = 0;
		if (yR > 0)yR = 0;
		yR -= 0.01;
		break;
	case 'q':
		xR = 0; yR = 0;
		if (zR < 0)zR = 0;
		zR += 0.01;
		break;
	case 'e':
		xR = 0; yR = 0;
		if (zR > 0)zR = 0;
		zR -= 0.01;
		break;
	case 'z':
		subdivision();
		break;
	default:
		break;
	}
	show_3d();
}
