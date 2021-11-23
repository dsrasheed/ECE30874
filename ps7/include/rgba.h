#ifndef __ECE30834_RGBA_H__
#define __ECE30834_RGBA_H__

class RGBA {
 public:
  static unsigned char floatToByte (float x) {
    int y = 255.0f*x + 0.5f;
    return y < 0 ? 0 : (y > 255 ? 255 : y);
  }

  union {
    unsigned char c[4];
    unsigned int i;
  } d;

  RGBA (unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    d.c[0] = r; d.c[1] = g; d.c[2] = b; d.c[3] = a;
  }
  // construct from float r, g, b
  RGBA (float rf, float gf, float bf)
    : RGBA(floatToByte(rf), floatToByte(gf), floatToByte(bf), 255u) {}
  RGBA (unsigned int rgb = 0u) { d.i = rgb; }
  // get r, g, b char values
  unsigned char getR () const { return d.c[0]; }
  unsigned char getG () const { return d.c[1]; }
  unsigned char getB () const { return d.c[2]; }
  // convert to unsigned int
  unsigned int pack () const { return d.i; }
  // mix two rgb values
  RGBA mix (float u, const RGBA &x) const {
    float v = 1.0f - u;
    unsigned char b[4];
    for (unsigned int i = 0u; i < 4u; ++i)
      b[i] = round(u*d.c[i] + v*x.d.c[i]);
    return RGBA(b[0], b[1], b[2], b[3]);
  }
};

#endif