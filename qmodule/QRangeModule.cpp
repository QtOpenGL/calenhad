//
// Created by martin on 19/12/16.
//

#include "QClampModule.h"
#include "QRangeModule.h"
#include "../pipeline/CalenhadModel.h"


QRangeModule::QRangeModule (Module* module, QWidget* parent) : QModule (module, parent){

}

void QRangeModule::initialise() {
    QModule::initialise();
    lowerBoundSpin = QNode::noiseValueParamControl ("Lower bound");
    _contentLayout -> addRow (tr ("Lower bound"), lowerBoundSpin);
    connect (lowerBoundSpin, SIGNAL (valueChanged (double)), this, SLOT (setLowerBound (double)));

    upperBoundSpin = QNode::noiseValueParamControl ("Upper bound");
    _contentLayout -> addRow (tr ("Upper bound"), upperBoundSpin);
    connect (upperBoundSpin, SIGNAL (valueChanged (double)), this, SLOT (setUpperBound (double)));


}

void QRangeModule::setLowerBound (double lowerBound) {
    if (lowerBound < upperBound()) {
        lowerBoundSpin -> setValue (lowerBound);
        setBounds (lowerBound, upperBound());
        upperBoundSpin -> setMinimum (lowerBound + lowerBoundSpin -> singleStep());
        emit QNode::nodeChanged ("lowerBound", lowerBound);
    }

}

void QRangeModule::setUpperBound (double upperBound) {
    if (upperBound > lowerBound()) {
        upperBoundSpin -> setValue (upperBound);
        setBounds (lowerBound(), upperBound);
        lowerBoundSpin -> setMaximum (upperBound - upperBoundSpin -> singleStep());
        emit QNode::nodeChanged ("upperBound", upperBound);
    }
}


void QRangeModule::inflate (const QDomElement& element, MessageFactory* messages) {
    QModule::inflate (element, messages);
    bool ok;

    double x = _model -> readParameter (element, "lowerBound").toDouble (&ok);
    if (ok) { setLowerBound (x); }

    double y = _model -> readParameter (element, "upperBound").toDouble (&ok);
    if (ok) { setUpperBound (y); }

}

void QRangeModule::serialise (QDomDocument& doc, MessageFactory* messages) {
    QModule::serialise (doc, messages);
    _model -> writeParameter (_element, "lowerBound", QString::number (lowerBound()));
    _model -> writeParameter (_element, "upperBound", QString::number (upperBound()));
}
