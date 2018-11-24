.text
.globl main

# $t1 = resultat de la fonction prng
# $t2 = 300 puis devient n
# $t3 = guess
# $t4 = read_int
# $t5 = pour $ra
# $t6 = valeur temporaire

# Fonction prng
prng:
  li $t2, 300
  la $t1, prgn_state
  mul $t1, $t1, 1664525
  addu $t1, $t1, 1013904223
  li $t6, 4294967296
  divu $t1, $t6
  mfhi $t6
  remu $t1, $t6, $t2
  jr $ra

# Fonction jeu
jeu:

  # afficher la chaîne intvl
  li $v0, 4
  la $a0, intvl
  syscall

  # afficher le nombre max
  li $v0, 1
  move $a0, $t2
  syscall

  # afficher la chaine " : "
  li $v0, 4
  la $a0, colon
  syscall

  # afficher la chaine "\n"
  li $v0, 4
  la $a0, nl
  syscall

  # guess = -1
  move $t2, $t1
  li $t3, -1

  # entrer dans la boucle 
  b loop

  jr $ra


# boucle principale (guess != n?)
loop:
  beq $t2, $t3, end_loop      # n == guess?

  li $v0, 5
  syscall
  move $t3, $v0               # read_int, $t3 = v0

  blt $t2, $t3, print_grand   # n < guess?
  bgt $t2, $t3, print_petit   # n > guess?

  b loop


# fin de la boucle principale 
end_loop:
  li $v0, 4
  la $a0, exact
  syscall
  jr $ra

# afficher "Trop petit !"
print_petit:
  li $v0, 4
  la $a0, petit
  syscall
  b loop

# afficher "Trop grand !"
print_grand:
  li $v0, 4
  la $a0, grand
  syscall
  b loop


main:
  move $t5, $ra
  jal prng
  move $ra, $t5

  move $t5, $ra
  jal jeu
  move $ra, $t5

  jr $ra

.data
prgn_state: .word 42
intvl:      .asciiz "Le nombre est entre 0 et "
petit:      .asciiz "Trop petit !\n"
grand:      .asciiz "Trop grand !\n"
exact:      .asciiz "Bravo !\n"
colon:      .asciiz " : "
nl:         .asciiz "\n"

