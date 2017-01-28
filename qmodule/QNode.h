//
// Created by martin on 16/12/16.
//

#ifndef CALENHAD_QNODE_H
#define CALENHAD_QNODE_H


#include <QWidget>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QTextEdit>
#include "../nodeedit/qneport.h"
#include "../controls/QLogSpinBox.h"

class CalenhadModel;


class QNode : public QWidget {
Q_OBJECT


public:
    enum { Type = QGraphicsItem::UserType + 6 };
    virtual ~QNode();
    virtual void initialise();
    void setModel (CalenhadModel* model);
    // don't want a copy constructor because subclass implementations will have to call initialise()
    virtual QNode* addCopy (CalenhadModel* model) = 0;

    QString name();

    void setName (const QString& name);
    void setNotes (const QString& notes);
    QString notes();
    void addPort (QNEPort* port);
    QList<QNEPort*> ports();
    bool isInitialised();

    Q_PROPERTY (QString name READ name WRITE setName MEMBER _name);
    Q_PROPERTY (QString notes READ notes WRITE setNotes MEMBER _notes);
    virtual QString typeString() = 0;

    bool isRenderable();
    bool isComplete();

signals:
    void nodeChanged (const char* property, QVariant value);

protected:
    QNode (QWidget* widget);
    CalenhadModel* _model;
    QString _name;
    QString _notes;
    QLineEdit* _nameEdit;
    QTextEdit* _notesEdit;
    QToolBox* _expander;
    QList<QNEPort*> _ports;
    QWidget* _content;
    virtual void addInputPorts() = 0;
    int addPanel (const QString& name, QWidget* widget);
    QDoubleSpinBox* noiseValueParamControl (const QString& text);
    QSpinBox* countParameterControl (const QString& text);
    QDoubleSpinBox* angleParameterControl (const QString& text);
    QLogSpinBox* logParameterControl (const QString& text);
    QFormLayout* _contentLayout;
    bool _isInitialised;


};


#endif //CALENHAD_QNODE_H
