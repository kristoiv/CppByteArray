#include <iostream>
#include <string>
#include "compression.h"
#include "bytearray.h"

int main(int argc, char **argv)
{
    // Get some data ready
    Utilities::ByteArray bytes("Test message again again again again again again");
    std::cout << "Orig data: '" << bytes.data() << "'" << std::endl;

    // Deflate (compress) it
    Utilities::ByteArray deflated = bytes.deflate();
    std::cout << "Compressed data: '" << deflated.data() << "'" << std::endl;

    // Inflated it again (decompress it)
    Utilities::ByteArray inflated = deflated.inflate();
    std::cout << "Uncompressed data: '" << inflated.data() << "'" << std::endl;

    // Compare the original to the reinflated bytearray and string
    std::cout << "Compare A and B: " << (bytes == inflated ? "true" : "false") << std::endl;
    std::cout << "Compare as strings A and B: " << (std::string(bytes.data(), bytes.size()) == std::string(inflated.data(), inflated.size()) ? "true" : "false") << std::endl;

    return 0;
}

