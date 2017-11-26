//
// Created by martin on 05/06/17.
//

#ifndef CALENHAD_CALENHADSTATSTAB_H
#define CALENHAD_CALENHADSTATSTAB_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <qwt/qwt_plot.h>
#include <QtWidgets/QComboBox>
#include "HypsographyWidget.h"


namespace calenhad {
    namespace qmodule {
        class QModule;
    }
    namespace controls {
        namespace globe {

            class CalenhadGlobeDialog;

            class CalenhadStatsPanel : public QWidget {
            public:
                CalenhadStatsPanel (calenhad::qmodule::QModule* source, CalenhadGlobeDialog* parent = 0);

                virtual ~CalenhadStatsPanel ();

                void showEvent (QShowEvent* e) override;

            protected:
                CalenhadGlobeDialog* dialog;
                QLabel* _mapExtremesLabel, * _worldExtremesLabel;
                QLabel* _worldMeanLabel;
                calenhad::controls::globe::HypsographyWidget* _worldHypsography;
                calenhad::qmodule::QModule* _source;
            };
        }
    }
}


#endif //CALENHAD_CALENHADSTATSTAB_H