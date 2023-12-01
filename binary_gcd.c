#include <stdio.h>

//-----------------------------------------------------------------------------
//
// Function: steins_algorithm
//
// Computes the GCD using Stein's algorithm for input a and b.
//
// Input args:
// a: value a
// b: value b
//
// Return value:
// The gcd of the given input.
//
//-----------------------------------------------------------------------------
int steins_algorithm(int a, int b) {


  if(a == 0){
    return b;
  };
  if(b == 0){
    return a;
  };

  int k;
  // check if a and b are round numbers
  for (k = 0; ((a | b) & 1) == 0; ++k) { 
          a >>= 1;
          b >>= 1;
      }

  // Making 'a' odd
  while ((a & 1) == 0) {
    a >>= 1;
  }

  do {
      // Making 'b' odd
      while ((b & 1) == 0) {
          b >>= 1;
      }

      
      if (a > b) {
          int temp = a;
          a = b;
          b = temp;
      }

      // Update b
      b = b - a;
  }while(b != 0);
    return a<<k;
}


//-----------------------------------------------------------------------------
//
// Function: binary_gcd
//
// Computes the GCD using Stein's algorithm for a number of input pairs.
//
// Input args:
// size: The number of GCDs we want to compute
// a: Array for a values
// b: Array for b values
// gcd: Array for gcd values
//
//-----------------------------------------------------------------------------
void binary_gcd(int size, int *a, int *b, int *gcd) {
  for (int i = 0; i < size; i++) {
        gcd[i] = steins_algorithm(a[i], b[i]);
    }
}

//-----------------------------------------------------------------------------
//
// Function: input
//
// The first value from stdin is the number of input pairs. After that all
// input pairs are read and stored to arrays a and b given as parameters. At
// most of 10 values are read.
//
// Input args:
// a: Array for a values
// b: Array for b values
//
// Return value:
// The number of elements read.
//
//-----------------------------------------------------------------------------
int input(int *a, int *b) {
  int size;
  if (fscanf(stdin, "%08x\n", &size) != 1)
    return 0;

  for (int i = 0; i < 10; i++) {
    if (fscanf(stdin, "%08x\n%08x\n", a, b) != 2) {
      return size;
    }
    a++;
    b++;
  }
  return size;
}

//-----------------------------------------------------------------------------
//
// Function: output
//
// Prints all data back to stdout
//
// Input args:
// size: Number of elements to print
// a: Array for a values
// b: Array for b values
// gcd: Array for gcd values
//
//-----------------------------------------------------------------------------
void output(int size, int *a, int *b, int *gcd) {
  for (int i = 0; i < size; i++) {
    fprintf(stdout, "%08x\n%08x\n%08x\n", (unsigned int)a[i],
            (unsigned int)b[i], (unsigned int)gcd[i]);
  }
}

//-----------------------------------------------------------------------------
//
// Main function
//
// Reads input data (At most 10 values), calls binary_gcd() to compute the GCD
// using Stein's algorithm for all input data, and finally calls output() to
// return the data back to stdout.
//
//-----------------------------------------------------------------------------
int main(void) {
  int a[10], b[10], gcd[10];
  int size;

  size = input(a, b);
  binary_gcd(size, a, b, gcd);
  output(size, a, b, gcd);

  return 0;
}
