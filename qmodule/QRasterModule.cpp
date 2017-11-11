//
// Created by martin on 03/11/17.
//

#include <QtWidgets/QAction>
#include <QtWidgets/QFileDialog>
#include "QRasterModule.h"
#include "../CalenhadServices.h"
#include "../preferences/PreferencesService.h"
#include "../controls/globe/CalenhadMapView.h"

using namespace calenhad::qmodule;
using namespace calenhad;
using namespace calenhad::preferences;

QRasterModule::QRasterModule (QModule* parent) : QModule (CalenhadServices::preferences() -> calenhad_module_raster, 0, parent),
    _raster (nullptr),
    _filename (QString::null) {
    initialise();
}

QRasterModule::~QRasterModule() {
    if (_raster) { delete _raster; }
}

/// Initialise a QRasterModule ready for use. Creates the UI.
void QRasterModule::initialise() {
    QModule::initialise();
    // create a panel to hold the parameter widgets, if we haven't done this already
    if (! (_content)) {
        addContentPanel();
    }

    QVBoxLayout* filenameLayout = new QVBoxLayout();
    _filenameLabel = new QLabel (this);
    _filenameLabel -> setMinimumSize (QSize (160, 80));
    QPushButton* selectFileButton = new QPushButton (this);
    selectFileButton -> setFixedSize (100, 20);
    selectFileButton -> setText ("Select image...");
    _contentLayout -> addWidget (_filenameLabel);
    _contentLayout -> addWidget (selectFileButton);
    connect (selectFileButton, &QAbstractButton::pressed, this, &QRasterModule::fileDialogRequested);
}

void QRasterModule::setRaster (const QImage& raster) {
    if (! raster.isNull()) {
        QImage p;
        int h, w;
        if (raster.height() > raster.width() / 2) {
            h = raster.width() / 2;
            p = raster.scaledToHeight (h);
        } else {
            w = raster.height() * 2;
            p = raster.scaledToWidth (w);
        }
        if (_raster) {
            delete _raster;
        }
        p = p.scaled (CalenhadServices::preferences() -> calenhad_globe_texture_height * 2, CalenhadServices::preferences() -> calenhad_globe_texture_height);
        _raster = new QImage (p);
        QPixmap pixmap = QPixmap::fromImage (p).scaled (_filenameLabel -> size());
        _filenameLabel -> setPixmap (pixmap);
        _expander -> setItemEnabled (_previewIndex, isComplete());
        _preview -> render();
    } else {

    }
}

QImage* QRasterModule::raster() {
    return _raster;
}

void QRasterModule::fileDialogRequested () {
    QString dir;
    if (_filename != QString::null) {
        QDir f (_filename);
        dir = f.dirName ();
    }
    QString filename = QFileDialog::getOpenFileName (this, "Select raster", "dir", "Image Files (*.png *.jpg *.bmp)");
    QPixmap pixmap (filename);
    openFile (filename);
}

void QRasterModule::openFile (const QString& filename) {
    QImage raster = QImage (filename);
    setRaster (raster);
}

bool QRasterModule::isComplete() {

    if (_raster) {
        return QModule::isComplete();
    } else {
        return false;
    }
}
