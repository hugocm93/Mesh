/*
 * MainWindow.cpp
 *
 *  Created on: Oct 16, 2015
 *      Author: hugocm93
 */

#include "MainWindow.h"


static int lastX, lastY;
static int pressedGlobal;

MainWindow::MainWindow() {

	/*Create canvas, define the canvas size and bind callbacks*/
	Ihandle* canvas = IupGLCanvas(NULL);
	IupSetfAttribute(canvas, IUP_RASTERSIZE, "%dx%d", 400, 400);
	IupSetCallback(canvas, IUP_ACTION, (Icallback)repaint_cb);
	IupSetCallback(canvas, IUP_BUTTON_CB, (Icallback)move);
	IupSetCallback(canvas, IUP_MOTION_CB, (Icallback)move2);
	IupSetCallback(canvas, IUP_RESIZE_CB, (Icallback)resize_cb);
	this->canvas = canvas;

	/*Create message bar at the bottom of the window*/
	Ihandle* msg = IupLabel("Message...");       /* label as a msg bar */
	IupSetfAttribute(msg,IUP_RASTERSIZE,"x%d",20);     /* define height in pixels */
	IupSetAttribute(msg,"EXPAND","HORIZONTAL");
	this->messageBar = msg;

	/*Create toolBar*/
	Ihandle* toolbar;
	Ihandle* load = IupSButton("img/file_open.bmp","Load an object",(Icallback) load_cb);
	toolbar=IupHbox(load, NULL);
	this->toolBar = toolbar;

	/*Insert componets into the vBox*/
	Ihandle * vBox = IupVbox(this->toolBar, this->canvas,this->messageBar, NULL);


	mesh = NULL;

	/*Create the dialog and set its attributes*/
	Ihandle* dialog = IupDialog(vBox);
	IupSetAttribute(dialog, "TITLE", "OpenGL");
	IupSetAttribute(dialog, "canvas", (const char *)canvas);
	IupSetAttribute(dialog, "dialog", (const char *)dialog);
	IupSetAttribute( dialog, IUP_BUFFER, IUP_DOUBLE );
	IupSetAttribute(dialog, "mesh", (const char *)mesh);
	IupSetAttribute(dialog, "messageBar", (const char *)messageBar);
	IupSetAttribute(dialog, "CLOSE_CB", "exit_cb");
	IupSetFunction("exit_cb", (Icallback) exit_cb);
	IupSetAttribute(dialog, "RESIZE", "NO");
	this->dialog = dialog;

}

MainWindow::~MainWindow() {

}

Ihandle* MainWindow::IupSButton(char const * image_file, char const * tip, Icallback callback)
{
	Ihandle* button = IupButton(NULL, NULL);
	IupSetAttribute(button, "IMAGE", image_file);
	IupSetAttribute(button,"TIP",tip);
	IupSetCallback(button, "ACTION", callback);

	return button;
}

void MainWindow::IupCanvasResize( Ihandle* canvas, Ihandle* parent_dialog, int new_width, int new_height )
{
	char buffer[64];
	sprintf(buffer,"%dx%d",new_width,new_height);
	IupStoreAttribute(canvas, IUP_RASTERSIZE, buffer);
	IupSetAttribute(parent_dialog, IUP_RASTERSIZE, NULL);
	IupShowXY(parent_dialog, IUP_CENTER, IUP_CENTER);
}

int MainWindow::repaint_cb(Ihandle *self)
{
	std::cout << "repaint" << std::endl;

	IupGLMakeCurrent(self);                /* all OpenGL primitives will be directed to this canvas */
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);  /* dark grey color in the RGBA mode A is the alpha channel (ignore) */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);          /* clear the color buffer */

	Ihandle* dialogR = (Ihandle*)IupGetAttribute(self, "dialog");
	Mesh* meshR = (Mesh*)IupGetAttribute(dialogR, (const char*)"mesh");

	if(meshR!=NULL){
		Vec3<double> at = meshR->getCenterOfBoundingBox();
		double diagonal = meshR->getDiagonal();

		GLfloat matSpecular[] = { 1, 0.1, 0.1, 1.0 };
		GLfloat matShininess[] = { 36.0 };
		GLfloat lightPosition[] = { at.getX(), at.getY()-diagonal, at.getZ(), 0.0 };
		GLfloat whiteLight[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat lmodelAmbient[] = { 0.1, 0.1, 0.1, 1.0 };

		glClearColor(0.0, 0.0, 0.0, 0.0);
		glShadeModel(GL_FLAT);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);

		//glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
		//glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
		glLightfv(GL_LIGHT0, GL_SPECULAR, whiteLight);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodelAmbient);
		glEnable( GL_COLOR_MATERIAL );
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(false);

		glBegin(GL_TRIANGLES);
		glColor3f(1, 0, 0);

		int ii =0;
		for(int i = 0 ; i < meshR->nTriangles*3 ; i+=3){

			int v[3];

			//printf("%d\n", i);
			for(int j = 0 ; j < 3; j++){
				v[j] = meshR->triangles[i+j];
			}
			//printf("%d %d %d\n", v[0], v[1], v[2]);
			//printf("?\n");

			for(int j = 0 ; j < 3; j++){

				if(i==0){
					//printf("numero vertice %d\n", v[j]);
				}
				float x = meshR->vertices[v[j]*3];
				float y = meshR->vertices[v[j]*3+1];
				float z = meshR->vertices[v[j]*3+2];
				//printf("?\n");

				float nx = meshR->normals[v[j]*3];
				float ny = meshR->normals[v[j]*3+1];
				float nz = meshR->normals[v[j]*3+2];

				glNormal3f ( nx , ny , nz ) ;
				//printf("------%f %f %f\n", nx, ny, nz);
				//printf("%d %d %d", v[j]*3, v[j]*3+1, v[j]*3+2);
				glVertex3f(x,y,z);

				if(nx==0 && ny==0 && nz==0){
					printf("asmdn,asmd ,asmd ,asm d,asmd ,amd \n");
				}

				//printf("%d\n", ii++);
				if(i==0){
					//printf("vertice de fato %f %f %f\n", x, y, z);
				}
			}
		}
		glEnd();
	}

	IupGLSwapBuffers(self);  /* change the back buffer with the front buffer */
	return IUP_DEFAULT; /* returns the control to the main loop */
}


/* function called in the event of changes in the width or in the height of the canvas */
int MainWindow::resize_cb(Ihandle *self, int new_width, int new_height)
{
	std::cout << "resize" << std::endl;

	IupGLMakeCurrent(self);  /* Make the canvas current in OpenGL */

	/* define the entire canvas as the viewport  */
	glViewport(0,0,new_width,new_height);

	Ihandle* dialogR = (Ihandle*)IupGetAttribute(self, "dialog");
	Mesh* meshR = (Mesh*)IupGetAttribute(dialogR, (const char*)"mesh");

	if(meshR!=NULL){
		Vec3<double> at = meshR->getCenterOfBoundingBox();
		double diagonal = meshR->getDiagonal();

		Vec3<double>* eye = new Vec3<double>(at.getX()+diagonal, at.getY(), at.getZ());

		/* transformation applied to each vertex */
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();           /* identity, i. e. no transformation */
		gluLookAt(eye->getX(), eye->getY(), eye->getZ(), at.getX(), at.getY(), at.getZ(), 0, 1, 0);

		/* projection transformation (orthographic in the xy plane) */
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45, 400/400., 0.1, 100);

		printf(" %f %f %f \n", eye->getX(), eye->getY(), eye->getZ());
	}

	/* update canvas size and repaint */
	Ihandle* canvas = (Ihandle*)IupGetAttribute(self, (const char*) "canvas");
	repaint_cb(canvas);

	return IUP_DEFAULT; /* return to the IUP main loop */
}

int MainWindow::load_cb(Ihandle* ih)
{
	/*Update message bar*/
	Ihandle* msg = (Ihandle*)IupGetAttribute(ih, (const char*)"messageBar");
	IupSetfAttribute(msg, "TITLE", "Load callback");

	/*Open load file dialog*/
	Ihandle* getfile = IupFileDlg();
	char* filename = NULL;
	IupSetAttribute(getfile, IUP_TITLE, "Load an object"  );
	IupSetAttribute(getfile, IUP_DIALOGTYPE, IUP_OPEN);
	IupSetAttribute(getfile, IUP_FILTER, "*off");
	IupPopup(getfile, IUP_CENTER, IUP_CENTER);
	filename = IupGetAttribute(getfile, IUP_VALUE);

	if (!filename) {
		IupSetfAttribute(msg, "TITLE", "Selection failed");
		return IUP_DEFAULT;
	}

	Mesh* meshR = (Mesh*)IupGetAttribute(ih, (const char*)"mesh");
	if(meshR!=NULL){
		delete meshR;
		meshR=NULL;
	}
	meshR = new Mesh(filename);
	meshR->normalByFace();

	Ihandle* dialogR = (Ihandle*)IupGetAttribute(ih, "dialog");
	IupSetAttribute(dialogR, "mesh", (const char *)meshR);

	resize_cb(ih, 400, 400);

	IupSetfAttribute(msg, "TITLE", "Object loaded");

	return IUP_DEFAULT;
}

int MainWindow::move(Ihandle* ih, int button, int pressed, int x, int y, char* status){
	pressedGlobal = pressed;
	//	if(pressed){
	//		lastX = x;
	//		lastY = y;
	//	}
	//	else{
	//		int dx = lastX - x;
	//		int dy = lastX - y;
	//
	//		Ihandle* dialogR = (Ihandle*)IupGetAttribute(ih, "dialog");
	//		Mesh* meshR = (Mesh*)IupGetAttribute(dialogR, (const char*)"mesh");
	//
	//		if(meshR!=NULL){
	//			Vec3<double> at = meshR->getCenterOfBoundingBox();
	//			double diagonal = meshR->getDiagonal();
	//			Vec3<double>* eye = new Vec3<double>(at.getX()+diagonal/2, at.getY(), at.getZ());
	//
	//			float aux = dx/diagonal;
	//			float aux2 = dy/diagonal;
	//
	//			glMatrixMode(GL_MODELVIEW);
	//			glTranslatef(at.getX(), at.getY(), at.getZ());
	//			glRotatef(aux*(180/3.1415), 0,1,0);
	//			glTranslatef(-at.getX(), -at.getY(), -at.getZ());
	//
	//			glTranslatef(at.getX(), at.getY(), at.getZ());
	//			glRotatef(aux2*(180/3.1415), 1,0,0);
	//			glTranslatef(-at.getX(), -at.getY(), -at.getZ());
	//
	//			/* update canvas size and repaint */
	//			Ihandle* canvas = (Ihandle*)IupGetAttribute(ih, (const char*) "canvas");
	//			repaint_cb(canvas);
	//			printf("dx %f", dx);
	//		}
	//	}
}

int MainWindow::move2(Ihandle *ih, int x, int y, char *status){
	if(pressedGlobal){
		int dx = x - lastX;
		int dy = y-lastY;

		Ihandle* dialogR = (Ihandle*)IupGetAttribute(ih, "dialog");
		Mesh* meshR = (Mesh*)IupGetAttribute(dialogR, (const char*)"mesh");

		if(meshR!=NULL){
			Vec3<double> at = meshR->getCenterOfBoundingBox();
			double diagonal = meshR->getDiagonal();
			Vec3<double>* eye = new Vec3<double>(at.getX()+diagonal, at.getY(), at.getZ());

			float aux = dx/(diagonal*1000);
			float aux2 = dy/(diagonal*1000);

			glMatrixMode(GL_MODELVIEW);
			glTranslatef(at.getX(), at.getY(), at.getZ());

			glRotatef(aux*(180/3.1415), 0,1,0);

			glRotatef(aux2*(180/3.1415), 1,0,0);

			glTranslatef(-at.getX(), -at.getY(), -at.getZ());

			/* update canvas size and repaint */
			Ihandle* canvas = (Ihandle*)IupGetAttribute(ih, (const char*) "canvas");
			repaint_cb(canvas);
			//printf("dx %f", dx);

			lastX = x;
			lastY = y;
		}
	}
	else{
		lastX = x;
		lastY = y;
	}
}

int MainWindow::exit_cb(Ihandle* ih)
{
	printf("Function to free memory and do finalizations...\n");
	return IUP_CLOSE;
}

void MainWindow::show(){
	IupShowXY(this->dialog, IUP_CENTER, IUP_CENTER);

}

void MainWindow::hide(){
	IupHide(this->dialog);

}




