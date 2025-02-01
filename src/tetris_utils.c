#include "tetris.h"

tet_Piece tet_generate_random_piece() {
  /*
     Generates a random piece with random rotation and returns it
  */

  return (tet_Piece) {
    .index = tet_generate_random_int(0, NUMBER_OF_PIECES),
    .rotation = tet_generate_random_int(0, NUMBER_OF_ROTATIONS)
  };
}

int32_t tet_generate_random_int(int32_t min, int32_t max) {
  /*
     Generates a random integer between given min(inclusive) and max(exclusive) and returns it
  */

  return rand() % (max - min) + min;
}

double tet_generate_random_double(double min, double max) {
  /*
     Generates a random double between given min(inclusive) and max(exclusive) and returns it
  */

  return min + (rand() / (double)RAND_MAX) * (max - min);
}
