//
// Created by martin on 10/05/17.
//

#ifndef CALENHAD_CALENHADGLOBECONTEXTMENU_H
#define CALENHAD_CALENHADGLOBECONTEXTMENU_H


#include <QtWidgets/QMenu>
#include <marble/MarbleGlobal.h>
#include "CalenhadGlobeConstants.h"
namespace calenhad {
    namespace mapping {
        namespace projection {
            class Projection;
        }
    }
}

using namespace calenhad::mapping::projection;
namespace calenhad {
    namespace controls {
        namespace globe {

            class CalenhadGlobeDialog;

            class CalenhadGlobeContextMenu : public QMenu {
            Q_OBJECT
            public:
                CalenhadGlobeContextMenu (CalenhadGlobeDialog* parent);

                virtual ~CalenhadGlobeContextMenu ();

            public slots:

                void setDragMode (const bool& enable);
                void selectQuality();

            signals:

                void showOverviewMap (const bool&);

                void showZoomSlider (const bool&);

                void showNavigator (const bool&);

                void showGraticule (const bool&);

                void dragModeSelected (const CalenhadGlobeDragMode&);

                void doubleClickModeSelected (const CalenhadGlobeDoubleClickMode&);

                void projectionChangeRequested (const QString& projection);

                void scaleVisibleSelected (const bool&);

            protected:
                CalenhadGlobeDialog* _parent;
                QMenu* _mouseDragMenu;
                QAction* _panAction;
                QAction* _zoomAction;
                QAction* _disableDragAction;
                QMenu* _mouseDoubleClickMenu;
                QAction* _gotoAction;
                QAction* _placeAction;
                QAction* _disableDoubleClickAction;
                QAction* _showOverviewMapAction;
                QAction* _showScaleAction;
                QAction* _showZoomSliderAction;
                QAction* _showNavigatorAction;
                QAction* _showGraticuleAction;
                QMenu* _projectionMenu;
                QActionGroup* _projectionActions;
                QMenu* _captureMenu;
                QMenu* _qualityMenu;
                QAction* _qualityBestAction, * _qualityDecentAction, * _qualityDraftAction, * _qualityShiteAction;

            protected slots:

                void initialise ();

                void setDoubleClickMode (const bool& enable);

                void projectionSelected (const bool& selected);

                void setScalebarVisible (const bool& visible);


            };
        }
    }
}

#endif //CALENHAD_CALENHADGLOBECONTEXTMENU_H
