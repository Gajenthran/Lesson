.text
.globl main

main:
  nop                   # initialise les valeurs
  li $t0, 42            # on met la valeur 42 dans t0
  li $t1, 9             # on met la valeur 9 dans t1
  add $t2, $t0, $t1     # on additionne t0 et t1 et on met le resultat dans t2 (t2 = t0 + t1 donc t2 = 51)
  move $a0, $t2         # on met la valeur de t2 dans a0 (a0 = t2 donc a0 = 51)
  li $v0, 1             # on met la valeur 1 dans v0 (1 = print_int - afficher l'entier)
  syscall               # appel du système pour executer une instruction selon la valeur de v0 (afficher 51 ici)
  jr $ra
