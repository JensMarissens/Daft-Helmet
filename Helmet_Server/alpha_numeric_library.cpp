#include "alpha_numeric_library.h"

static byte zero[5][4] = {
  { 1, 1, 1, 1 },
  { 1, 0, 0, 1 },
  { 1, 0, 0, 1 },
  { 1, 0, 0, 1 },
  { 1, 1, 1, 1 }
};

static byte A[5][4] = {
        {0, 1, 1, 0},
        {1, 0, 0, 1},
        {1, 1, 1, 1},
        {1, 0, 0, 1},
        {1, 0, 0, 1}
      };

byte getMatrixLetter(int row, int col) {
  return A[row][col];
}
