/*!
\file mapview.cpp

\brief Definition file for MapView class

This file contains all of the neccessary definitions for the MapView class.

\author Daniel Randle
\date December 11 2013
\copywrite Earth Banana Games 2013
*/


#include <preview.h>

// Qt Includes
#include <qevent.h>
#include <qtimer.h>
#include <nsengine.h>
#include <nsshader.h>
#include <nsshadermanager.h>
#include <nsinputmanager.h>
#include <qmessagebox.h>
#include <nstexmanager.h>
#include <nsshadermanager.h>
#include <nsentity.h>
#include <nsmesh.h>
#include <nsmeshmanager.h>
#include <nsrendercomp.h>
#include <nscamcomp.h>
#include <nsmaterial.h>
#include <nsbuildsystem.h>
#include <nsrendersystem.h>
#include <toolkit.h>
#include <nsmatmanager.h>
#include <nstexture.h>
#include <outputview.h>
#include <nscamerasystem.h>
#include <nsscene.h>
#include <nsinputsystem.h>
Preview::Preview(QWidget * parent) :
QOpenGLWidget(parent),
mTK(NULL)
{
}

Preview::~Preview()
{
	if (nsengine.makeCurrent(mEngineContextID))
	{
		nsengine.shutdown();
		nsengine.delContext(mEngineContextID);
	}
}

void Preview::init(Toolkit * pTK)
{
	mTK = pTK;
//	_connect();
}


void Preview::_connect()
{
	QTimer * t = new QTimer();
	connect(t, SIGNAL(timeout()), this, SLOT(onIdle()));
	t->start(0);
}

void Preview::enterEvent(QEvent * pEvent)
{
	nsengine.makeCurrent(mEngineContextID);
	QPoint p = mapFromGlobal(QCursor::pos());
	fvec2 pos(nsfloat(p.x()) / nsfloat(width()), 1.0f - nsfloat(p.y()) / nsfloat(height()));
	nsengine.system<NSBuildSystem>()->toCursor(pos);
	nsengine.system<NSInputSystem>()->setLastPos(pos);
}

void Preview::wheelEvent(QWheelEvent * wevent)
{
	nsengine.makeCurrent(mEngineContextID);
	nsfloat normXPos = nsfloat(wevent->pos().x()) / nsfloat(width());
	nsfloat normYPos = 1.0f - nsfloat(wevent->pos().y()) / nsfloat(height());

	nsengine.system<NSInputSystem>()->mouseScroll(wevent->delta(), normXPos, normYPos);
}

void Preview::keyReleaseEvent(QKeyEvent * kEvent)
{
	nsengine.makeCurrent(mEngineContextID);
	if (kEvent->isAutoRepeat())
	{
		QOpenGLWidget::keyPressEvent(kEvent);
		return;
	}

	switch (kEvent->key())
	{
	case (Qt::Key_W) :
		nsengine.system<NSInputSystem>()->keyRelease(NSInputMap::Key_W);
		break;
	case (Qt::Key_S) :
		nsengine.system<NSInputSystem>()->keyRelease(NSInputMap::Key_S);
		break;
	case (Qt::Key_A) :
		nsengine.system<NSInputSystem>()->keyRelease(NSInputMap::Key_A);
		break;
	case (Qt::Key_D) :
		nsengine.system<NSInputSystem>()->keyRelease(NSInputMap::Key_D);
		break;
	case (Qt::Key_Z) :
		nsengine.system<NSInputSystem>()->keyRelease(NSInputMap::Key_Z);
		break;
	case (Qt::Key_Y) :
		nsengine.system<NSInputSystem>()->keyRelease(NSInputMap::Key_Y);
		break;
	case (Qt::Key_B) :
		nsengine.system<NSInputSystem>()->keyRelease(NSInputMap::Key_B);
		break;
	case (Qt::Key_P) :
		nsengine.system<NSInputSystem>()->keyRelease(NSInputMap::Key_P);
		break;
	case (Qt::Key_Shift) :
		nsengine.system<NSInputSystem>()->keyRelease(NSInputMap::Key_LShift);
		break;
	case (Qt::Key_Control) :
		nsengine.system<NSInputSystem>()->keyRelease(NSInputMap::Key_LCtrl);
		break;
	default:
		QOpenGLWidget::keyPressEvent(kEvent);
	}
}

void Preview::keyPressEvent(QKeyEvent * kEvent)
{
	nsengine.makeCurrent(mEngineContextID);
	if (kEvent->isAutoRepeat())
	{
		QOpenGLWidget::keyPressEvent(kEvent);
		return;
	}

	switch (kEvent->key())
	{
	case (Qt::Key_W) :
		nsengine.system<NSInputSystem>()->keyPress(NSInputMap::Key_W);
		break;
	case (Qt::Key_Z) :
		nsengine.system<NSInputSystem>()->keyPress(NSInputMap::Key_Z);
		break;
	case (Qt::Key_Y) :
		nsengine.system<NSInputSystem>()->keyPress(NSInputMap::Key_Y);
		break;
	case (Qt::Key_S) :
		nsengine.system<NSInputSystem>()->keyPress(NSInputMap::Key_S);
		break;
	case (Qt::Key_A) :
		nsengine.system<NSInputSystem>()->keyPress(NSInputMap::Key_A);
		break;
	case (Qt::Key_D) :
		nsengine.system<NSInputSystem>()->keyPress(NSInputMap::Key_D);
		break;
	case (Qt::Key_B) :
		nsengine.system<NSInputSystem>()->keyPress(NSInputMap::Key_B);
		break;
	case (Qt::Key_P) :
		nsengine.system<NSInputSystem>()->keyPress(NSInputMap::Key_P);
		break;
	case (Qt::Key_Shift) :
		nsengine.system<NSInputSystem>()->keyPress(NSInputMap::Key_LShift);
		break;
	case (Qt::Key_Control) :
		nsengine.system<NSInputSystem>()->keyPress(NSInputMap::Key_LCtrl);
		break;
	default:
		QOpenGLWidget::keyPressEvent(kEvent);
	}
}

void Preview::mouseMoveEvent(QMouseEvent * mevent)
{
	nsengine.makeCurrent(mEngineContextID);
	nsfloat normXPos = nsfloat(mevent->pos().x()) / nsfloat(width());
	nsfloat normYPos = 1.0f - nsfloat(mevent->pos().y()) / nsfloat(height());

	nsengine.system<NSInputSystem>()->mouseMove(normXPos, normYPos);
	QOpenGLWidget::mouseMoveEvent(mevent);
}

void Preview::mouseReleaseEvent(QMouseEvent * mevent)
{
	nsengine.makeCurrent(mEngineContextID);
	nsfloat normXPos = nsfloat(mevent->pos().x()) / nsfloat(width());
	nsfloat normYPos = 1.0f - nsfloat(mevent->pos().y()) / nsfloat(height());

	if (mevent->button() == Qt::LeftButton)
		nsengine.system<NSInputSystem>()->mouseRelease(NSInputMap::LeftButton, normXPos, normYPos);
	else if (mevent->button() == Qt::MiddleButton)
		nsengine.system<NSInputSystem>()->mouseRelease(NSInputMap::MiddleButton, normXPos, normYPos);
	else if (mevent->button() == Qt::RightButton)
		nsengine.system<NSInputSystem>()->mouseRelease(NSInputMap::RightButton, normXPos, normYPos);
	QOpenGLWidget::mouseReleaseEvent(mevent);
}

void Preview::mousePressEvent(QMouseEvent * mevent)
{
	nsengine.makeCurrent(mEngineContextID);
	nsfloat normXPos = nsfloat(mevent->pos().x()) / nsfloat(width());
	nsfloat normYPos = 1.0f - nsfloat(mevent->pos().y()) / nsfloat(height());

	if (mevent->button() == Qt::LeftButton)
		nsengine.system<NSInputSystem>()->mousePress(NSInputMap::LeftButton, normXPos, normYPos);
	else if (mevent->button() == Qt::MiddleButton)
		nsengine.system<NSInputSystem>()->mousePress(NSInputMap::MiddleButton, normXPos, normYPos);
	else if (mevent->button() == Qt::RightButton)
		nsengine.system<NSInputSystem>()->mousePress(NSInputMap::RightButton, normXPos, normYPos);
	QOpenGLWidget::mousePressEvent(mevent);
}

void Preview::mouseDoubleClickEvent(QMouseEvent * event)
{
	nsengine.makeCurrent(mEngineContextID);
}

void Preview::onIdle()
{
	setFocus();
	if (hasFocus())
		update();
}

void Preview::initializeGL()
{	
	// mEngineContextID = nsengine.createContext(false);
	// nsengine.start();


	// Create basic scene with camera and directional light
	// NSPlugin * plg = nsengine.createPlugin("preview");
	// NSEngine & nseng = nsengine;
	// NSScene * scene = plg->create<NSScene>("preview");
	// nsengine.setCurrentScene(scene);
	// scene->setBackgroundColor(fvec3(0.7, 0.7, 0.8));
	// NSEntity * dl = plg->createDirLight(plg->name() + "dlight", 0.6f, 0.3f);
	// NSEntity * cam = plg->createCamera(plg->name() + "cam", 60.0f, uivec2(mTK->mapView()->width(), mTK->mapView()->height()), fvec2(DEFAULT_Z_NEAR, DEFAULT_Z_FAR));
	// scene->setCamera(cam);
	// scene->add(dl);
	
//	emit opengl_initialized(mEngineContextID);
}

void Preview::resizeGL(int width, int height)
{
	nsengine.system<NSRenderSystem>()->setScreenfbo(defaultFramebufferObject());
	nsengine.makeCurrent(mEngineContextID);

	NSScene * sc = nsengine.currentScene(); if (sc == NULL) return;
	NSEntity * cam = sc->camera(); if (cam == NULL) return;
	cam->get<NSCamComp>()->resize(width, height);
	glViewport(0, 0, width, height);
}

void Preview::paintGL()
{
	nsengine.makeCurrent(mEngineContextID);
	nsengine.update();
}
