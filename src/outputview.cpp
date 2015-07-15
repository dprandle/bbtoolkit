/*!
\file outputview.cpp

\brief Definition file for OutputView class

This file contains all of the neccessary definitions for the OutputView class.

\author Daniel Randle
\date December 11 2013
\copywrite Earth Banana Games 2013
*/

// Includes
#include <toolkitdef.h>
#include <outputview.h>
#include <qboxlayout.h>
#include <qtoolbar.h>
#include <qaction.h>
#include <nsengine.h>
#include <toolkit.h>



OutputView::OutputView(QWidget * parent) : 
screen(new QTextEdit(this)),
topToolBar(new QToolBar(this)),
clrScreen(new QAction(QIcon(), OUTVIEW_CLRSCRN_ACTION, this)),
selectAll(new QAction(QIcon(), OUTVIEW_SELECTALL_ACTION, this)),
copy(new QAction(QIcon(), OUTVIEW_COPY_ACTION, this)),
QWidget(parent)
{}

OutputView::~OutputView()
{}

void OutputView::init(Toolkit * pTK)
{
	mTK = pTK;
	// Setup toolbar
	topToolBar->addAction(clrScreen);
	topToolBar->addAction(selectAll);
	topToolBar->addAction(copy);
	topToolBar->setFixedHeight(30);
	// Setup page layout
	QVBoxLayout * pageLayout = new QVBoxLayout();
	pageLayout->addWidget(topToolBar);
	pageLayout->addWidget(screen);
	pageLayout->setContentsMargins(0, 0, 0, 0);
	pageLayout->setSpacing(0);
	setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
	
	setLayout(pageLayout);

	// Set starting settings
	screen->setReadOnly(true);
	_connect();
}

void OutputView::_connect()
{
	connect(clrScreen, SIGNAL(triggered()), this, SLOT(onClearScreen()));
	connect(selectAll, SIGNAL(triggered()), this, SLOT(onSelectAll()));
	connect(copy, SIGNAL(triggered()), this, SLOT(onCopy()));
}

void OutputView::onClearScreen()
{
	clearScreen();
}

void OutputView::onSelectAll()
{
	screen->selectAll();
	screen->setFocus();
}


void OutputView::onCopy()
{
	screen->copy();
	screen->setFocus();
}

void OutputView::writeToScreen(const QString & text)
{
	// newline after every entry by default with append
	screen->append(text);
}


void OutputView::clearScreen()
{
	screen->clear();
}

void OutputView::debugCallback(const nsstring & pMessage, void * pThis)
{
	//OutputView * th = (OutputView*)pThis;
	//th->writeToScreen(pMessage.c_str());
}
