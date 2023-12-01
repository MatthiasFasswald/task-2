#include <stdint.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// RISC-V Register set
const size_t zero = 0;
size_t a0, a1;                     // fn args or return args
size_t a2, a3, a4, a5, a6, a7;     // fn args
size_t t0, t1, t2, t3, t4, t5, t6; // temporaries
// Callee saved registers, must be stacked befor using it in a function!
size_t s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// Function: steins_algorithm
//
//
//-----------------------------------------------------------------------------
void steins_algorithm() {
  
  size_t stack_s1 = s1;
  size_t stack_s2 = s2;
  size_t stack_s3 = s3;

  
  s1 = a0; // a
  s2 = a1; // b

  if(s1 == zero) goto a_is_zero;
  if(s2 == zero) goto b_is_zero;
  
  s3 = zero; // k
  check_even:
    t1 = s1 | s2;
    t2 = t1 & 1;
    if(t2 != zero) goto make_a_odd;
    s1 = s1 >> 1;
    s2 = s2 >> 1;
    s3 = s3 + 1;
    goto check_even;

  make_a_odd:
    t1 = s1 & 1;
    if(t1 != zero) goto main_loop;
    s1 = s1 >> 1;
    goto make_a_odd;

  main_loop:
    t1 = s2 & 1;
    if(t1 != zero) goto compare_a_b;
    s2 = s2 >> 1;
    goto main_loop;

  compare_a_b:
    if(s1 <= s2) goto update_b;
    // Swap a and b
    t1 = s1;
    s1 = s2;
    s2 = t1;
    goto update_b;

  update_b:
    s2 = s2 - s1;
    if(s2 != zero) goto main_loop;
    goto steins_return;

  a_is_zero:
    a0 = s2;
    s1 = stack_s1;
    s2 = stack_s2;
    s3 = stack_s3;
    return;

  b_is_zero:
    a0 = s1;
    s1 = stack_s1;
    s2 = stack_s2;
    s3 = stack_s3;
    return;

  steins_return:
    a0 = s1 << s3;
    s1 = stack_s1;
    s2 = stack_s2;
    s3 = stack_s3;
    return;
}


//-----------------------------------------------------------------------------
//
// Function: binary_gcd
//
//
//-----------------------------------------------------------------------------
void binary_gcd() {
  size_t stack_s4 = s4; // loop counter
  size_t stack_s5 = s5;
  size_t stack_s6 = s6;
  size_t stack_s7 = s7;
  
  s4 = a0; // size
  s5 = a1; // array a
  s6 = a2; // array b
  s7 = a3; // array gcd

  gcd_loop:
    if(s4 == 0) goto gcd_loop_end;
    a0 = *(int*)s5;
    a1 = *(int*)s6;
    steins_algorithm();
    *(int*)s7 = a0;
    s5 = s5 + 4;
    s6 = s6 + 4;
    s7 = s7 + 4 ;
    s4 = s4 - 1;
    goto gcd_loop;

  gcd_loop_end:
    a3 = s7;
    s4 = stack_s4;
    s5 = stack_s5;
    s6 = stack_s6;
    s7 = stack_s7;
    return;
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
// a0: Address of array for a values
// a1: Address of array for b values
//
// Return value:
// a0: The number of elements read.
//
//-----------------------------------------------------------------------------
void input(void) {
  // Read size
  t0 = a0; // Save a0
  a0 = fscanf(stdin, "%08x\n", (int *)&t1);
  t4 = 1;
  if (a0 == t4)
    goto input_continue;
  // Early exit
  a0 = 0;
  return;

input_continue:
  t4 = 2;
  t5 = 10;
input_loop_begin:
  if (t5 == 0)
    goto after_input_loop;
  a0 = fscanf(stdin, "%08x\n%08x\n", (int *)&t2, (int *)&t3);
  if (a0 == t4)
    goto continue_read;
  // Exit, because read was not successful
  a0 = t1;
  return;
continue_read:
  *(int *)t0 = t2;
  *(int *)a1 = t3;
  // Pointer increment for next iteration
  t0 = t0 + 4;
  a1 = a1 + 4;
  // Loop counter decrement
  t5 = t5 - 1;
  goto input_loop_begin;

after_input_loop:
  a0 = t1;
  return;
}

//-----------------------------------------------------------------------------
//
// Function: output
//
// Prints all data back to stdout
//
// Input args:
// a0: Number of elements to print
// a1: Addres of array for a values
// a2: Addres of array for b values
// a3: Addres of array for gcd values
//
//-----------------------------------------------------------------------------
void output(void) {
before_output_loop:
  if (a0 == 0)
    goto after_output_loop;

  fprintf(stdout, "%08x\n%08x\n%08x\n", (unsigned int)*(int *)a1,
          (unsigned int)*(int *)a2, (unsigned int)*(int *)a3);

  // Pointer increment for next iteration
  a1 = a1 + 4;
  a2 = a2 + 4;
  a3 = a3 + 4;
  // Decrement loop counter
  a0 = a0 - 1;
  goto before_output_loop;

after_output_loop:
  return;
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

  a0 = (size_t)a;
  a1 = (size_t)b;
  input();
  size = a0;

  a1 = (size_t)a;
  a2 = (size_t)b;
  a3 = (size_t)gcd;
  binary_gcd();

  a0 = size;
  a1 = (size_t)a;
  a2 = (size_t)b;
  a3 = (size_t)gcd;
  output();

  return 0;
}
