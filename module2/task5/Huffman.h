typedef unsigned char byte;
#define interface struct

interface IInputStream {
    virtual bool Read(byte &value) = 0;
};

interface IOutputStream {
    virtual void Write(byte value) = 0;
};