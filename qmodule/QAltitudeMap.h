//
// Created by martin on 31/03/17.
//

#ifndef CALENHAD_QALTITUDEMAP_H
#define CALENHAD_QALTITUDEMAP_H

#include <QtCore/QString>
#include <QtXml/QDomElement>
#include "QModule.h"

class AltitudeMapEditor;

class QAltitudeMap : public QModule {
    Q_OBJECT
public:
    static QAltitudeMap* newInstance();
    virtual ~QAltitudeMap();
    void initialise() override;

    QAltitudeMap* addCopy (CalenhadModel* model) override;
    virtual QString moduleType () override;
    QVector<QPointF> getEntries() const;
    virtual void inflate (const QDomElement& element, MessageFactory* messages) override;
    virtual void serialise (QDomDocument& doc, MessageFactory* messages) override;
    void addEntry (const double& in, const double& out = 0);
    void removeEntry (const double& key);

public slots:

    void updateEntries();
    void editingFinished();
    void clearMap();
    void editAltitudeMap();

protected:
    QAltitudeMap (QWidget* parent = 0);
    AltitudeMapEditor* _editor;


};


#endif //CALENHAD_QALTITUDEMAP_H
