#include "Reader.h"

char ReadByte(FILE* f)
{

	char read;
	fread_s(&read, 1, 1, 1, f);
	return read;

}

unsigned char ReadUByte(FILE* f)
{
	unsigned char read;
	fread_s(&read, 1, 1, 1, f);
	return read;
}

std::vector<uint8_t> ReadBytes(FILE* f, int amount)
{
	std::vector<uint8_t> read;
	for (int i = 0; i < amount; i++)
	{
		uint8_t c;
		fread_s(&c, 1, 1, 1, f);
		read.push_back(c);
	}
	return read;
}


short ReadShort(FILE* f)
{
	if (e == LITTLE)
	{
		short read;
		fread_s(&read, 2, 2, 1, f);
		return read;
	}
	else
	{
		unsigned char read[2];
		fread_s(read, 2, 2, 1, f);
		return static_cast<short>(
			(read[1]) |
			(read[0] << 8));
	}

}

uint16_t ReadUShort(FILE* f)
{
	if (e == LITTLE)
	{
		uint16_t read;
		fread_s(&read, 2, 2, 1, f);
		return read;
	}
	else
	{
		uint8_t read[2];
		fread_s(&read, 2, 2, 1, f);
		return static_cast<unsigned short>(
			(read[1]) |
			(read[0] << 8));
	}
}

half ReadHalfFloat(FILE* f)
{
	if (e == LITTLE)
	{
		half read;
		fread_s(&read, 2, 2, 1, f);
		return read;
	}
	else
	{
		uint8_t read[2];
		fread_s(read, 2, 2, 1, f);
		return static_cast<float>(
			(read[1]) |
			(read[0] << 8));
	}
	//half read;
	//fread_s(&read, 2, 2, 1, f);
	//return read;
}

float ReadFloat(FILE* f)
{
	if (e == LITTLE)
	{
		float read;
		fread_s(&read, 4, 4, 1, f);
		return read;
	}
	else
	{
		uint8_t read[4];
		fread_s(read, 4, 4, 1, f);
		return static_cast<float>(
			(read[3]) |
			(read[2] << 8) |
			(read[1] << 16) |
			(read[0] << 24));
	}
}

long ReadLong(FILE* f)
{
	if (e == LITTLE)
	{
		long read;
		fread_s(&read, 4, 4, 1, f);
		return read;
	}
	else
	{
		uint8_t read[4];
		fread_s(read, 4, 4, 1, f);
		return static_cast<long>(
			(read[3]) |
			(read[2] << 8) |
			(read[1] << 16) |
			(read[0] << 24));
	}
}

unsigned long ReadULong(FILE* f)
{
	if (e == LITTLE)
	{
		unsigned long read;
		fread_s(&read, 4, 4, 1, f);
		return read;
	}
	else
	{
		uint8_t read[4];
		fread_s(read, 4, 4, 1, f);
		return static_cast<unsigned long>(
			(read[3]) |
			(read[2] << 8) |
			(read[1] << 16) |
			(read[0] << 24));
	}
}

uint32_t ReadUInt(FILE* f)
{
	if (e == LITTLE)
	{
		uint32_t read;
		fread_s(&read, 4, 4, 1, f);
		return read;
	}
	else
	{
		uint8_t read[4];
		fread_s(read, 4, 4, 1, f);
		return static_cast<uint32_t>(
			(read[3]) |
			(read[2] << 8) |
			(read[1] << 16) |
			(read[0] << 24));
	}
}

int32_t ReadInt(FILE* f)
{
	if (e == LITTLE)
	{
		int32_t read;
		fread_s(&read, 4, 4, 1, f);
		return read;
	}
	else
	{
		uint8_t read[4];
		fread_s(read, 4, 4, 1, f);
		return static_cast<int32_t>(
			(read[3]) |
			(read[2] << 8) |
			(read[1] << 16) |
			(read[0] << 24));
	}
}

long long ReadLongLong(FILE* f)
{
	if (e == LITTLE)
	{
		long long read;
		fread_s(&read, 8, 8, 1, f);
		return read;
	}
	else
	{
		uint8_t read[8];
		fread_s(read, 8, 8, 1, f);
		return static_cast<long long>(
			(read[7]) |
			(read[6] << 8) |
			(read[5] << 16) |
			(read[4] << 24) |
			(read[3] << 32) |
			(read[2] << 40) |
			(read[1] << 48) |
			(read[0] << 56));
	}
}

unsigned long long ReadULongLong(FILE* f)
{
	if (e == LITTLE)
	{
		unsigned long long read;
		fread_s(&read, 8, 8, 1, f);
		return read;
	}
	else
	{
		uint8_t read[8];
		fread_s(read, 8, 8, 1, f);
		return static_cast<unsigned long long>(
			(read[7]) |
			(read[6] << 8) |
			(read[5] << 16) |
			(read[4] << 24) |
			(read[3] << 32) |
			(read[2] << 40) |
			(read[1] << 48) |
			(read[0] << 56));
	}
}

std::string ReadString(FILE* f)
{
	std::string read;
	char c;
	char c1 = '\0';
	fread_s(&c1, 1, 1, 1, f);
	char c2 = '\0';
	fread_s(&c2, 1, 1, 1, f);
	fseek(f, -2, SEEK_CUR);
	if (c1 != '\0' && c2 != '\0')
	{
		do
		{
			fread_s(&c, 1, 1, 1, f);
			if (c == '\0')
			{
				break;
			}
			read.push_back(c);
		} while (true);
	}
	else
	{
		int counter = 0;
		do
		{
			fread_s(&c, 1, 1, 1, f);
			if (c == '\0')
			{
				if (counter == 0 || read[counter - 1] == '\0')
				{
					break;
				}
			}
			read.push_back(c);
			counter++;
		} while (true);
	}

	// Remove all 0's
	for (size_t i = 0; i < read.length(); i++)
	{
		if (read[i] == '\0')
		{
			read.erase(i, 1);
		}
	}
	fseek(f, -1, SEEK_CUR);
	return read;
}
std::string ReadString(FILE* f, int chars)
{
	std::string read;
	char c;
	char c1 = '\0';
	fread_s(&c1, 1, 1, 1, f);
	char c2 = '\0';
	fread_s(&c2, 1, 1, 1, f);
	fseek(f, -2, SEEK_CUR);
	for (int i = 0; i < chars; i++)
	{
		fread_s(&c, 1, 1, 1, f);
		read.push_back(c);
	}
	return read;
}

std::wstring ReadUnicodeString(FILE* f)
{
	wchar_t uChar;
	std::wstring uStr;
	do
	{
		fread_s(&uChar, 2, 2, 1, f);
		if (uChar == '\0')
		{
			break;
		}
		uStr.push_back(uChar);
	} while (true);
	return uStr;
}

std::wstring ReadUnicodeString(FILE* f, int chars)
{
	uint8_t uBytes[2];
	wchar_t uChar;
	std::wstring uStr;
	if (e == Endian::LITTLE)
	{
		for (int i = 0; i < chars; i++)
		{
			fread_s(&uChar, 2, 2, 1, f);
			uStr.push_back(uChar);
		}
	}
	else
	{
		for (int i = 0; i < chars; i++)
		{
			fread_s(uBytes, 2, 2, 1, f);
			uChar = static_cast<wchar_t>((uBytes[1]) | (uBytes[0] << 8));
			uStr.push_back(uChar);
		}
	}
	return uStr;
}
