/*!
\file mapview.cpp

\brief Definition file for MapView class

This file contains all of the neccessary definitions for the MapView class.

\author Daniel Randle
\date December 11 2013
\copywrite Earth Banana Games 2013
*/

#include <qpushbutton.h>
#include <mapview.h>
#include <toolkit.h>
#include <myGL/glew.h>
#include <nsmesh.h>
#include <nslogfile.h>
//#include <windows.h>
#include <nsdebug.h>
#include <nsshadermanager.h>
#include <camerasettingsdialog.h>
#include <nsmeshmanager.h>
#include <nsanimmanager.h>
#include <nsinputmanager.h>
#include <nsbuildsystem.h>
#include <nsoccupycomp.h>
#include <nstilecomp.h>
#include <nstilebrushcomp.h>
#include <nsmatmanager.h>
#include <nstexmanager.h>
#include <nsbufferobject.h>
#include <nsshader.h>
#include <nsglobal.h>
#include <nsanimset.h>
#include <nsanimcomp.h>
#include <nsrendersystem.h>
#include <nsscenemanager.h>
#include <nslightcomp.h>
#include <nscamcomp.h>
#include <nsentity.h>
#include <nspluginmanager.h>
#include <nsengine.h>
#include <nsentitymanager.h>
#include <nsinputcomp.h>
#include <nsplugin.h>
#include <nsselcomp.h>
#include <vector>
#include <nsinputsystem.h>
// Qt Includes
#include <qevent.h>
#include <qtimer.h>


MapView::MapView(QWidget * parent) :
	QOpenGLWidget(parent)
{
}

MapView::~MapView()
{}

nsuint MapView::glewID()
{
	return mGlewID;
}

void MapView::init(Toolkit * pTK)
{
	mTK = pTK;
	setMouseTracking(true);
	setFocusPolicy(Qt::StrongFocus);
	setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
	setMinimumSize(200, 200);
	_connect();
}


void MapView::_connect()
{
	QTimer * t = new QTimer();
	connect(t, SIGNAL(timeout()), this, SLOT(onIdle()));
	t->start(0);
}

void MapView::enterEvent(QEvent *)
{
	nsengine.makeCurrent(mGlewID);
	QPoint p = mapFromGlobal(QCursor::pos());
	fvec2 pos(nsfloat(p.x()) / nsfloat(width()), 1.0f - nsfloat(p.y()) / nsfloat(height()));
	nsengine.system<NSBuildSystem>()->toCursor(pos);
	nsengine.system<NSInputSystem>()->setLastPos(pos);
}

void MapView::keyReleaseEvent(QKeyEvent * kEvent)
{
	nsengine.makeCurrent(mGlewID);
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

void MapView::keyPressEvent(QKeyEvent * kEvent)
{
	nsengine.makeCurrent(mGlewID);
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

void MapView::mouseMoveEvent(QMouseEvent * mevent)
{
	nsengine.makeCurrent(mGlewID);
	nsfloat normXPos = nsfloat(mevent->pos().x()) / nsfloat(width());
	nsfloat normYPos = 1.0f - nsfloat(mevent->pos().y()) / nsfloat(height());

	nsengine.system<NSInputSystem>()->mouseMove(normXPos, normYPos);
	QOpenGLWidget::mouseMoveEvent(mevent);
}

void MapView::mouseReleaseEvent(QMouseEvent * mevent)
{
	nsengine.makeCurrent(mGlewID);
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

void MapView::mousePressEvent(QMouseEvent * mevent)
{
	nsengine.makeCurrent(mGlewID);
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

void MapView::mouseDoubleClickEvent(QMouseEvent *)
{
	nsengine.makeCurrent(mGlewID);
}

void MapView::onIdle()
{
	update();
}

void MapView::wheelEvent(QWheelEvent * wevent)
{
	nsengine.makeCurrent(mGlewID);
	nsfloat normXPos = nsfloat(wevent->pos().x()) / nsfloat(width());
	nsfloat normYPos = 1.0f - nsfloat(wevent->pos().y()) / nsfloat(height());

	nsengine.system<NSInputSystem>()->mouseScroll(wevent->delta(), normXPos, normYPos);
}

void MapView::initializeGL()
{
	mGlewID = nsengine.createContext();
	nsengine.start();
	mTK->camSettings()->init(mTK);

	// Load from file all plugins - dont actually call load unless checked in load plugins screen
	mTK->loadPluginFiles(QDir(nsengine.pluginDirectory().c_str()));
//	NSPlugin * plg = nsengine.createPlugin("buildandbattle");
//	plg->manager<NSSceneManager>()->setSaveMode(NSResManager::Text);
//	plg->createTile("grasstile", nsengine.importdir() + "diffuseGrass.png", nsengine.importdir() + "normalGrass.png", fvec3(1, 1, 1), 16.0f, 0.2f, fvec3(1, 1, 1), false, true);

#ifdef NSDEBUG
	nsengine.debug()->setMessageCallback(OutputView::debugCallback, mTK->outputView());
#else
	mUI.mOutputView->hide();
#endif

	mTK->refreshViews();
	mTK->statusBar()->setSizeGripEnabled(false);
	mTK->statusBar()->showMessage("Ready");
}

void MapView::resizeGL(int width, int height)
{
	nsengine.system<NSRenderSystem>()->setScreenfbo(defaultFramebufferObject());
	nsengine.makeCurrent(mGlewID);

	NSScene * sc = nsengine.currentScene(); if (sc == NULL) return;
	NSEntity * cam = sc->camera(); if (cam == NULL) return;
	cam->get<NSCamComp>()->resize(width, height);
	glViewport(0, 0, width, height);
}

void MapView::paintGL()
{
	if (hasFocus())
	{
		nsengine.makeCurrent(mGlewID);
		nsengine.update();
		mTK->updateUI();
	}
}
