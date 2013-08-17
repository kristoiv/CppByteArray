#include "compression.h"
#include "bytearray.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <exception>

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

//#define ZLIB_CHUNK 262144
#define ZLIB_CHUNK 1

Utilities::ByteArray Compression::Zlib::deflate(const Utilities::ByteArray & input, const Compression::CompressionRate rate)
{
    int result, round = 0, flush;
    z_stream zstream;
    unsigned char out_buffer[ZLIB_CHUNK];
    Utilities::ByteArray output;

    zstream.zalloc = Z_NULL;
    zstream.zfree  = Z_NULL;
    zstream.opaque = Z_NULL;

    result = ::deflateInit(&zstream, rate);
    if (result != Z_OK) {
        throw std::runtime_error("Failed to initiate deflation");
    }

    do {

        Utilities::ByteArray in = input.mid(ZLIB_CHUNK * round++, ZLIB_CHUNK);

        flush = (input.size()-ZLIB_CHUNK * round <= 0 ? Z_FINISH : Z_NO_FLUSH);

        zstream.avail_in = in.size();
        zstream.next_in  = (unsigned char *) in.data();

        do {

            zstream.avail_out = ZLIB_CHUNK;
            zstream.next_out  = out_buffer;

            result = ::deflate(&zstream, flush);
            if (result == Z_STREAM_ERROR) throw std::logic_error("Z_STREAM_ERROR fired in deflate operation");

            output.append((const char *)out_buffer, ZLIB_CHUNK - zstream.avail_out);

        }while(zstream.avail_out == 0);

    }while(flush != Z_FINISH);

    (void) ::deflateEnd(&zstream);

    return output;
}

Utilities::ByteArray Compression::Zlib::inflate(const Utilities::ByteArray & input)
{
    Utilities::ByteArray output;

    int result, round = 0;
    z_stream zstream;
    unsigned char out_buffer[ZLIB_CHUNK];

    zstream.zalloc = Z_NULL;
    zstream.zfree  = Z_NULL;
    zstream.opaque = Z_NULL;
    zstream.avail_in = 0;
    zstream.next_in = Z_NULL;

    result = ::inflateInit(&zstream);
    if (result != Z_OK) throw std::runtime_error("Failed inflation initialization");

    do {

        Utilities::ByteArray in = input.mid(ZLIB_CHUNK * round++, ZLIB_CHUNK);
        if (in.size() == 0) break;

        zstream.avail_in = in.size();
        zstream.next_in  = (unsigned char *) in.data();

        do {

            zstream.avail_out = ZLIB_CHUNK;
            zstream.next_out  = out_buffer;

            result = ::inflate(&zstream, Z_NO_FLUSH);

            if (result == Z_NEED_DICT || result == Z_DATA_ERROR || result == Z_MEM_ERROR) throw std::runtime_error("Inflation failed");

            output.append((const char *)out_buffer, ZLIB_CHUNK - zstream.avail_out);

        }while(zstream.avail_out == 0);

    }while(result != Z_STREAM_END);

    (void) ::inflateEnd(&zstream);

    return output;
}

