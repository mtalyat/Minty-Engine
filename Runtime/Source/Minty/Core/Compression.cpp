#include "pch.h"
#include "Compression.h"

#include <zlib.h>

using namespace Minty;

unsigned long Minty::compress_bound(unsigned long const sourceSize)
{
    return compressBound(sourceSize);
}

int Minty::compress(void* const destination, unsigned long& destinationSize, void* const source, unsigned long const sourceSize, CompressionLevel const level)
{
    return compress2(static_cast<Bytef*>(destination), &destinationSize, static_cast<Bytef*>(source), sourceSize, static_cast<int>(level));
}

int Minty::uncompress(void* const destination, unsigned long& destinationSize, void* const source, unsigned long& sourceSize)
{
    return uncompress2(static_cast<Bytef*>(destination), &destinationSize, static_cast<Bytef*>(source), &sourceSize);
}
