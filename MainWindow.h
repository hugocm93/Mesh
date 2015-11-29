/*
 * MainWindow.h
 *
 *  Created on: Oct 16, 2015
 *      Author: hugocm93
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <iup/iup.h>
#include <iup/iupgl.h>
#include <cstdlib>
#include <GL/gl.h>     /* OpenGL functions*/
#include <GL/glu.h>
#include <iostream>
#include "Mesh.h"
#include "FileReader.h"

class MainWindow {

private:
	Ihandle* dialog;
	Ihandle* canvas;
	Ihandle* toolBar;
	Ihandle* messageBar;
	Mesh* mesh;

public:
	MainWindow();
	virtual ~MainWindow();

	/*Shows the dialog*/
	void show();

	/*Hides the dialog*/
	void hide();

private:
	/* Callbacks*/
	static int load_cb(Ihandle* ih);
	static int exit_cb(Ihandle* ih);
	static int repaint_cb(Ihandle* ih);
	static int resize_cb(Ihandle *self, int new_width, int new_height);
	static int move(Ihandle* ih, int button, int pressed, int x, int y, char* status);
	static int move2(Ihandle *ih, int x, int y, char *status);

	/*Intern method that makes it easier to create a button*/
	static Ihandle* IupSButton(char const * image_file, char const * tip, Icallback callback);
	static void IupCanvasResize( Ihandle* canvas, Ihandle* parent_dialog, int new_width, int new_height );

};

#endif /* MAINWINDOW_H_ */
