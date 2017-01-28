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

#include "qneport.h"
#include "qneblock.h"
#include <QGraphicsScene>
#include <QFontMetrics>
#include "qneconnection.h"
#include "qnemainwindow.h"
#include "../preferences.h"
#include "../qmodule/QModule.h"

QNEPort::QNEPort (int type, int index, const QString& name, QGraphicsItem* parent) :
        QGraphicsPathItem (parent),
        _radius (QNEMainWindow::preferences -> calenhad_port_radius),
        _margin (QNEMainWindow::preferences -> calenhad_port_margin),
        _index (index),
        _portType (type),
        _name (name) {

    QPainterPath p;
    QPolygonF polygon;


    if (type == OutputPort) {
        polygon << QPointF (-_radius, -_radius) << QPointF (_radius, 0) << QPointF (-_radius, _radius) << QPointF (-_radius, -_radius);
        setPen (QPen (QNEMainWindow::preferences -> calenhad_port_out_border_color, QNEMainWindow::preferences -> calenhad_port_border_weight));
        setBrush (QNEMainWindow::preferences -> calenhad_port_out_fill_color);
        setCursor (Qt::ArrowCursor);
    }
    if (type == InputPort) {
        polygon << QPointF (-_radius, -_radius) << QPointF (_radius, 0) << QPointF (-_radius, _radius) << QPointF (-_radius, -_radius);
        setPen (QPen (QNEMainWindow::preferences -> calenhad_port_in_border_color, QNEMainWindow::preferences -> calenhad_port_border_weight));
        setBrush (QNEMainWindow::preferences -> calenhad_port_in_fill_color);
    }
    if (type == ControlPort) {
        polygon = QRectF (-_radius, -_radius, _radius * 2, _radius * 2);
        setPen (QPen (QNEMainWindow::preferences -> calenhad_port_control_border_color, QNEMainWindow::preferences -> calenhad_port_border_weight));
        setBrush (QNEMainWindow::preferences -> calenhad_port_control_fill_color);
    }
    p.addPolygon (polygon);
    setPath (p);
    setFlag (QGraphicsItem::ItemSendsScenePositionChanges);
}

QNEPort::~QNEPort () {
    foreach (QNEConnection* conn, m_connections) delete conn;
}

void QNEPort::setBlock (QNEBlock* b) {
    _block = b;
}

void QNEPort::setName (const QString& n) {
    if (! (_label)) {
        _label = new QGraphicsTextItem (this);
    }
    _name = n;
    _label -> setPlainText (n);
    _label -> setDefaultTextColor (QNEMainWindow::preferences -> calenhad_port_text_color);
}


void QNEPort::setPortType (const unsigned& o) {
    _portType = o;
    if (scene ()) {
        initialise ();
    }
}

int QNEPort::radius () {
    return _radius;
}

QVector<QNEConnection*>& QNEPort::connections () {
    return m_connections;
}


QNEBlock* QNEPort::block () const {
    return _block;
}

quint64 QNEPort::ptr () {
    return _ptr;
}

void QNEPort::setPtr (quint64 p) {
    _ptr = p;
}

bool QNEPort::isConnected (QNEPort* other) {
            foreach (QNEConnection* conn, m_connections) {
            if (conn -> port1 () == other || conn -> port2 () == other) {
                return true;
            }
        }
    return false;
}

QVariant QNEPort::itemChange (GraphicsItemChange change, const QVariant& value) {
    if (change == ItemScenePositionHasChanged) {
                foreach (QNEConnection* conn, m_connections) {
                conn -> updatePosFromPorts ();
                conn -> updatePath ();
            }
    }
    return value;
}

void QNEPort::initialise () {
    QGraphicsScene* s = scene();
    if (s) {
        QFontMetrics fm (scene ()->font ());
        QRect r = fm.boundingRect (_name);
        if (!(_label)) {
            _label = new QGraphicsTextItem (this);
        }

        _label->setPlainText (_name);

        _label->setDefaultTextColor (QNEMainWindow::preferences->calenhad_port_text_color);
        if (_portType == OutputPort) {
            _label->setPos (_radius, -2 * (_radius + 1));
        } else {
            _label->setPos (-(_label->boundingRect ().width () + 4), -2 * (_radius + 1));
        }
    }
}

bool QNEPort::hasConnection() {
    return (! (m_connections.isEmpty()));
}

QRectF QNEPort::boundingRect() const {
    QRectF r = QRectF (-(_radius + 5), -(_radius + 5),  2 * (_radius + 5), 2 * (_radius + 5));
    return  r;
}

int QNEPort::index() {
    return _index;
}

Module* QNEPort::module()  {
    ComponentProxyWidget* cpw = ((ComponentProxyWidget*) _block -> parentItem());
    return ((QModule*) cpw -> widget()) -> module();
}

void QNEPort::invalidateRenders() {
    ComponentProxyWidget* cpw = ((ComponentProxyWidget*) _block -> parentItem());
    ((QModule*) cpw -> widget()) -> invalidate();
}