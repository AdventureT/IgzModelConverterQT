#ifndef INFO_H
#define INFO_H

#include <cinttypes>

struct Info
{
    char name[4];
    uint32_t count;
    uint32_t size;
    uint32_t offset;
};

#endif // INFO_H
