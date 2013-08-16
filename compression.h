#ifndef __COMPRESSION_H
#define __COMPRESSION_H

#include "zlib.h"
#include <string>

namespace Utilities
{
    class ByteArray;
}

namespace Compression
{

    enum CompressionRate {
        NO_COMPRESSION = Z_NO_COMPRESSION,
        BEST_SPEED = Z_BEST_SPEED,
        BEST_COMPRESSION = Z_BEST_COMPRESSION,
        DEFAULT_COMPRESSION = Z_DEFAULT_COMPRESSION,
    };

    class Zlib
    {
        public:
            Utilities::ByteArray deflate(const Utilities::ByteArray & input, const CompressionRate rate = CompressionRate::DEFAULT_COMPRESSION); // Compress string
            Utilities::ByteArray inflate(const Utilities::ByteArray & input); // Uncompress string
    };

}

#endif // compression.h

