addi s1, x0, 42
addi s0, x0, 78

beq s1, s0, done
addi a0, x0, 52

bne s1, s1, done
addi t1, s0, 2

bge s1, s0, done
addi a0, x0, 2

bge s0, s1, done
addi a0, x0, 4


done:
addi t2, s1, 4


blt s1, s0, anotherDone
addi t2, s1, 8

anotherDone:
addi t2, s0, 5