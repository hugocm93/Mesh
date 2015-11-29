/*
 * Mesh.cpp
 *
 *  Created on: Nov 13, 2015
 *      Author: hugocm93
 */

#include "Mesh.h"

Mesh::Mesh(char name[]) {
	FileReader::read(name, &this->vertices, &this->triangles, &this->nVertices, &this->nTriangles, &this->Xmin, &this->Ymin, &this->Zmin, &this->Xmax, &this->Ymax, &this->Zmax);

}

Mesh::~Mesh() {
	delete []vertices;
	delete[] triangles;
	delete[] normals;
}


//getters


void Mesh::normalByFace(){

	this->normals = (float*)malloc(sizeof(float)*nTriangles*3);

	for(int i = 0 ; i < this->nTriangles*3 ; i+=3){

		int v[3];

		for(int j = 0 ; j < 3; j++){
			v[j] = this->triangles[i+j];
		}

		Vec3<float> vec[3];
		for(int j = 0 ; j < 3; j++){
			float x = this->vertices[v[j]*3];
			float y = this->vertices[v[j]*3+1];
			float z = this->vertices[v[j]*3+2];

			vec[j].setX(x);
			vec[j].setY(y);
			vec[j].setZ(z);

			//printf("%f %f %f\n", vec[j].getX(), vec[j].getY(), vec[j].getZ());

		}

		Vec3<float> normal = Vec3<float>::crossProduct((vec[2] - vec[0]), (vec[1] - vec[0]));
		normal.normalise();
		//printf("%f %f %f\n", normal.getX(), normal.getY(), normal.getZ());

		//printf("%d %d %d\n", v[0], v[1], v[2]);

		for(int j = 0 ; j < 3; j++){
			this->normals[v[j]*3] = normal.getX();
			this->normals[v[j]*3+1] = normal.getY();
			this->normals[v[j]*3+2] = normal.getZ();

			//printf("%f %f %f\n", this->normals[v[j]*3], this->normals[v[j]*3+1],this->normals[v[j]*3+2]);
		}
	}
}

void Mesh::normalByArea(){

}

void Mesh::normalByAngle(){

}

Vec3<double> Mesh::getCenterOfBoundingBox(){
	Vec3<double>* top = new Vec3<double>(Xmax, Ymax, Zmax);
	Vec3<double>* bottom = new Vec3<double>(Xmin, Ymin, Zmin);

	Vec3<double> aux = (*top - *bottom)/2 + *bottom;

	return aux;
}

double Mesh::getDiagonal(){
	Vec3<double>* top = new Vec3<double>(Xmax, Ymax, Zmax);
	Vec3<double>* bottom = new Vec3<double>(Xmin, Ymin, Zmin);


	return Vec3<double>::getDistance(*top, *bottom);
}

