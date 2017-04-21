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

#ifndef QNECONNECTION_H
#define QNECONNECTION_H

#include <QGraphicsPathItem>
#include <QtXml/QDomDocument>

class QNEPort;
class MessageFactory;

class QNEConnection : public QGraphicsPathItem {

public:
	enum { Type = QGraphicsItem::UserType + 2 };

    QNEConnection (QGraphicsItem *parent = 0);
	~QNEConnection();
	void setPos1(const QPointF &p);
	void setPos2(const QPointF &p);
	void setPort1(QNEPort *p);
	void setPort2(QNEPort *p);
	void updatePosFromPorts();
	void updatePath();
	QNEPort* port1() const;
	QNEPort* port2() const;
	void save(QDataStream&);
	void load(QDataStream&, const QMap<quint64, QNEPort*> &portMap);
    bool canDrop;
	int type() const { return Type; }
	void serialise (QDomDocument& doc);
	void inflate (const QDomDocument& doc);

private:

	QPointF pos1;
	QPointF pos2;
	QNEPort *m_port1;
	QNEPort *m_port2;


};

#endif // QNECONNECTION_H
