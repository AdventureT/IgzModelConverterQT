#pragma once
#include <stdio.h>
#include "umHalf.h"
#include <string>
#include <vector>
enum Endian
{
	LITTLE,
	BIG
};

static enum Endian e;

char ReadByte(FILE* f);

unsigned char ReadUByte(FILE* f);

std::vector<uint8_t> ReadBytes(FILE* f, int amount);

short ReadShort(FILE* f);

uint16_t ReadUShort(FILE* f);

half ReadHalfFloat(FILE* f);

float ReadFloat(FILE* f);

long ReadLong(FILE* f);

unsigned long ReadULong(FILE* f);

uint32_t ReadUInt(FILE* f);

int32_t ReadInt(FILE* f);

long long ReadLongLong(FILE* f);

unsigned long long ReadULongLong(FILE* f);

std::string ReadString(FILE* f);
std::string ReadString(FILE* f, int chars);
std::wstring ReadUnicodeString(FILE* f);
std::wstring ReadUnicodeString(FILE* f, int chars);
