/* Copyright (c) 2012, STANISLAW ADASZEWSKI
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of STANISLAW ADASZEWSKI nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL STANISLAW ADASZEWSKI BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#include "qnemainwindow.h"
#include "CalenhadController.h"

#include "../preferences.h"
#include "../messagefactory.h"
#include "../pipeline/ModuleFactory.h"
#include "../pipeline/CalenhadModel.h"
#include "CalenhadView.h"
#include "../nodeedit/qnetoolbox.h"



Preferences* QNEMainWindow::preferences;
MessageFactory* QNEMainWindow::messages = new MessageFactory();
QNEToolBox* QNEMainWindow::toolbox = new QNEToolBox();

QNEMainWindow::QNEMainWindow (QWidget* parent) :
        QMainWindow (parent) {

    messages -> setHost (this);
    _controller = new CalenhadController (this);

    // Nodes editor

    _view = new CalenhadView (this);
    _view -> setRenderHint (QPainter::Antialiasing, true);
    _view -> centerOn (0, 0);

    setCentralWidget (_view);


    // Tools

    QToolBar* modulesToolbar = QNEMainWindow::toolbox -> toolbar ("Modules");
    QDockWidget* editToolsDock = new QDockWidget (modulesToolbar -> windowTitle(), this);
    editToolsDock -> setAllowedAreas (Qt::AllDockWidgetAreas);
    modulesToolbar -> setParent (this);
    editToolsDock -> setWidget (modulesToolbar);
    addDockWidget (Qt::RightDockWidgetArea, editToolsDock);

    QToolBar* zoomToolbar = QNEMainWindow::toolbox -> toolbar ("View");
    QDockWidget* zoomToolsDock = new QDockWidget (zoomToolbar -> windowTitle(), this);
    zoomToolsDock -> setAllowedAreas (Qt::AllDockWidgetAreas);
    zoomToolbar -> setParent (this);
    zoomToolsDock -> setWidget (zoomToolbar);
    addDockWidget (Qt::RightDockWidgetArea, zoomToolsDock);


    // Actions

    QAction* quitAction = new QAction (tr ("&Quit"), this);
    quitAction -> setShortcuts (QKeySequence::Quit);
    quitAction -> setStatusTip (tr ("Quit the application"));
    connect (quitAction, SIGNAL (triggered()), qApp, SLOT (quit()));

    QAction* loadAction = new QAction (tr ("&Load"), this);
    loadAction -> setShortcuts (QKeySequence::Open);
    loadAction -> setStatusTip (tr ("Open a file"));
    connect (loadAction, SIGNAL (triggered()), this, SLOT (loadFile()));

    QAction* saveAction = new QAction (tr ("&Save"), this);
    saveAction -> setShortcuts (QKeySequence::Save);
    saveAction -> setStatusTip (tr ("Save a file"));
    connect (saveAction, SIGNAL (triggered()), this, SLOT (saveFile()));

    // Menu

    QMenu* fileMenu = menuBar () -> addMenu (tr ("&File"));
    fileMenu -> addAction (loadAction);
    fileMenu -> addAction (saveAction);
    fileMenu -> addSeparator();
    fileMenu -> addAction (quitAction);
    QMenu* toolMenu = toolbox -> menu ("Modules");
    menuBar() -> addMenu (toolMenu);
    QMenu* viewMenu = toolbox -> menu ("View");
    menuBar() -> addMenu (viewMenu);

    setWindowTitle (tr ("Node Editor"));

    // Settings

    QSettings* settings = preferences -> settings();
    settings -> beginGroup ("MainWindow");
    resize (settings -> value("size", QSize(400, 400)).toSize());
    move (settings -> value("pos", QPoint(200, 200)).toPoint());
    settings -> endGroup();
}

QNEMainWindow::~QNEMainWindow() {
    delete messages;
}

void QNEMainWindow::setModel (CalenhadModel* model) {
    _model = model;
    _model -> setSceneRect (-1000, -1000, 1000, 1000);
    _controller->setModel (_model);
    _model -> setController (_controller);
    _view -> setController (_controller);
    _controller -> addView (_view);
    _view -> setScene (_model);

}

CalenhadModel* QNEMainWindow::model() {
    return _model;
}

void QNEMainWindow::saveFile() {
    QString fname = QFileDialog::getSaveFileName ();
    if (fname . isEmpty()) {
        return;
    }
    QFile f (fname);
    f.open (QFile::WriteOnly);
    QDataStream ds (&f);
    _model -> save (ds);
}

void QNEMainWindow::loadFile() {
    QString fname = QFileDialog::getOpenFileName ();
    if (fname . isEmpty()) {
        return;
    }
    QFile f (fname);
    f.open (QFile::ReadOnly);
    QDataStream ds (&f);
    _model -> load (ds);
}

void QNEMainWindow::closeEvent (QCloseEvent* event) {
    preferences -> saveSettings();
    QSettings* settings = preferences -> settings();
    settings -> beginGroup ("MainWindow");
    settings -> setValue ("size", size());
    settings -> setValue ("pos", pos());
    settings -> endGroup();
    event -> accept();
}
