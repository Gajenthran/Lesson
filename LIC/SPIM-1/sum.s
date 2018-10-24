.text
.globl main

sum_num:

main:
  li $t2, 3               # t2 = 3 (on va donc boucler trois fois)

loop:                     # debut de la boucle avec le label "loop"
  beq $t2, $0, end_loop   # if(t2 == 0): on va dans le label "end_loop" qui signale la fin de la boucle
  addi $t2, $t2, -1       # t2 = t2 - 1
  jal add_user_num        # appel de add_user_num
  b loop                  # on rappelle le label "loop"

end_loop:                 # le label "end_loop"
  li $v0, 10              # v0 = 10 (= exit - quitter le programme)
  syscall                 # executer v0 donc quitte le programme

.data
num1q: .asciiz "Please enter a number: "
sum:   .asciiz "Result: "
nl:    .asciiz "\n"
