/*!
\file mapview.cpp

\brief Definition file for MapView class

This file contains all of the neccessary definitions for the MapView class.

\author Daniel Randle
\date December 11 2013
\copywrite Earth Banana Games 2013
*/

// Engine includes
#include <nsdebug.h>
#include <nsinput_system.h>
#include <nsrender_system.h>
#include <nsbuild_system.h>
#include <nsentity.h>
#include <nsplugin.h>
#include <nsscene.h>

// Qt Includes
#include <qevent.h>
#include <qpushbutton.h>
#include <mapview.h>
#include <toolkit.h>
#include <qtimer.h>
#include <camerasettingsdialog.h>
#include <qmessagebox.h>
#include <resource_browser.h>
#include <resource_dialog.h>
#include <resource_dialog_prev.h>
#include <resource_dialog_prev_lighting.h>

MapView::MapView(QWidget * parent) :
	QOpenGLWidget(parent)
{
}

MapView::~MapView()
{
}

uint32 MapView::glewID()
{
    return m_glew_id;
}

void MapView::init()
{
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
    nse.make_current(m_glew_id);
    nse.system<nsinput_system>()->set_cursor_pos(platform_normalized_mpos());
    nse.system<nsbuild_system>()->to_cursor();
}

void MapView::keyReleaseEvent(QKeyEvent * kEvent)
{
    nse.make_current(m_glew_id);
    if (kEvent->isAutoRepeat())
    {
        QOpenGLWidget::keyPressEvent(kEvent);
        return;
    }

    if (!_on_key(false,kEvent->key()))
        QOpenGLWidget::keyPressEvent(kEvent);
}

void MapView::keyPressEvent(QKeyEvent * kEvent)
{
    nse.make_current(m_glew_id);
    if (kEvent->isAutoRepeat())
    {
        QOpenGLWidget::keyPressEvent(kEvent);
        return;
    }

    if (!_on_key(true,kEvent->key()))
        QOpenGLWidget::keyPressEvent(kEvent);
}

bool MapView::_on_key(bool pressed, int key)
{
    switch (key)
    {
    case (Qt::Key_A) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_a, pressed);
        break;
    case (Qt::Key_B) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_b, pressed);
        break;
    case (Qt::Key_C) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_c, pressed);
        break;
    case (Qt::Key_D) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_d, pressed);
        break;
    case (Qt::Key_E) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_e, pressed);
        break;
    case (Qt::Key_F) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_f, pressed);
        break;
    case (Qt::Key_G) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_g, pressed);
        break;
    case (Qt::Key_H) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_h, pressed);
        break;
    case (Qt::Key_I) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_i, pressed);
        break;
    case (Qt::Key_J) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_j, pressed);
        break;
    case (Qt::Key_K) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_k, pressed);
        break;
    case (Qt::Key_L) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_l, pressed);
        break;
    case (Qt::Key_M) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_m, pressed);
        break;
    case (Qt::Key_N) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_n, pressed);
        break;
    case (Qt::Key_O) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_o, pressed);
        break;
    case (Qt::Key_P) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_p, pressed);
        break;
    case (Qt::Key_Q) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_q, pressed);
        break;
    case (Qt::Key_R) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_r, pressed);
        break;
    case (Qt::Key_S) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_s, pressed);
        break;
    case (Qt::Key_T) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_t, pressed);
        break;
    case (Qt::Key_U) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_u, pressed);
        break;
    case (Qt::Key_V) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_v, pressed);
        break;
    case (Qt::Key_W) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_w, pressed);
        break;
    case (Qt::Key_X) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_x, pressed);
        break;
    case (Qt::Key_Y) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_y, pressed);
        break;
    case (Qt::Key_Z) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_z, pressed);
        break;
    case (Qt::Key_0) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_keypad_0, pressed);
        break;
    case (Qt::Key_1) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_keypad_1, pressed);
        break;
    case (Qt::Key_2) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_keypad_2, pressed);
        break;
    case (Qt::Key_3) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_keypad_3, pressed);
        break;
    case (Qt::Key_4) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_keypad_4, pressed);
        break;
    case (Qt::Key_5) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_keypad_5, pressed);
        break;
    case (Qt::Key_6) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_keypad_6, pressed);
        break;
    case (Qt::Key_7) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_keypad_7, pressed);
        break;
    case (Qt::Key_8) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_keypad_8, pressed);
        break;
    case (Qt::Key_9) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_keypad_9, pressed);
        break;
    case (Qt::Key_Shift) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_lshift, pressed);
        break;
    case (Qt::Key_Control) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_lctrl, pressed);
        break;
    case (Qt::Key_Alt) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_lalt, pressed);
        break;
    case (Qt::Key_Tab) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_tab, pressed);
        break;
    case (Qt::Key_CapsLock) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_capslock, pressed);
        break;
    case (Qt::Key_Space) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_space, pressed);
        break;
    case (Qt::Key_Super_L) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_lsuper, pressed);
        break;
    case (Qt::Key_Super_R) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_rsuper, pressed);
        break;
    case (Qt::Key_Left) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_left, pressed);
        break;
    case (Qt::Key_Right) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_right, pressed);
        break;
    case (Qt::Key_Up) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_up, pressed);
        break;
    case (Qt::Key_Down) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_down, pressed);
        break;
    case (Qt::Key_Escape) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_esc, pressed);
        break;
    case (Qt::Key_Delete) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_delete, pressed);
        break;
    case (Qt::Key_Backspace) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_backspace, pressed);
        break;
    case (Qt::Key_Enter) :
        nse.event_dispatch()->push<nskey_event>(nsinput_map::key_enter, pressed);
        break;
    default:
        return false;
    }
    return true;
}

void MapView::mouseMoveEvent(QMouseEvent * mevent)
{
    nse.make_current(m_glew_id);
    nse.event_dispatch()->push<nsmouse_move_event>(platform_normalized_mpos());
    QOpenGLWidget::mouseMoveEvent(mevent);
}

void MapView::mouseReleaseEvent(QMouseEvent * mevent)
{
    nse.make_current(m_glew_id);
    if (!_on_mouse(false, mevent->button(), platform_normalized_mpos()))
        QOpenGLWidget::mousePressEvent(mevent);
}

void MapView::mousePressEvent(QMouseEvent * mevent)
{
    nse.make_current(m_glew_id);
    if (!_on_mouse(true, mevent->button(), platform_normalized_mpos()))
        QOpenGLWidget::mousePressEvent(mevent);
}

bool MapView::_on_mouse(bool pressed, int mbutton, const fvec2 & norm_mpos)
{
    switch (mbutton)
    {
    case (Qt::LeftButton) :
        nse.event_dispatch()->push<nsmouse_button_event>(nsinput_map::left_button, pressed, norm_mpos);
        break;
    case (Qt::RightButton) :
        nse.event_dispatch()->push<nsmouse_button_event>(nsinput_map::right_button, pressed, norm_mpos);
        break;
    case (Qt::MiddleButton) :
        nse.event_dispatch()->push<nsmouse_button_event>(nsinput_map::middle_button, pressed, norm_mpos);
        break;
    case (Qt::XButton1) :
        nse.event_dispatch()->push<nsmouse_button_event>(nsinput_map::aux_button_1, pressed, norm_mpos);
        break;
    case (Qt::XButton2) :
        nse.event_dispatch()->push<nsmouse_button_event>(nsinput_map::aux_button_2, pressed, norm_mpos);
        break;
    default:
        return false;
    }
    return true;

}

void MapView::mouseDoubleClickEvent(QMouseEvent *)
{
    QMessageBox mb;
    mb.setText("Double clicked");
    mb.exec();
}


void MapView::wheelEvent(QWheelEvent * wevent)
{
    nse.make_current(m_glew_id);
    nse.event_dispatch()->push<nsmouse_scroll_event>(wevent->delta(), platform_normalized_mpos());
}

fvec2 platform_normalized_mpos()
{
    QWidget * current_widget = QApplication::focusWidget();
    if (current_widget == 0)
        return fvec2();

    QPoint p = current_widget->mapFromGlobal(QCursor::pos());
    fvec2 pos(float(p.x()) / float(current_widget->width()), 1.0f - float(p.y()) / float(current_widget->height()));
    return pos;
}

void MapView::onIdle()
{
    if (hasFocus())
        update();
}

void MapView::initializeGL()
{
    bbtk.output_view()->writeToScreen("Initializing mapview opengL");

    m_glew_id = nse.create_context();

#ifdef NSDEBUG
    nse.debug()->set_message_callback(OutputView::debugCallback, bbtk.output_view());
#else
    bbtk.output_view()->hide();
#endif

    nse.start();

    nsplugin * plg = nse.load_plugin("testplug.bbp");
    plg->bind();
    nse.set_active(plg);

    // Setup build brush (simple one)
    nsinput_map * imap = plg->get<nsinput_map>("bb_toolkit");
    nse.set_current_scene("mainscene", false, false);
    nse.system<nsrender_system>()->set_fog_factor(uivec2(60,110));
    nse.system<nsrender_system>()->set_fog_color(fvec4(nse.current_scene()->bg_color(),1.0f));
    nse.system<nsinput_system>()->set_input_map(imap->full_id());
    nse.system<nsinput_system>()->push_context("Main");

	// Load from file all plugins - dont actually call load unless checked in load plugins screen
    bbtk.load_plugin_files(QDir(nse.plugin_dir().c_str()));

    bbtk.refresh_views();
    bbtk.statusBar()->setSizeGripEnabled(false);
    bbtk.statusBar()->showMessage("Ready");

    // Initialize stuff that needs to be initialized after the engine
    bbtk.res_browser()->init();
    bbtk.res_dialog()->init();
    bbtk.res_dialog_prev()->init();
    bbtk.res_dialog_prev_lighting()->init();
}

void MapView::make_current()
{
    makeCurrent();
    nse.make_current(m_glew_id);
}

void MapView::resizeGL(int width, int height)
{
    nse.make_current(m_glew_id);
    nsrender_system * rs = nse.system<nsrender_system>();
    if (rs != NULL)
    {
        rs->set_screen_fbo(defaultFramebufferObject());
        rs->resize_screen(ivec2(width,height));
    }
}

void MapView::paintGL()
{
    nse.make_current(m_glew_id);
    nse.update();
    bbtk.update_ui();
}
