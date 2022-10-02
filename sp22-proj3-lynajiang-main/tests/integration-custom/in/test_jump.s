jal x0, label1
addi s1, x0, 3
label1:
addi a0, x0, 2

la s0, label2
jalr x0, s0, 0


label2:
addi t0, x0, 5

