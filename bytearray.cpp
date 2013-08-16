#include "bytearray.h"
#include <iostream>
#include <stdexcept>

Utilities::ByteArray::ByteArray()
{
    init();
}

Utilities::ByteArray::ByteArray(const char * b, int len)
{
    init();
    append(b, len);
}

Utilities::ByteArray::~ByteArray()
{
    delete _buffer;
}

void Utilities::ByteArray::append(const char * b, int len)
{
    if (_capacity < _size+len) {
        int newCapacity = (_size+len) * 2;
        resize(newCapacity);
    }

    for (int i = 0; i < len; i++) {
        at(_size++) = b[i];
    }
}

void Utilities::ByteArray::resize(int toSize)
{
    if (toSize == 0) {
        delete _buffer;
        init();
        return;
    }

    char * b = new char[toSize];
    for (int i = 0; i < _size; i++) {
        if (i == toSize) break;
        b[i] = _buffer[i];
    }

    delete _buffer;

    _buffer = b;
    _capacity = toSize;
    if (_size > _capacity) _size = _capacity;
}

char & Utilities::ByteArray::at(int i)
{
    if (i < _size) {
        return _buffer[i];
    }
    throw std::runtime_error("Index out of bounds");
}

char * Utilities::ByteArray::data()
{
    return (char *) _buffer;
}

const char * Utilities::ByteArray::data() const
{
    return (const char *) _buffer;
}

Utilities::ByteArray Utilities::ByteArray::mid(int pos, int len) const
{
    if (pos > _size) {
        Utilities::ByteArray b;
        return b;
    }

    char * buffer = new char[len];
    if (pos+len > _size) len = _size - pos;
    for (int i = pos, j = 0; i < pos+len; i++, j++) buffer[j] = _buffer[i];
    return ByteArray(buffer, len);
}

Utilities::ByteArray Utilities::ByteArray::deflate(const Compression::CompressionRate rate) const
{
    Compression::Zlib zlib;
    return zlib.deflate(*this, rate);
}

Utilities::ByteArray Utilities::ByteArray::inflate() const
{
    Compression::Zlib zlib;
    return zlib.inflate(*this);
}

bool Utilities::ByteArray::operator==(Utilities::ByteArray &ba) const
{
    if (size() != ba.size()) return false;
    for (int i = 0; i < size(); i++)
        if (_buffer[i] != ba.at(i)) return false;
    return true;
}

void Utilities::ByteArray::init()
{
    _size = 0;
    _capacity = 0;
    _buffer = 0;
}

