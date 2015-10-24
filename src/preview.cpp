/*!
\file mapview.cpp

\brief Definition file for MapView class

This file contains all of the neccessary definitions for the MapView class.

\author Daniel Randle
\date December 11 2013
\copywrite Earth Banana Games 2013
*/





//#include <nsengine.h>
//#include <nsshader.h>
//#include <nsshader_manager.h>
//#include <nsinput_map_manager.h>
//#include <nstex_manager.h>
//#include <nsshader_manager.h>
//#include <nsentity.h>
//#include <nsmesh.h>
//#include <nsmesh_manager.h>
//#include <nsrender_comp.h>
//#include <nscam_comp.h>
//#include <nsmaterial.h>
//#include <nsbuild_system.h>
//#include <nsrender_system.h>
//#include <nsmat_manager.h>
//#include <nstexture.h>
//#include <nscamera_system.h>
//#include <nsscene.h>
//#include <nsinput_system.h>

// Qt Includes
#include <preview.h>
#include <qevent.h>
#include <qtimer.h>
#include <qmessagebox.h>
#include <toolkit.h>
#include <outputview.h>

Preview::Preview(QWidget * parent) :
QOpenGLWidget(parent)
{
}

Preview::~Preview()
{
//    if (nse.make_current(mEngineContextID))
//	{
//        nse.shutdown();
//        nse.destroyContext(mEngineContextID);
//	}
}


void Preview::init()
{
    _connect();
}


void Preview::_connect()
{
//	QTimer * t = new QTimer();
//	connect(t, SIGNAL(timeout()), this, SLOT(onIdle()));
//	t->start(0);
}

void Preview::enterEvent(QEvent * pEvent)
{
//    nse.make_current(mEngineContextID);
//	QPoint p = mapFromGlobal(QCursor::pos());
//    fvec2 pos(float(p.x()) / float(width()), 1.0f - float(p.y()) / float(height()));
//    nse.system<nsbuild_system>()->toCursor(pos);
//    nse.system<nsinput_system>()->setLastPos(pos);
}

void Preview::wheelEvent(QWheelEvent * wevent)
{
//    nse.make_current(mEngineContextID);
//    float normXPos = float(wevent->pos().x()) / float(width());
//    float normYPos = 1.0f - float(wevent->pos().y()) / float(height());

//    nse.system<nsinput_system>()->mouseScroll(wevent->delta(), normXPos, normYPos);
}

void Preview::keyReleaseEvent(QKeyEvent * kEvent)
{
//    nse.make_current(mEngineContextID);
//	if (kEvent->isAutoRepeat())
//	{
//		QOpenGLWidget::keyPressEvent(kEvent);
//		return;
//	}

//	switch (kEvent->key())
//	{
//	case (Qt::Key_W) :
//        nse.system<nsinput_system>()->keyRelease(nsinput_map::Key_W);
//		break;
//	case (Qt::Key_S) :
//        nse.system<nsinput_system>()->keyRelease(nsinput_map::Key_S);
//		break;
//	case (Qt::Key_A) :
//        nse.system<nsinput_system>()->keyRelease(nsinput_map::Key_A);
//		break;
//	case (Qt::Key_D) :
//        nse.system<nsinput_system>()->keyRelease(nsinput_map::Key_D);
//		break;
//	case (Qt::Key_Z) :
//        nse.system<nsinput_system>()->keyRelease(nsinput_map::Key_Z);
//		break;
//	case (Qt::Key_Y) :
//        nse.system<nsinput_system>()->keyRelease(nsinput_map::Key_Y);
//		break;
//	case (Qt::Key_B) :
//        nse.system<nsinput_system>()->keyRelease(nsinput_map::Key_B);
//		break;
//	case (Qt::Key_P) :
//        nse.system<nsinput_system>()->keyRelease(nsinput_map::Key_P);
//		break;
//	case (Qt::Key_Shift) :
//        nse.system<nsinput_system>()->keyRelease(nsinput_map::Key_LShift);
//		break;
//	case (Qt::Key_Control) :
//        nse.system<nsinput_system>()->keyRelease(nsinput_map::Key_LCtrl);
//		break;
//	default:
//		QOpenGLWidget::keyPressEvent(kEvent);
//	}
}

void Preview::keyPressEvent(QKeyEvent * kEvent)
{
//    nse.make_current(mEngineContextID);
//	if (kEvent->isAutoRepeat())
//	{
//		QOpenGLWidget::keyPressEvent(kEvent);
//		return;
//	}

//	switch (kEvent->key())
//	{
//	case (Qt::Key_W) :
//        nse.system<nsinput_system>()->keyPress(nsinput_map::Key_W);
//		break;
//	case (Qt::Key_Z) :
//        nse.system<nsinput_system>()->keyPress(nsinput_map::Key_Z);
//		break;
//	case (Qt::Key_Y) :
//        nse.system<nsinput_system>()->keyPress(nsinput_map::Key_Y);
//		break;
//	case (Qt::Key_S) :
//        nse.system<nsinput_system>()->keyPress(nsinput_map::Key_S);
//		break;
//	case (Qt::Key_A) :
//        nse.system<nsinput_system>()->keyPress(nsinput_map::Key_A);
//		break;
//	case (Qt::Key_D) :
//        nse.system<nsinput_system>()->keyPress(nsinput_map::Key_D);
//		break;
//	case (Qt::Key_B) :
//        nse.system<nsinput_system>()->keyPress(nsinput_map::Key_B);
//		break;
//	case (Qt::Key_P) :
//        nse.system<nsinput_system>()->keyPress(nsinput_map::Key_P);
//		break;
//	case (Qt::Key_Shift) :
//        nse.system<nsinput_system>()->keyPress(nsinput_map::Key_LShift);
//		break;
//	case (Qt::Key_Control) :
//        nse.system<nsinput_system>()->keyPress(nsinput_map::Key_LCtrl);
//		break;
//	default:
//		QOpenGLWidget::keyPressEvent(kEvent);
//	}
}

void Preview::mouseMoveEvent(QMouseEvent * mevent)
{
//    nse.make_current(mEngineContextID);
//    float normXPos = float(mevent->pos().x()) / float(width());
//    float normYPos = 1.0f - float(mevent->pos().y()) / float(height());

//    nse.system<nsinput_system>()->mouseMove(normXPos, normYPos);
//	QOpenGLWidget::mouseMoveEvent(mevent);
}

void Preview::mouseReleaseEvent(QMouseEvent * mevent)
{
//    nse.make_current(mEngineContextID);
//    float normXPos = float(mevent->pos().x()) / float(width());
//    float normYPos = 1.0f - float(mevent->pos().y()) / float(height());

//	if (mevent->button() == Qt::LeftButton)
//        nse.system<nsinput_system>()->mouseRelease(nsinput_map::LeftButton, normXPos, normYPos);
//	else if (mevent->button() == Qt::MiddleButton)
//        nse.system<nsinput_system>()->mouseRelease(nsinput_map::MiddleButton, normXPos, normYPos);
//	else if (mevent->button() == Qt::RightButton)
//        nse.system<nsinput_system>()->mouseRelease(nsinput_map::RightButton, normXPos, normYPos);
//	QOpenGLWidget::mouseReleaseEvent(mevent);
}

void Preview::mousePressEvent(QMouseEvent * mevent)
{
//    nse.make_current(mEngineContextID);
//    float normXPos = float(mevent->pos().x()) / float(width());
//    float normYPos = 1.0f - float(mevent->pos().y()) / float(height());

//	if (mevent->button() == Qt::LeftButton)
//        nse.system<nsinput_system>()->mousePress(nsinput_map::LeftButton, normXPos, normYPos);
//	else if (mevent->button() == Qt::MiddleButton)
//        nse.system<nsinput_system>()->mousePress(nsinput_map::MiddleButton, normXPos, normYPos);
//	else if (mevent->button() == Qt::RightButton)
//        nse.system<nsinput_system>()->mousePress(nsinput_map::RightButton, normXPos, normYPos);
//	QOpenGLWidget::mousePressEvent(mevent);
}

void Preview::mouseDoubleClickEvent(QMouseEvent * event)
{
//    nse.make_current(mEngineContextID);
}

void Preview::onIdle()
{
//	setFocus();
//	if (hasFocus())
//		update();
}

void Preview::initializeGL()
{	
    // mEngineContextID = nse.createContext(false);
    // nse.start();


	// Create basic scene with camera and directional light
    // nsplugin * plg = nse.createPlugin("preview");
    // nsengine & nseng = nse;
    // nsscene * scene = plg->create<nsscene>("preview");
    // nse.setCurrentScene(scene);
	// scene->setBackgroundColor(fvec3(0.7, 0.7, 0.8));
    // nsentity * dl = plg->createDirLight(plg->name() + "dlight", 0.6f, 0.3f);
    // nsentity * cam = plg->createCamera(plg->name() + "cam", 60.0f, uivec2(mTK->mapView()->width(), mTK->mapView()->height()), fvec2(DEFAULT_Z_NEAR, DEFAULT_Z_FAR));
	// scene->setCamera(cam);
	// scene->add(dl);
	
//	emit opengl_initialized(mEngineContextID);
}

void Preview::resizeGL(int width, int height)
{
//    nse.system<nsrender_system>()->setScreenfbo(defaultFramebufferObject());
//    nse.make_current(mEngineContextID);

//    nsscene * sc = nse.current_scene(); if (sc == NULL) return;
//    nsentity * cam = sc->camera(); if (cam == NULL) return;
//    cam->get<nscam_comp>()->resize(width, height);
//	glViewport(0, 0, width, height);
}

void Preview::paintGL()
{
//    nse.make_current(mEngineContextID);
//    nse.update();
}
