.text
.globl main

sum_num:
loop:
  beq $t0, $0, end_loop
  add $t1, $t1, $t0
  addi $t0, $t0, -1
  b loop

end_loop:
  li $v0, 4
  la $a0, sum
  syscall

  li $v0, 1
  add $a0, $0, $t1
  syscall

  li $v0, 4
  la $a0, nl
  syscall

  jr $ra


main:
  li $t1, 0

  li $v0, 4
  la $a0, numq
  syscall

  li $v0, 5
  syscall
  move $t0, $v0

  jal sum_num

  li $v0, 10
  syscall

.data
numq: .asciiz "Please enter a number: "
sum:   .asciiz "Result: "
nl:    .asciiz "\n"
