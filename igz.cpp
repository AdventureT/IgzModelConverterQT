#include "igz.h"
#include <cstdio>

IGZ::IGZ(QWidget* obj, std::string fP)
{
    this->obj = obj;
    this->filePath = fP;
    //QMessageBox::information(obj,"LOL", "Hello from the other side!");
    if (fopen_s(&f, fP.data(), "rb") != 0)
        QMessageBox::information(obj,"File Error", "File couldn't be opened");
    //setvbuf(f, NULL, _IONBF, 0);
    readHeader();
}

void IGZ::readHeader()
{
    std::string magic = ReadString(f,4);
    uint32_t version = ReadUInt(f);
    int hash = ReadInt(f);
    uint32_t unknown = ReadUInt(f);
    for (size_t i = 0; i < 10; i++)
    {
        Section section;
        fread_s(&section, sizeof(Section), sizeof(Section), 1, f);
        if (section.sectionOffset == 0)
            break;
        sections.push_back(section);
    }
    readInfos();
}

void IGZ::readInfos()
{
    fseek(f, sections[0].sectionOffset, SEEK_SET);
    for (size_t i = 0; i < sections[0].dataCount; i++)
    {
        Info info;
        fread_s(&info, sizeof(info), sizeof(info), 1, f);
        if (strncmp(info.name, "ROFS", 4) == 0) //Consistent with models but everything else not really
        {
            int32_t id = ReadInt(f);
            switch (id) {
            case 1276912152:
                type = IGZType::STATICMODEL;
                break;
            case 404496920:
                type = IGZType::ACTOR;
                break;
            case 31629848:
                type = IGZType::TEXTURE;
                break;
            default:
                QMessageBox::information(obj,"Error", "Sorry this IGZ file is not supported yet!");
                return;
            }
            fseek(f, info.size-20,SEEK_CUR);
        }
        else
        {
            fseek(f, info.size-16,SEEK_CUR);
        }
        infos.push_back(info);
    }
    readModel();
}

void IGZ::readModel()
{
    uint32_t unknown1 = ReadUInt(f);
    uint32_t unknown2 = ReadUInt(f);
    uint32_t unknown3 = ReadUInt(f);
    uint32_t unknown4 = ReadUInt(f);
    uint32_t subSectCount = ReadUInt(f);
    plat = static_cast<IGZPlatform>(ReadUInt(f)); //2 PC
    uint32_t subSectSize = ReadUInt(f);
    uint32_t offsetMarker = ReadInt(f); //?
    uint64_t subSectOffset = ReadULongLong(f);
    fseek(f, subSectOffset + sections[1].sectionOffset, SEEK_SET);

    for (size_t i = 0; i < subSectCount; i++)
    {
        subSectDataOffsets.push_back(ReadULongLong(f));
    }
    if (type == IGZType::STATICMODEL)
    {
        readStaticModel();
    }
    else
    {
        readActor();
    }

}

void IGZ::readActor()
{
    fseek(f, 40, SEEK_CUR);
    uint64_t boneInfoOffset = ReadULongLong(f);
    uint64_t modelInfoOffset = ReadULongLong(f);
    uint64_t vertexInfoOffset = ReadULongLong(f);
    fseek(f, vertexInfoOffset, SEEK_SET);
}

void IGZ::readStaticModel()
{
    fseek(f, sections[1].sectionOffset + 0xE0, SEEK_SET);
    uint32_t modelCount = ReadUInt(f);
    fseek(f, 4, SEEK_CUR);
    uint32_t modelOffsetBuffSize = ReadUInt(f);
    uint32_t offsetMarker = ReadInt(f); //?
    uint64_t modelOffset = ReadULongLong(f) + sections[1].sectionOffset;
    uint64_t modelCount2 = ReadUInt(f);
    fseek(f, 4, SEEK_CUR);
    uint32_t unknownCount = ReadUInt(f);
    uint32_t unknownIdentifier = ReadUInt(f);
    uint64_t ModelOffset2 = ReadULongLong(f);

    fseek(f, subSectDataOffsets[subSectDataOffsets.size()-1] + 0x50 + sections[1].sectionOffset, SEEK_SET);
    uint32_t modelJump = ReadUInt(f);
    uint32_t offsetIdentifier = ReadUInt(f);
    uint64_t modelJumpOffset = ReadUInt(f) + sections[1].sectionOffset;
    fseek(f, modelJumpOffset + modelJump + 0x18, SEEK_SET);
    uint32_t VertPlus = ReadUInt(f);
    uint32_t Unknown = ReadUInt(f);
    uint32_t VertStart = ReadUInt(f) + VertPlus + sections[1].sectionOffset;
    fseek(f, sections[1].sectionOffset + 0x140, SEEK_SET);
    FbxManager* lSdkManager = FbxManager::Create();
    FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");
    FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);
    std::vector<uint32_t> verticesCount;
    std::vector<uint32_t> faceIndicesCount;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> uvs;
    std::vector<uint32_t> faces;
    for (size_t a = 0; a < modelCount; a++)
    {
        uint64_t modelGroupOffset = ReadULongLong(f) + sections[1].sectionOffset;
        long returnHere = ftell(f);
        fseek(f, modelGroupOffset, SEEK_SET);
        Unknown = ReadUInt(f);
        uint32_t Blank = ReadUInt(f);
        Unknown = ReadUInt(f);
        Blank = ReadUInt(f);
        Unknown = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        long BoundMinX = ReadLong(f);
        long BoundMinY = ReadLong(f);
        long BoundMinZ = ReadLong(f);
        Blank = ReadUInt(f);
        long BoundMaxX = ReadLong(f);
        long BoundMaxY = ReadLong(f);
        long BoundMaxZ = ReadLong(f);
        Blank = ReadUInt(f);
        uint16_t MatID = ReadUShort(f) + 1;
        Unknown = ReadUShort(f);
        Blank = ReadLong(f);
        uint64_t VertInfoOffset = ReadULongLong(f) + sections[1].sectionOffset;
        uint64_t PolyInfoOffset = ReadULongLong(f) + sections[1].sectionOffset;
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        Unknown = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        Unknown = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);

        fseek(f, VertInfoOffset, SEEK_SET);
        Unknown = ReadUInt(f);
        Blank = ReadUInt(f);
        Unknown = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        Unknown = ReadUInt(f);
        Blank = ReadUInt(f);
        uint64_t InfoOffset1 = ReadULongLong(f) + sections[1].sectionOffset;
        uint64_t InfoOffset2 = ReadULongLong(f) + sections[1].sectionOffset;
        uint64_t InfoOffset3 = ReadULongLong(f) + sections[1].sectionOffset;

        fseek(f, InfoOffset1, SEEK_SET);
        Unknown = ReadUInt(f);
        Blank = ReadUInt(f);
        Unknown = ReadUInt(f);
        if (plat == 2) Blank = ReadUInt(f);
        uint32_t VertexCount = ReadUInt(f);
        verticesCount.push_back(VertexCount);
        if (plat == 2) Blank = ReadUInt(f);
        Unknown = ReadUInt(f);
        Unknown = ReadUInt(f);
        uint64_t InfoOffset1B = ReadULongLong(f) + sections[1].sectionOffset;
        uint32_t VertexBufferSize = ReadUInt(f);
        Unknown = ReadUInt(f);
        uint64_t VertexBufferStart = ReadULong(f) - 0x08000000 + VertStart;
        fseek(f, 4, SEEK_CUR);
        uint64_t InfoOffset1C = ReadULongLong(f) + sections[1].sectionOffset;
        Unknown = ReadUInt(f);
        Blank = ReadUInt(f);
        uint64_t InfoOffset1D = ReadULongLong(f) + sections[1].sectionOffset;
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        uint64_t InfoOffset1E = ReadULongLong(f) + sections[1].sectionOffset;

        fseek(f, InfoOffset1B, SEEK_SET);
        uint32_t VertexCount2 = ReadUInt(f);
        fseek(f, 4, SEEK_CUR);

        fseek(f, InfoOffset1C, SEEK_SET);
        Unknown = ReadUInt(f);
        Blank = ReadUInt(f);
        Unknown = ReadUInt(f);
        if (plat == 2) Blank = ReadUInt(f);
        uint32_t VertexBufferStride = ReadUInt(f);
        if (plat == 2) Blank = ReadUInt(f);
        uint32_t VertBuffParamASize = ReadUInt(f);
        Unknown = ReadUInt(f);
        uint64_t VertBuffParamAOffset = ReadULongLong(f) + sections[1].sectionOffset;
        uint32_t VertBuffParamBSize = ReadUInt(f);
        Unknown = ReadUInt(f);
        uint64_t VertBuffParamBOffset = ReadULongLong(f) + sections[1].sectionOffset;
        Unknown = ReadUInt(f);
        Unknown = ReadUInt(f);
        uint64_t InfoOffset1F = ReadULongLong(f) + sections[1].sectionOffset;
        uint64_t InfoOffset1G = ReadULongLong(f) + sections[1].sectionOffset;
        Unknown = ReadUInt(f);
        Blank = ReadUInt(f);
        Unknown = ReadUInt(f);
        Unknown = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        Unknown = ReadUInt(f);
        Blank = ReadUInt(f);
        uint64_t InfoOffset1H = ReadULongLong(f) + sections[1].sectionOffset;
        Unknown = ReadUInt(f);
        Unknown = ReadUInt(f);

        fseek(f, VertBuffParamAOffset, SEEK_SET);
        std::vector<VertexInfo> verticesInfo;
        for (int i = 0; i < (VertBuffParamASize / 0x0C); i++)
        {
            verticesInfo.push_back({ReadUByte(f), ReadUByte(f), ReadUByte(f), ReadUByte(f), ReadUByte(f), ReadUByte(f), ReadUByte(f), ReadUByte(f), ReadUInt(f)});
            verticesInfo[i].VertType += 1;
            verticesInfo[i].VertLayer += 1;
        }
        fseek(f, VertBuffParamBOffset, SEEK_SET);
        for (int i = 0; i < (VertBuffParamBSize / 0x04); i++)
        {
            uint32_t VertUnknown = ReadUInt(f);
        }

        fseek(f, PolyInfoOffset, SEEK_SET); //Pretty much the same like VertInfoOffset
        Unknown = ReadUInt(f);
        Blank = ReadUInt(f);
        Unknown = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        Unknown = ReadUInt(f);
        Blank = ReadUInt(f);
        InfoOffset1 = ReadULongLong(f) + sections[1].sectionOffset;
        InfoOffset2 = ReadULongLong(f) + sections[1].sectionOffset;
        InfoOffset3 = ReadULongLong(f) + sections[1].sectionOffset;

        fseek(f, InfoOffset1, SEEK_SET);
        Unknown = ReadUInt(f);
        Blank = ReadUInt(f);
        Unknown = ReadUInt(f);
        if (plat == 2) Blank = ReadUInt(f);
        uint32_t FacepointCount = ReadUInt(f);
        faceIndicesCount.push_back(FacepointCount);
        if (plat == 2) Blank = ReadUInt(f);
        Unknown = ReadUInt(f);
        Unknown = ReadUInt(f);
        InfoOffset1B = ReadULongLong(f) + sections[1].sectionOffset;
        uint32_t PolyBufferSize = ReadUInt(f);
        Unknown = ReadUInt(f);
        uint32_t PolyBufferStart = ReadUInt(f) - 0x08000000 + VertStart;
        Blank = ReadUInt(f);
        InfoOffset1C = ReadULongLong(f) + sections[1].sectionOffset;
        Unknown = ReadUInt(f);
        Blank = ReadUInt(f);
        InfoOffset1D = ReadULongLong(f) + sections[1].sectionOffset;
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        InfoOffset1E = ReadULongLong(f) + sections[1].sectionOffset;

        fseek(f, InfoOffset1B, SEEK_SET);
        uint64_t FacepointCount2 = ReadULongLong(f);

        fseek(f, InfoOffset1D, SEEK_SET);
        Unknown = ReadUInt(f);
        Blank = ReadUInt(f);
        Unknown = ReadUInt(f);
        if (plat == 2) Blank = ReadUInt(f);
        uint32_t faceBufferStride = ReadUInt(f); //Again?
        if (plat == 2) Blank = ReadUInt(f);
        uint32_t PolyBuffParamASize = ReadUInt(f);
        Unknown = ReadUInt(f);
        uint64_t PolyBuffParamAOffset = ReadULongLong(f) + sections[1].sectionOffset;
        uint32_t PolyBuffParamBSize = ReadUInt(f);
        Unknown = ReadUInt(f);
        uint64_t PolyBuffParamBOffset = ReadULongLong(f) + sections[1].sectionOffset;
        Unknown = ReadUInt(f);
        Unknown = ReadUInt(f);
        InfoOffset1F = ReadULongLong(f) + sections[1].sectionOffset;
        InfoOffset1G = ReadULongLong(f) + sections[1].sectionOffset;
        Unknown = ReadUInt(f);
        Blank = ReadUInt(f);
        Unknown = ReadUInt(f);
        Unknown = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        Blank = ReadUInt(f);
        Unknown = ReadUInt(f);
        Blank = ReadUInt(f);
        InfoOffset1H = ReadULongLong(f) + sections[1].sectionOffset;
        Unknown = ReadUInt(f);
        Unknown = ReadUInt(f);
        float tanX;
        float tanY;
        float tanZ;
        uint8_t colorR;
        uint8_t colorG;
        uint8_t colorB;
        float colorA;
        float uvU; float uvV; float uvW;
        half HuvU; float HuvV;
        uint8_t Weight1;
        uint8_t Weight2;
        uint8_t Weight3;
        uint8_t Weight4;
        long index;
        long index1;
        long index2;
        long index3;
        long index4;
        long index5;
        long index6;
        long index7;
        fseek(f, VertexBufferStart, SEEK_SET);

        for (size_t i = 0; i < VertexCount; i++)
        {
            for (size_t j = 0; j < verticesInfo.size(); j++)
            {
                uint8_t curVertType = verticesInfo[j].VertType;
                uint8_t curVertFmt = verticesInfo[j].VertFormat1;
                uint8_t curVertLyr = verticesInfo[j].VertLayer;


                switch (curVertType)
                {
                case 1: //Vertices
                    switch (curVertFmt)
                    {
                    case 2:
                        vertices.push_back(ReadFloat(f));
                        vertices.push_back(ReadFloat(f));
                        vertices.push_back(ReadFloat(f));
                        break;
                    default:
                        break;
                    }
                    break;
                case 2: //Normals
                    switch (curVertFmt)
                    {
                    case 2:
                        normals.push_back(ReadFloat(f));
                        normals.push_back(ReadFloat(f));
                        normals.push_back(ReadFloat(f));
                        //lLayerElementNormal->GetDirectArray().Add(FbxVector4(ReadFloat(f), ReadFloat(f), ReadFloat(f)));
                        break;
                    default:
                        break;
                    }
                    break;
                case 3: //Tangents
                    switch (curVertFmt)
                    {
                    case 2:
                        fread_s(&tanX, 4, 4, 1, f);
                        fread_s(&tanY, 4, 4, 1, f);
                        fread_s(&tanZ, 4, 4, 1, f);
                        break;
                    default:
                        break;
                    }
                    break;
                case 5: //Colors
                    switch (curVertFmt)
                    {
                    case 4:
                        fread_s(&colorR, 1, 1, 1, f);
                        fread_s(&colorG, 1, 1, 1, f);
                        fread_s(&colorB, 1, 1, 1, f);
                        fread_s(&colorA, 1, 1, 1, f);
                        colorA /= 255;
                        break;
                    default:
                        break;
                    }
                    break;
                case 6: //UVS
                    switch (curVertFmt)
                    {
                    case 2:
                        uvU = ReadFloat(f);
                        uvV = ReadFloat(f);
                        uvs.push_back(uvU);
                        uvs.push_back(1-uvV);
                        ReadFloat(f); //uvW
                        //lLayerElementUV->GetDirectArray().Add(FbxVector2(ReadFloat(f), -ReadFloat(f)));
                        break;
                    case 42:
                        uvU = ReadHalfFloat(f);
                        uvV = ReadHalfFloat(f);
                        uvs.push_back(uvU);
                        uvs.push_back(1-uvV);
                        //lLayerElementUV->GetDirectArray().Add(FbxVector2(ReadHalfFloat(f), -ReadHalfFloat(f)));
                        break;
                    default:
                        break;
                    }
                    break;
                case 7:
                    switch (curVertFmt)
                    {
                    case 26:
                        //Weights Stuff
                        fread_s(&Weight1, 1, 1, 1, f);
                        fread_s(&Weight2, 1, 1, 1, f);
                        fread_s(&Weight3, 1, 1, 1, f);
                        fread_s(&Weight4, 1, 1, 1, f);
                        //Weight1 = ((float)ReadUByte(f)) / 255;
                        //Weight2 = ((float)ReadUByte(f)) / 255;
                        //Weight3 = ((float)ReadUByte(f)) / 255;
                        //Weight4 = ((float)ReadUByte(f)) / 255;
                        break;
                    default:
                        break;
                    }
                    break;
                case 9:
                    switch (curVertFmt)
                    {
                    case 13:
                        //Bone Stuff
                        fread_s(&index, 4, 4, 1, f);
                        fread_s(&index1, 4, 4, 1, f);
                        fread_s(&index2, 4, 4, 1, f);
                        fread_s(&index3, 4, 4, 1, f);
                        break;
                    case 23:
                        //Bone Stuff
                        fread_s(&index4, 1, 1, 1, f);
                        fread_s(&index5, 1, 1, 1, f);
                        fread_s(&index6, 1, 1, 1, f);
                        fread_s(&index7, 1, 1, 1, f);
                        break;
                    default:
                        break;
                    }
                    break;
                default:
                    break;
                }

            }
        }

        fseek(f, PolyBufferStart, SEEK_SET);
        if (VertexCount <= 65535)
        {
            for (int i = 0; i < (FacepointCount / 3); i++)
            {
                faces.push_back(ReadUShort(f));
                faces.push_back(ReadUShort(f));
                faces.push_back(ReadUShort(f));
            }
        }
        else
        {
            for (int i = 0; i < (FacepointCount / 3); i++)
            {
                faces.push_back(ReadUInt(f));
                faces.push_back(ReadUInt(f));
                faces.push_back(ReadUInt(f));
            }
        }
        fseek(f, returnHere, SEEK_SET);
        verticesInfo.clear();
    }
    int sumVertices = 0;
    int Viord = 0;
    int index = 0;
    int rememberI = 0;
    int remeberU = 0;
    int Niord = 0;
    for (int i = 0; i < modelCount; i++)
    {
        char meshNode[8];
        snprintf(meshNode, sizeof meshNode, "Mesh_%d", i);
        char mesh[8];
        snprintf(mesh, sizeof mesh, "Mesh_%d", i);
        FbxNode* lMeshNode = FbxNode::Create(lScene, meshNode);
        FbxMesh* lMesh = FbxMesh::Create(lScene, mesh);
        lMeshNode->SetNodeAttribute(lMesh);
        FbxNode* lRootNode = lScene->GetRootNode();
        FbxNode* lPatchNode = lScene->GetRootNode();
        lRootNode->AddChild(lMeshNode);
        lMesh->InitControlPoints(verticesCount[i]);
        FbxVector4* lControlPoints = lMesh->GetControlPoints();
        FbxLayer* lLayer = lMesh->GetLayer(0);

        if (lLayer == NULL) {
            lMesh->CreateLayer();
            lLayer = lMesh->GetLayer(0);
        }
        FbxLayerElementNormal* lLayerElementNormal = FbxLayerElementNormal::Create(lMesh, "");
        FbxLayerElementUV* lLayerElementUV = FbxLayerElementUV::Create(lMesh, "");
        // Set its mapping mode to map each normal vector to each control point.
        lLayerElementNormal->SetMappingMode(FbxLayerElement::eByControlPoint);
        lLayerElementUV->SetMappingMode(FbxLayerElement::eByControlPoint);
        // Set the reference mode of so that the n'th element of the normal array maps to the n'th
        // element of the control point array.
        lLayerElementNormal->SetReferenceMode(FbxLayerElement::eDirect);
        lLayerElementUV->SetReferenceMode(FbxLayerElement::eDirect);
        for (int j = 0; j < verticesCount[i]; j++)
        {
            FbxVector4 vertex(vertices[Viord], vertices[Viord + 1], vertices[Viord + 2]);
            FbxVector4 normal(normals[Niord], normals[Niord + 1], normals[Niord + 2]);
            FbxVector2 uv(uvs[remeberU], uvs[remeberU + 1]);
            lLayerElementNormal->GetDirectArray().Add(normal);
            lLayerElementUV->GetDirectArray().Add(uv);
            lControlPoints[j] = vertex;
            Viord += 3;
            Niord += 3;
            remeberU += 2;

        }
        for (int y = 0; y < (faceIndicesCount[i] / 3); y++)
        {
            lMesh->BeginPolygon();
            lMesh->AddPolygon(faces[rememberI]);
            lMesh->AddPolygon(faces[rememberI + 1]);
            lMesh->AddPolygon(faces[rememberI + 2]);
            lMesh->EndPolygon();
            rememberI += 3;
        }
        lLayer->SetNormals(lLayerElementNormal);
        lLayer->SetUVs(lLayerElementUV);
        lMeshNode->LclRotation.Set(FbxVector4(-90.0, -90.0, 0.0)); //Right rotation

    }
    FbxExporter* lExporter = FbxExporter::Create(lSdkManager, "");
    bool lExportStatus = lExporter->Initialize(filePath.erase(filePath.length()-4).c_str(), -1, lSdkManager->GetIOSettings());
    if (!lExportStatus) {
        //
    }
    lExporter->Export(lScene);
    lExporter->Destroy();
}


