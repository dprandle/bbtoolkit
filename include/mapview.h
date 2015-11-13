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
#include <qopenglwidget.h>
#include <toolkitdef.h>
#include <nsmath.h>

class Toolkit;
class nsengine;
class nsentity;
class nstex_manager;
class nsshader_manager;

class map_view : public QOpenGLWidget
{
	Q_OBJECT
public:
    map_view(QWidget * parent=NULL);
    ~map_view();

    uint32 glew_id();
    void init();
    void make_current();

public slots:
    void on_idle();

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
    bool _on_key(bool pressed, int key);
    bool _on_mouse(bool pressed, int mbutton, const fvec2 & norm_mpos);

    uint32 m_glew_id;
};

#endif // MAPWINDOW_H
