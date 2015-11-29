/*
 * main.cpp
 *
 *  Created on: Nov 13, 2015
 *      Author: hugocm93
 */

#include "MainWindow.h"

using namespace std;

int main(int argc, char* argv[]){
	IupOpen(&argc, &argv);

	IupGLCanvasOpen();

	MainWindow * mainWindow = new MainWindow();

	mainWindow->show();

	IupMainLoop();

	IupClose();

	return IUP_NOERROR;

	return 0;
}


