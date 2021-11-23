#ifndef __ECE30874_TEXTURE_H__
#define __ECE30874_TEXTURE_H__

#include <iostream>

class Texture {
    private:
        unsigned int width, height;
        bool swrap, twrap;
        float f;
        unsigned int* rgba;
        unsigned int txId;
    public:

    Texture();

    Texture(const char* filename);

    void use();

    void use(unsigned int index);

    void unuse();

    void free();

    const unsigned int* getRGBA() const;

    const unsigned int getWidth() const;

    const unsigned int getHeight() const;

    const float getF() const;

    friend std::ostream& operator<<(std::ostream& o, const Texture& tx);
};

#endif