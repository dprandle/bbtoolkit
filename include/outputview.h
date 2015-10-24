/*!
\file outputview.h

\brief Header file for OutputView class

This file contains all of the neccessary declarations for the OutputView class.

\author Daniel Randle
\date December 11 2013
\copywrite Earth Banana Games 2013
*/

#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

// Inlcudes
#include <qtextedit.h>
#include <toolkitdef.h>
#include <nsstring.h>

// Class Forward Declarations
class QVBoxLayout;
class QToolBar;
class nsengine;
class Toolkit;
class QAction;

class OutputView : public QWidget
{
	Q_OBJECT

public:
	OutputView(QWidget * parent=NULL);

	~OutputView();

    void init();

	void writeToScreen(const QString & text);

	void clearScreen();

	static void debugCallback(const nsstring & pMessage, void * pThis);

private slots:

	void onClearScreen();

	void onSelectAll();

	void onCopy();

private:
	void _connect();

	QTextEdit * screen;
	QToolBar * topToolBar;
	QAction * clrScreen;
	QAction * selectAll;
	QAction * copy;
};
#endif
