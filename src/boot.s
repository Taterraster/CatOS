
.section .multiboot
.align 4
.long 0x1BADB002 
.long 0x00 
.long -(0x1BADB002) 


.section .text
.global _start
_start:

cli
call kernel_main
.hang:
hlt
jmp .hang