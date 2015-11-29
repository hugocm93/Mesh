/*
 * Mesh.h
 *
 *  Created on: Nov 13, 2015
 *      Author: hugocm93
 */

#ifndef MESH_H_
#define MESH_H_

#include "FileReader.h"
#include <iostream>
#include "Vec3.hpp"

class Mesh {

public:
	float* vertices;
	int* triangles;
	float* normals;

	int nVertices;
	int nTriangles;
	int nNormals;

	float Xmin, Ymin,Zmin;
	float Xmax, Ymax, Zmax;

public:
	void normalByFace();
	void normalByArea();
	void normalByAngle();
public:
	Mesh(char name[]);
	virtual ~Mesh();
	Vec3<double> getCenterOfBoundingBox();
	double getDiagonal();
};

#endif /* MESH_H_ */
