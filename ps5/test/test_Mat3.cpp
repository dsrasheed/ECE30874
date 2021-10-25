#include <iostream>
#include <cassert>
#include <stdlib.h>

#include "Vec3.h"
#include "Mat3.h"

void check_contents(Vec3 v, float x, float y, float z) {
  assert(v[0] == x && v[1] == y && v[2] == z);
}

void check_contents(Mat3 m,
  float x1, float y1, float z1, 
  float x2, float y2, float z2,
  float x3, float y3, float z3
  ) {
  check_contents(m[0], x1, y1, z1);
  check_contents(m[1], x2, y2, z2);
  check_contents(m[2], x3, y3, z3);
}

int main() {
  // Zero constructor
  Mat3 m;
  check_contents(m, 
    0, 0, 0,
    0, 0, 0,
    0, 0, 0
  );

  // subscript indexing
  m = Mat3();
  m[0][0] = 1;
  m[1][1] = 1;
  m[2][2] = 1;
  check_contents(m, 
    1, 0, 0,
    0, 1, 0,
    0, 0, 1
  );

  // initializing with float array
  float data[] = {1,2,3,4,5,6,7,8,9,10,11,12};
  m = Mat3(data);
  check_contents(m, 
    1, 2, 3,
    4, 5, 6,
    7, 8, 9
  );
  data[0] = 400;
  check_contents(m, 
    1, 2, 3,
    4, 5, 6,
    7, 8, 9
  );

  // initializing with Vec3
  m = Mat3(Vec3(1,2,3), Vec3(4,5,6), Vec3(7,8,9));
  check_contents(m, 
    1, 2, 3,
    4, 5, 6,
    7, 8, 9
  );

  // +, +=
  Mat3 m1 = Mat3(Vec3(1,2,3), Vec3(4,5,6), Vec3(7,8,9));
  Mat3 m2 = Mat3(Vec3(1,2,3), Vec3(4,5,6), Vec3(7,8,9));
  check_contents(m1 + m2, 
    2, 4, 6,
    8, 10, 12,
    14, 16, 18
  );
  m1 += m2;
  check_contents(m1, 
    2, 4, 6,
    8, 10, 12,
    14, 16, 18
  );

  // -, -=
  m1 = Mat3(Vec3(1,2,3), Vec3(4,5,6), Vec3(7,8,9));
  m2 = Mat3(Vec3(1,2,3), Vec3(4,5,6), Vec3(7,8,9));
  check_contents(m1 - m2, 
    0, 0, 0,
    0, 0, 0,
    0, 0, 0
  );
  m1 -= m2;
  check_contents(m1, 
    0, 0, 0,
    0, 0, 0,
    0, 0, 0
  );

  // *, *=
  m1 = Mat3(1, 2, 3, 4, 5, 6, 7, 8, 9);
  check_contents(m1 * 3,
    3, 6, 9,
    12, 15, 18,
    21, 24, 27
  );
  check_contents(3 * m1,
    3, 6, 9,
    12, 15, 18,
    21, 24, 27
  );
  m1 *= 3;
  check_contents(m1,
    3, 6, 9,
    12, 15, 18,
    21, 24, 27
  );

  // =
  m1 = Mat3(1, 2, 3, 4, 5, 6, 7, 8, 9);
  m2 = m1;
  check_contents(m2,
    1, 2, 3,
    4, 5, 6,
    7, 8, 9
  );
  m1[0][0] = 2;
  check_contents(m2,
    1, 2, 3,
    4, 5, 6,
    7, 8, 9
  );

  // Mat3 * Vec3
  check_contents(Mat3(
    3, 2, 1,
    4, 5, 6,
    8, 9, 7
  ) * Vec3(3, 4, 5), 3*3 + 2*4 + 5, 62, 24 + 36 + 35);
  check_contents(Mat3(
    1, 0, 0,
    0, 1, 0,
    0, 0, 1
  ) * Vec3(4, 5, 6), 4, 5, 6);
  check_contents(Mat3(
    -1, 0, 0,
    0, -1, 0,
    0, 0, -1
  ) * Vec3(4, 5, 6), -4, -5, -6);

  // Mat3 * Mat3
  check_contents(
  Mat3(
    3, 7, 9,
    8, 6, 8,
    4, 7, 6
  ) *
  Mat3(
    9, 2, 6,
    4, 6, 1,
    4, 5, 3
  ),
  91, 93, 52,
  128, 92, 78,
  88, 80, 49);

  // Getting floating point data
  m1 = Mat3(1,2,3,4,5,6,7,8,9);
  float* data2 = m1.data();
  for (int i = 0; i < 9; i++) {
    assert(data2[i] == i+1);
  }
  delete data2;
}
