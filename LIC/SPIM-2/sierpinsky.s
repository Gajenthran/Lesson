.text
.globl main

sierpinsky_f:
  li $t0, 1                       
  lw $t5, max
loop:
  bge $t0, $t5, end_loop
  move $t2, $t0
  li $t4, 2
  mul $t4, $t0, $t4
  xor $t0, $t0, $t4
  b sloop

sloop: 
  li $t4, 0                     
  ble $t2, $t4, end_sloop
  li $t4, 1
  and $t1, $t2, $t4
  li $t4, 0
  beq $t1, $t4, print_space
  bne $t1, $t4, print_hash

end_sloop:
  li $v0, 4
  la $a0, nl
  syscall
  b loop

end_loop:
  jr $ra


print_hash:
  li $v0, 4
  la $a0, hash
  syscall

  li $t4, 2
  div $t2, $t2, $t4
  b sloop

print_space:
  li $v0, 4
  la $a0, space
  syscall

  li $t4, 2
  div $t2, $t2, $t4
  b sloop


main:
  jal sierpinsky_f

  li $v0, 10
  syscall

.data
max: .word 20000000
hash: .asciiz "#"
space:   .asciiz " "
nl:    .asciiz "\n"


