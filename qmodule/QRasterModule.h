//
// Created by martin on 03/11/17.
//

#ifndef CALENHAD_QRASTERMODULE_H
#define CALENHAD_QRASTERMODULE_H

#include "QModule.h"

namespace calenhad {
    namespace qmodule {
        class QRasterModule : public QModule {
        public:
            QRasterModule (QModule* parent = 0);
            virtual ~QRasterModule();
            void setRaster (const QImage& raster);
            QImage* raster();
            void initialise() override;
            bool isComplete() override;
        protected:
            QString _filename;
            QImage* _raster;
            QLabel* _filenameLabel;
        protected slots:
            void fileDialogRequested();

            void openFile (const QString& filename);



        };
    }
}



#endif //CALENHAD_QRASTERMODULE_H
