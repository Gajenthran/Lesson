.text
.globl main

# $t0 = x
# $t5 = 2147483648
# $t2 = n

sierpinsky_f:
  li $t0, 1                       # $t0 = x = 1
  lw, $t5, max

loop:                             # première boucle
  bgeu $t0, $t5, end_loop         # $t0 >= $t5??
  move $t2, $t0                   # $t2 = $t0 (n = x) donc $t2 = n
  li $t4, 2                       # $t4 = 2
  mul $t4, $t0, $t4               # $t4 = $t0 * $t4 donc $t4 = x * 2
  xor $t0, $t0, $t4               # $t0 = $t0 ^ $t4
  b sloop                         # continue sur la boucle princ...

sloop:                            # deuxième boucle
  li $t4, 0                       # $t4 = 0
  ble $t2, $t4, end_sloop         # $t2 <= $t4?? (n <= 0)
  li $t4, 1                       # $t4 = 1
  and $t1, $t2, $t4               # $t1 = $t2 & t4
  li $t4, 0                       # $t4 = 0
  beq $t1, $t4, print_space       # $t1 == $t4 (n & 1 == 0)
  bne $t1, $t4, print_hash        # $t1 != $t4 (n & 1 != 0)

end_sloop:                        # fin de la deuxième boucle
  li $v0, 4 
  la $a0, nl
  syscall                         # on affiche un "\n"
  b loop                          # on retourne à la boucle princ

end_loop:                         # fin de la boucle princ
  jr $ra                          # on retourne au "main"


print_hash:                       # on affiche un "#"
  li $v0, 4
  la $a0, hash
  syscall

  li $t4, 2
  div $t2, $t2, $t4
  b sloop

print_space:                      # on affiche un " "
  li $v0, 4
  la $a0, space
  syscall

  li $t4, 2
  div $t2, $t2, $t4
  b sloop


main:
  move $t6, $ra
  jal sierpinsky_f
  move $ra $t6

  jr $ra

.data
max:   .word 2147483648
hash:  .asciiz "#"
space: .asciiz " "
nl:    .asciiz "\n"


