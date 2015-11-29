/*
 * FileReader.h
 *
 *  Created on: Nov 13, 2015
 *      Author: hugocm93
 */

#ifndef FILEREADER_H_
#define FILEREADER_H_

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>

class FileReader {
public:
	FileReader();
	virtual ~FileReader();

	static void read(const char name[], float** verticesI, int** trianglesI, int* numVertices, int* numTriangles, float* XminI, float* YminI, float* ZminI, float* XmaxI, float* YmaxI, float* ZmaxI);
};

#endif /* FILEREADER_H_ */
