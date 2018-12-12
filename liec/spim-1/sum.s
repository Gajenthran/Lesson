.text
.globl main

sum_num:
loop:                        # label "loop" qui représente une boucle
  beq $t0, $0, end_loop      # condition qui marque l'arrêt de la loupe ($t0 == 0?)
  add $t1, $t1, $t0          # t1 = t1 + t0
  addi $t0, $t0, -1          # t0 = t0 - 1
  b loop                     # on continue...

end_loop:                    # label "end_loop" qui représente la fin de la boucle
  li $v0, 4                  # v0 = 4 (pour afficher une chaîne)
  la $a0, sum                # a0 = sum ("result : ")
  syscall                    # affiche "result :"

  li $v0, 1                  # v0 = 1
  add $a0, $0, $t1           # a0 = t1 + 0
  syscall                    # affiche le contenu de $t1

  li $v0, 4                  # v0 = 4
  la $a0, nl                 # a0 = nl ("\n")
  syscall                    # affiche "\n"

  jr $ra


main:
  li $t1, 0                  # t1 = 0 

  li $v0, 4                  # v0 = 4
  la $a0, numq               # a0 = numq ("Please enter a number: ")
  syscall                    # affiche "Please enter a number: "

  li $v0, 5                  # v0 = 5
  syscall                    # read_int
  move $t0, $v0              # on place la valeur de read_int ($v0) dans $t0

  move $t5, $ra              # $t5 = $ra 
  jal sum_num                # appel de "sum_num"
  move $ra, $t5              # $ra = $t5

  jr $ra

.data
numq: .asciiz "Please enter a number: " 
sum:   .asciiz "Result: "
nl:    .asciiz "\n"
