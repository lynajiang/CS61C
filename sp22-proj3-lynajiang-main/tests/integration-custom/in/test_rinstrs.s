add s1, x0, x0
addi t1, x0, 3
addi t4, x0, 16
addi t3, x0, 5
addi t5, x0, 17
sub t2, t4, t5

add s0, s1, t1
add s1, t1, t1
add s1, t3, t2

sub t0, s1, t1
sub a0, t1, t1

and a0, s1, t1
and a0, t4, t1

or a0, t4, t1
or a0, s1, t1

xor s0, t4, s1
xor a0, t4, t1

sll a0, t4, t1
sll a0, t4, t3

srl a0, t4, t1
srl a0, t4, t3

sra a0, t4, t1
sra a0, t4, t3

slt a0, t4, t3
slt a0, t4, t5

mul a0, t4, t3
mul a0, t4, t5

mulh a0, t4, t1
mulh a0, t4, t2

mulhu a0, t2, t3
mulhu a0, t4, t2
mulhu a0, t4, t3

