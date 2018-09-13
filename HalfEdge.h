#pragma once
#include <iostream>
#include <unordered_map>
using namespace std;


unordered_map<int, int> edges = unordered_map<int, int>();

struct Oedge {
	int v0, v1;//边的两个顶点
	int v2;//边所在的第一个面的另一个顶点
	int v3 = -1;//边所在的可能存在的另一个面的另一个顶点
	bool changed = false;//是否已经进行过修改
	int v4 = -1;//进行网格细分后增添的一个顶点
};

struct Overtex {
	float x, y, z;//顶点的位置信息
	int edges=0;//相邻的边数
	int* adjusts=new int[500];//相邻的顶点指针
	bool changed = false;//是否已经进行过修改
};
	//第一步，遍历所有顶点，存储xyz信息
	//第二步，遍历所有面，对每个面分别遍历其三条边
	//对每条边执行如下操作（要按v0<v1操作）：将两个顶点互相加入其相邻顶点列表
		//检查边是否已经存在，已经存在则将另一个顶点为v3赋值
		//如果边不存在则创建，并且将另一个边为v2赋值。
	int vNum = 0, eNum = 0, fNum = 0;
	Oedge* Edges = new Oedge[50000];
	Overtex* Vertexs = new Overtex[10000];
	
	void add_vertex(float x, float y, float z) {
		Vertexs[vNum].x = x;
		Vertexs[vNum].y = y;
		Vertexs[vNum].z = z;
		vNum++;
	}

	void add_edge(int v0, int v1, int v2) {
		if (v0 >= v1)
			swap(v0, v1);
		if (edges.find(v0*vNum+v1) != edges.end()) {
			Edges[edges[v0*vNum+v1]].v3 = v2;
		}
		else {
			Edges[eNum].v0 = v0;
			Edges[eNum].v1 = v1;
			Edges[eNum].v2 = v2;
			edges[v0*vNum+v1] = eNum;
			eNum++;
			Vertexs[v0].adjusts[Vertexs[v0].edges] = v1;
			Vertexs[v0].edges += 1;
			Vertexs[v1].adjusts[Vertexs[v1].edges] = v0;
			Vertexs[v1].edges += 1;
		}
	}

void add_face(int v0, int v1, int v2) {
		add_edge(v0, v1, v2);
		add_edge(v1, v2, v0);
		add_edge(v2, v0, v1);
		fNum++;
	}

	

float** new_vertexs;
int** new_faces;
int newnum = 0;//用于新增加的点的存储
int newfacenum = 0;

//逐个面修改顶点。修改过程如下。
//设面的原顶点是ABC，生成的三个新顶点是EFG，则四个新面是AEB，BFC，CGA，EFG

void change_vertex(int v) {
	if (Vertexs[v].changed)
		return;
	int n = Vertexs[v].edges;
	float pie = cos(2 * 3.14159 / n);
	float k = 3, z = 8;
	if (n == 2) {
		new_vertexs[v][0] = 3 / 4 * (Vertexs[v].x) + 1 / 8 * (Vertexs[Vertexs[v].adjusts[0]].x + Vertexs[Vertexs[v].adjusts[1]].x);
		new_vertexs[v][1] = 3 / 4 * (Vertexs[v].y) + 1 / 8 * (Vertexs[Vertexs[v].adjusts[0]].y + Vertexs[Vertexs[v].adjusts[1]].y);
		new_vertexs[v][2] = 3 / 4 * (Vertexs[v].z) + 1 / 8 * (Vertexs[Vertexs[v].adjusts[0]].z + Vertexs[Vertexs[v].adjusts[1]].z);
	}
	else {
		pie /= 4;
		pie +=(k/z);
		k = 5;
		pie = pie*pie;
		pie = (k/z)-pie;
		pie = pie / n;
		float beta = pie;
		new_vertexs[v][0] = (1 - n*beta)* (Vertexs[v].x);
		new_vertexs[v][1] = (1 - n*beta)* (Vertexs[v].y);
		new_vertexs[v][2] = (1 - n*beta)* (Vertexs[v].z);
		for (int i = 0; i < n; i++) {
			new_vertexs[v][0] += Vertexs[Vertexs[v].adjusts[i]].x *beta;
			new_vertexs[v][1] += Vertexs[Vertexs[v].adjusts[i]].y *beta;
			new_vertexs[v][2] += Vertexs[Vertexs[v].adjusts[i]].z *beta;
		}
	}
	Vertexs[v].changed = true;
}

int change_edge(int v0, int v1) {
	if (v0 >= v1)
		swap(v0, v1);
	int e = edges[v0*vNum + v1];
	if (Edges[e].changed)
		return Edges[e].v4;
	if (Edges[e].v3 == -1) {
		new_vertexs[newnum][0] = (Vertexs[Edges[e].v0].x + Vertexs[Edges[e].v1].x) / 2;
		new_vertexs[newnum][1] = (Vertexs[Edges[e].v0].y + Vertexs[Edges[e].v1].y) / 2;
		new_vertexs[newnum][2] = (Vertexs[Edges[e].v0].z + Vertexs[Edges[e].v1].z) / 2;
		Edges[e].v4 = newnum;
		newnum++;
		Edges[e].changed = true;
		return (newnum - 1);
	}

	new_vertexs[newnum][0] = (Vertexs[Edges[e].v0].x + Vertexs[Edges[e].v1].x) *3/8+(Vertexs[Edges[e].v2].x + Vertexs[Edges[e].v3].x)  / 8;
	new_vertexs[newnum][1] = (Vertexs[Edges[e].v0].y + Vertexs[Edges[e].v1].y) *3/8 + (Vertexs[Edges[e].v2].y + Vertexs[Edges[e].v3].y) / 8;
	new_vertexs[newnum][2] = (Vertexs[Edges[e].v0].z + Vertexs[Edges[e].v1].z) *3/8 + (Vertexs[Edges[e].v2].z + Vertexs[Edges[e].v3].z) / 8;
	Edges[e].v4 = newnum; 
	newnum++;
	Edges[e].changed = true;
	return (newnum - 1);
}

void change_face(int v0, int v1, int v2) {
	int v3, v4, v5;
	if (newnum == 0) {
		newnum = vNum;
		new_vertexs = new float*[vNum + eNum ];
		for (int i = 0; i < vNum +eNum; i++)
			new_vertexs[i] = new float[3];
		new_faces = new int*[fNum * 4];
		for (int i = 0; i < fNum * 4; i++)
			new_faces[i] = new int[3];
	}
	v3=change_edge(v0, v1);
	v4=change_edge(v1, v2);
	v5=change_edge(v2, v0);
	change_vertex(v0);
	change_vertex(v1);
	change_vertex(v2);
	new_faces[newfacenum][0] = v0;
	new_faces[newfacenum][1] = v3;
	new_faces[newfacenum][2] = v1;
	newfacenum++;
	new_faces[newfacenum][0] = v1;
	new_faces[newfacenum][1] = v4;
	new_faces[newfacenum][2] = v2;
	newfacenum++;
	new_faces[newfacenum][0] = v2;
	new_faces[newfacenum][1] = v5;
	new_faces[newfacenum][2] = v0;
	newfacenum++;
	new_faces[newfacenum][0] = v3;
	new_faces[newfacenum][1] = v4;
	new_faces[newfacenum][2] = v5;
	newfacenum++;
}

void reset() {
	vNum = eNum = fNum = newnum = newfacenum = 0;
	edges.clear();
	Edges = new Oedge[50000];
	Vertexs = new Overtex[10000];
}