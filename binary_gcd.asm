###############################################################################
# Startup code
#
# Initializes the stack pointer, calls main, and stops simulation.
#
# Memory layout:
#   0 ... ~0x300  program
#   0x7EC       Top of stack, growing down
#   0x7FC       stdin/stdout
#
###############################################################################

.org 0x00
_start:
  ADDI sp, zero, 0x7EC
  ADDI fp, sp, 0

  # set saved registers to unique default values
  # to make checking for correct preservation easier
  LUI  s1, 0x11111
  ADDI s1, s1, 0x111
  ADD  s2, s1, s1
  ADD  s3, s2, s1
  ADD  s4, s3, s1
  ADD  s5, s4, s1
  ADD  s6, s5, s1
  ADD  s7, s6, s1
  ADD  s8, s7, s1
  ADD  s9, s8, s1
  ADD  s10, s9, s1
  ADD  s11, s10, s1

  JAL  ra, main
  EBREAK

#-----------------------------------------------------------------------------
#
# Function: steins_algorithm
#
#
#-----------------------------------------------------------------------------
steins_algorithm:
  ADDI sp, sp, -16
  SW ra, 12(sp)
  SW fp, 8(sp)
  ADDI fp, sp, 16

  SW s1, -8(sp)                        # size_t stack_s1 = s1;
  SW s2, -12(sp)                        # size_t stack_s2 = s2;
  SW s3, -16(sp)

  ADDI s1, a0, 0
  ADDI s2, a1, 0

  BEQ s1, zero, a_is_zero
  BEQ s2, zero, b_is_zero

  ADDI s3, zero, 0

check_even:                            
  ADDI t0, zero, 1                      
  OR t1, s1, s2                         
  AND t2, t1, t0                       
  BNE t2, zero, make_a_odd              
  SRL s1, s1, t0                       
  SRL s2, s2, t0                        
  ADDI s3, s3, 1                       
  JAL zero, check_even                  

make_a_odd:
  ADDI t0, zero, 1
  AND t1, s1, t0
  BNE t1, zero, main_loop
  SRL s1, s1, t0
  JAL zero, make_a_odd

main_loop:
  ADDI t0, zero, 1  
  AND t1, s2, t0
  BNE t1, zero, comparre_a_b
  SRL s2, s2, t0
  JAL zero, main_loop

comparre_a_b:
  BGE s2, s1, update_b
  ADDI t1, s1, 0
  ADDI s1, s2, 0
  ADDI s2, t1, 0
  JAL zero, update_b

update_b:
  SUB s2, s2, s1
  BNE s2, zero, main_loop
  JAL zero, steins_return

a_is_zero:
  ADDI a0, s2, 0
  LW ra, 12(sp)
  LW s1, -8(sp)
  LW s2, -12(sp)
  LW s3, -16(sp)
  LW fp, 8(sp)
  ADDI sp, sp, 16

  JALR zero, 0(ra)

b_is_zero:
  ADDI a0, s1, 0
  LW ra, 12(sp)
  LW s1, -8(sp)
  LW s2, -12(sp)
  LW s3, -16(sp)
  LW fp, 8(sp)
  ADDI sp, sp, 16

  JALR zero, 0(ra)

steins_return:
  SLL a0, s1, s3
  LW ra, 12(sp)
  LW s1, -8(sp)
  LW s2, -12(sp)
  LW s3, -16(sp)
  LW fp, 8(sp)
  ADDI sp, sp, 16

  JALR zero, 0(ra)     # return;
  

#-----------------------------------------------------------------------------
#
# Function: binary_gcd
#
#
#-----------------------------------------------------------------------------
binary_gcd:
  ADDI sp, sp, -20
  SW ra, 16(sp)
  SW fp, 12(sp)
  ADDI fp, sp, 20

  SW s4, -8(sp)
  SW s5, -12(sp)
  SW s6, -16(sp)
  SW s7, -20(sp)

  ADDI s4, a0, 0         
  ADDI s5, a1, 0         
  ADDI s6, a2, 0         
  ADDI s7, a3, 0         


gcd_loop:
  BEQ s4, zero, gcd_loop_end
  LW a0, 0(s5)           
  LW a1, 0(s6)           
  JAL ra, steins_algorithm
  SW a0, 0(s7)           

  ADDI s5, s5, 4         
  ADDI s6, s6, 4         
  ADDI s7, s7, 4         
  ADDI s4, s4, -1
  JAL zero, gcd_loop

gcd_loop_end:
  ADDI a3, s7, 0
  LW s7, -20(sp)          
  LW s6, -16(sp)          
  LW s5, -12(sp)           
  LW s4, -8(sp)    

  LW fp, 12(sp)
  LW ra, 16(sp)           
  ADDI sp, sp, 20         
  JALR zero, 0(ra)        

###############################################################################
# Function: input
#
# Reads at most 10 values from stdin to input arrays.
#
# Input args:
# a0: Address for array a
# a1: Address for array b
# Return value:
# a0: Number of read elements
#
###############################################################################
input:
  ADDI t0, a0, 0                 # Save a0
  LW   a0, 0x7fc(zero)           # Load size
  ADDI t1, zero, 10              # Maximum
  ADDI t2, zero, 0               # Loop counter
before_input_loop:
  BGE t2, t1, after_input_loop   # Maximum values reached
  BGE t2, a0,  after_input_loop  # All values read

  # Read from stdin in store in array a and b
  LW t3, 0x7fc(zero)
  LW t4, 0x7fc(zero)
  SW t3, 0(t0)
  SW t4, 0(a1)
  # Pointer increments
  ADDI t0, t0, 4
  ADDI a1, a1, 4

  ADDI t2, t2, 1                 # Increment loop counter
  JAL zero, before_input_loop    # Jump to loop begin

after_input_loop:
  JALR zero, 0(ra)


###############################################################################
# Function: output
#
# Prints input and output values to stdout
#
# Input args:
# a0: Number of elements
# a1: Address for array a
# a2: Address for array b
# a3: Address for array gcd
#
###############################################################################
output:
  BEQ a0, zero, after_output_loop
  # Load values
  LW t2, 0(a1)
  LW t3, 0(a2)
  LW t4, 0(a3)

  # Output Values to stdout
  SW t2, 0x7fc(zero)
  SW t3, 0x7fc(zero)
  SW t4, 0x7fc(zero)

  # Pointer increments
  ADDI a1, a1, 4
  ADDI a2, a2, 4
  ADDI a3, a3, 4

  # Decrement loop counter
  ADDI a0, a0, -1
  # jump to beginning
  JAL zero, output

after_output_loop:
  JALR zero, 0(ra)

###############################################################################
# Function: main
#
# Calls input, binary_gcd, and output
#
###############################################################################
main:
  # Make room for  # return;
  #    ra
  #    size
  #    int a[10]
  #    int b[10]
  #    int gcd[10]
  ADDI sp, sp, -148            # prologue
  SW ra, 0(sp)                 # Store return address

  ADDI a0, sp, 8               # &a[0]
  ADDI a1, sp, 48              # &b[0]

  JAL ra, input                # call input
  SW a0, 4(sp)                 # Store size to stack

  ADDI a1, sp, 8               # &a[0]
  ADDI a2, sp, 48              # &b[0]
  ADDI a3, sp, 88              # &gcd[0]

  JAL ra, binary_gcd           # call binary_gcd

  LW a0, 4(sp)                 # Load size fron stack
  ADDI a1, sp, 8               # &a[0]
  ADDI a2, sp, 48              # &b[0]
  ADDI a3, sp, 88              # &gcd[0]

  JAL ra, output               # call output

  ADDI a0, zero, 0             # return 0

  LW ra, 0(sp)                 # Epilogue
  ADDI sp, sp, 148
  JALR zero, 0(ra)
