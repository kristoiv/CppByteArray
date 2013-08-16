#ifndef __BYTEARRAY_H
#define __BYTEARRAY_H

#include "compression.h"

namespace Utilities
{

    class ByteArray
    {
        public:
            ByteArray();
            ByteArray(const char * b, int len);
            ~ByteArray();
            void append(const char * b, int len);
            void resize(int toSize);
            char & at(int i);
            char & operator[](int i) {return at(i);}
            int size() const {return length();}
            int length() const {return _size;}
            int capacity() const {return _capacity;}
            char * data();
            const char * data() const;
            ByteArray mid(int pos, int len) const;
            ByteArray deflate(const Compression::CompressionRate rate = Compression::CompressionRate::DEFAULT_COMPRESSION) const;
            ByteArray inflate() const;
            bool operator==(ByteArray &ba) const;

        protected:
            int _size;
            int _capacity;
            char * _buffer;

        private:
            void init();
    };

}

#endif // bytearray.h

