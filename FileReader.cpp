/*
 * FileReader.cpp
 *
 *  Created on: Nov 13, 2015
 *      Author: hugocm93
 */

#include "FileReader.h"


using namespace std;

FileReader::FileReader() {
	// TODO Auto-generated constructor stub

}

FileReader::~FileReader() {
	// TODO Auto-generated destructor stub
}

void FileReader::read(const char name[], float** verticesI, int** trianglesI, int* numVertices, int* numTriangles, float* XminI, float* YminI, float* ZminI, float* XmaxI, float* YmaxI, float* ZmaxI){

	float* vertices;
	int* triangles;
	FILE * file = fopen(name, "r");

	if(file == NULL){
		cout << "File was not found" << endl;
		exit(1);
	}

	char header[5];
	fscanf(file, "%[^\n]", header);
	header[3] = '\0';

	if(strcmp(header, "OFF") == 0){
		cout << "File is correct" << endl;
	}
	else{
		cout << "File is incorrect" << endl;
	}

	int nVertices, nTriangles, garbage;
	fscanf(file, " %d %d %d", &nVertices, &nTriangles, &garbage);

	vertices = (float*)malloc(sizeof(float)*nVertices*3);
	triangles = (int*)malloc(sizeof(int)*nTriangles*3);

	cout << "Alloc Vertices" << endl;

	float Xmax=0, Ymax=0, Zmax=0;
	float Xmin=100000, Ymin=100000, Zmin=100000;
	for(int i = 0; i < nVertices*3 ; i+=3){
		float x, y, z;
		fscanf(file, " %f %f %f", &x, &y, &z);

		vertices[i] = x;
		vertices[i+1] = y;
		vertices[i+2] = z;

		if(x<Xmin){
			Xmin = x;
		}
		else if(x>Xmax){
			Xmax = x;
		}

		if(y<Ymin){
			Ymin = y;
		}
		else if(y>Ymax){
			Ymax = y;
		}

		if(z<Zmin){
			Zmin = z;
		}
		else if(z>Zmax){
			Zmax = z;
		}


	}

	*XminI = Xmin;
	*YminI = Ymin;
	*ZminI = Zmin;

	*XmaxI = Xmax;
	*YmaxI = Ymax;
	*ZmaxI = Zmax;

	printf("%f %f %f\n", Xmax, Ymax, Zmax);
	printf("%f %f %f\n", Xmin, Ymin, Zmin);

	cout << "Alloc triangles" << endl;
	for(int i = 0; i < nTriangles*3 ; i+=3){
		int garbagef, v1, v2, v3;
		fscanf(file, " %d %d %d %d", &garbagef, &v1, &v2, &v3);

		triangles[i] = v1;
		triangles[i+1] = v2;
		triangles[i+2] = v3;

		//printf("%d %d %d\n", v1, v2, v3);
	}

	*verticesI = vertices;
	*trianglesI = triangles;

	*numVertices = nVertices;
	*numTriangles = nTriangles;

	cout << "Finished reading" << endl;
}

