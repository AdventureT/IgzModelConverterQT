#ifndef VERTEXINFO_H
#define VERTEXINFO_H

#include <cinttypes>

struct VertexInfo
{
    uint8_t VertFormat1;
    uint8_t VertFormat2;
    uint8_t VertFormat3;
    uint8_t VertFormat4;
    uint8_t VertType;
    uint8_t VertLayer;
    uint8_t VertUnk1;
    uint8_t VertUnk2;
    uint32_t VertShift;
};

#endif // VERTEXINFO_H
