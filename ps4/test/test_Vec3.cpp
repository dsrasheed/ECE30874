#include <iostream>
#include <cassert>

#include "Vec3.h"

void check_contents(Vec3 v, float x, float y, float z) {
  assert(v[0] == x && v[1] == y && v[2] == z);
}

int main() {
  Vec3 v;
  assert(v[0] == 0 && v[1] == 0 && v[2] == 0);
  check_contents(v, 0, 0, 0);

  // +
  Vec3 v1 = Vec3(5, 10, 11);
  Vec3 v2 = Vec3(5, 10, 11);
  Vec3 v3 = v1 + v2;
  Vec3 v4 = v2 + v1;
  check_contents(v3, 10, 20, 22);
  check_contents(v4, 10, 20, 22);

  // -
  v1 = Vec3(3, 3, 3) - Vec3(4, 4, 4);
  check_contents(v1, -1, -1, -1);

  // += and -=
  v3 = Vec3(10, 10, 10);
  v3 -= Vec3(1,  2,  3);
  assert(v3[0] == 9 && v3[1] == 8 && v3[2] == 7);
  check_contents(v3, 9, 8, 7);
  v3 += Vec3(1,  2,  3);
  check_contents(v3, 10, 10, 10);

  // *, *=
  v3 = Vec3(10, 10, 10);
  v3 = v3 * 2;
  check_contents(v3, 20, 20, 20);
  v3 = 2 * v3;
  check_contents(v3, 40, 40, 40);
  v3 *= 3;
  check_contents(v3, 120, 120, 120);
  (v3 *= 2) += Vec3(10, 10, 10);
  check_contents(v3, 250, 250, 250);

  // /, /=
  v3 = Vec3(10, 10, 10);
  v3 = v3 / 2;
  check_contents(v3, 5, 5, 5);
  v3 = 20 / Vec3(10, 10, 10);
  check_contents(v3, 2, 2, 2);
  v3 = Vec3(10, 10, 10);
  v3 /= 10;
  check_contents(v3, 1, 1, 1);

  // =
  v3 = Vec3(10, 10, 10);
  v4 = v3;
  check_contents(v4, 10, 10, 10);
  v3[0] = 20;
  check_contents(v3, 20, 10, 10);
  check_contents(v4, 10, 10, 10);

  // norm
  v3 = Vec3(4, 3, 0);
  assert(v3.norm() == 5);

  // setting through [] syntax
  v3 = Vec3(4, 3, 0);
  v3[2] = 2;
  check_contents(v3, 4, 3, 2);

}
