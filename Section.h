#ifndef SECTION_H
#define SECTION_H

#include <cinttypes>

struct Section
{
    uint32_t dataCount;
    uint32_t unknown;
    uint32_t sectionOffset;
    uint32_t sectionLength;
};

#endif // SECTION_H
