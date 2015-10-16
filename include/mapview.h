/*!
\file mapview.h

\brief Header file for MapView class

This file contains all of the neccessary declarations for the MapView class.

\author Daniel Randle
\date December 11 2013
\copywrite Earth Banana Games 2013
*/

#ifndef MAPVIEW_H
#define MAPVIEW_H


// Includes
#include <myGL\glew.h>
#include <qopenglwidget.h>
#include <toolkitdef.h>
#include <nsglobal.h>

class Toolkit;
class NSEngine;
class NSEntity;
class NSTexManager;
class NSShaderManager;

class MapView : public QOpenGLWidget
{
	Q_OBJECT
public:
	MapView(QWidget * parent=NULL);
	~MapView();

	nsuint glewID();
	void init(Toolkit * pTK);

public slots:
	void onIdle();

protected:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

	void enterEvent(QEvent * pEevent);
	void keyPressEvent(QKeyEvent * keyEvent);
	void keyReleaseEvent(QKeyEvent * keyEvent);
	void mouseMoveEvent(QMouseEvent * mevent);
	void mousePressEvent(QMouseEvent * mevent);
	void mouseReleaseEvent(QMouseEvent * mevent);
	void mouseDoubleClickEvent(QMouseEvent * event);
	void wheelEvent(QWheelEvent * wevent);

private:
	void _connect();

	Toolkit * mTK;
	nsuint mGlewID;
};

#endif // MAPWINDOW_H