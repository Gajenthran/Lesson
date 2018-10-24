.text
.globl main

main:
  li $v0, 4         # v0 = 4 (= print_string - afficher la chaîne)
  la $a0, num1q     # a0 = #num1q (= adresse de la chaîne en mémoire)
  syscall           # execute l'instruction selon la valeur de v0 (ici 4 donc afficher la chaîne a0)

  li $v0, 5         # v0 = 5 (= read_int - lire un entier en entrée = equivalent d'un scanf)
  syscall           # execute l'instruction selon la valeur de v0 (ici 5 donc lire un entier en entrée)
  move $t0, $v0     # t0 = v0 (on récupère la valeur retournée par l'appel de read_int)

  li $v0, 4         # v0 = 4 (= print_string - afficher la chaîne)
  la $a0, num2q     # a0 = #num2q (= adresse de la chaîne en mémoire)
  syscall           # execute l'instruction selon la valeur de v0 (ici 4 donc afficher la chaîne a0)

  li $v0, 5         # v0 = 5 (= read_int - lire un entier en entrée = equivalent d'un scanf)
  syscall           # execute l'instruction selon la valeur de v0 (ici 5 donc lire un entier en entrée)
  move $t1, $v0     # t1 = v0  (on récupère la valeur retournée par l'appel de read_int)

  li $v0, 4         # v0 = 4 (= print_string - afficher la chaîne)
  la $a0, sum       # a0 = #sum (= adresse de la chaîne en mémoire)
  syscall           # execute l'instruction selon la valeur de v0 (ici 4 donc afficher la chaîne a0)

  add $a0, $t0, $t1 # a0 = t0 + t1
  li $v0, 1         # v0 = 1 (= print_int - afficher un nombre entier)
  syscall           # execute l'instruction selon la valeur de v0 (ici 1 donc afficher l'entier a0)

  li $v0, 4         # v0 = 4 (= print_string - afficher la chaîne)
  la $a0, nl        # a0 = #nl (= adresse de la chaîne en mémoire)
  syscall           # execute l'instruction selon la valeur de v0 (ici 4 donc afficher la chaîne a0)

  jr $ra

.data
num1q: .asciiz "Please enter a first number: "      # on met une chaîne de caractères dans num1q
num2q: .asciiz "Please enter a second number: "     # on met une chaîne de caractères dans num2q
sum: .asciiz "The sum of these numbers is: "        # on met une chaîne de caractères dans sum
nl: .asciiz "\n"                                    # on met une chaîne de caractères dans nl
