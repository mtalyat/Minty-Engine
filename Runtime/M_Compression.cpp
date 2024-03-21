#include "pch.h"
#include "M_Compression.h"

#include <zlib.h>

using namespace minty;

unsigned long minty::compress_bound(unsigned long const sourceSize)
{
    return compressBound(sourceSize);
}

int minty::compress(void* const destination, unsigned long& destinationSize, void* const source, unsigned long const sourceSize, CompressionLevel const level)
{
    return compress2(static_cast<Bytef*>(destination), &destinationSize, static_cast<Bytef*>(source), sourceSize, static_cast<int>(level));
}

int minty::uncompress(void* const destination, unsigned long& destinationSize, void* const source, unsigned long& sourceSize)
{
    return uncompress2(static_cast<Bytef*>(destination), &destinationSize, static_cast<Bytef*>(source), &sourceSize);
}
