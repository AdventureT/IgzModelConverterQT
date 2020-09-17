#ifndef IGZ_H
#define IGZ_H

#include <QWidget>
#include <QMessageBox>
#include "Reader.h"
#include "Section.h"
#include "Info.h"
#include "IGZType.h"
#include "IGZPlatform.h"
#include "VertexInfo.h"
#include <fbxsdk.h>
#include <Qt3DRender/QMesh>
#include <Qt3DRender/QPointLight>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QPhongMaterial>


class IGZ
{
private:
    QWidget* obj;
    FILE* f;
    std::string filePath;
    std::vector<Section> sections;
    std::vector<Info> infos;
    IGZType type;
    IGZPlatform plat;
    std::vector<uint64_t> subSectDataOffsets;
public:
    IGZ(QWidget* ui, std::string fP);
protected:
    void readHeader();
    void readInfos();
    void readModel();
    void readStaticModel();
    void readActor();
};

#endif // IGZ_H
