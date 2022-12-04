/* -- first.s */
/* This is a comment */
/* Src: https://thinkingeek.com/2013/01/09/arm-assembler-raspberry-pi-chapter-1/ */
.global main /* 'main' is our entry point and must be global */

main:          /* This is main */
    mov r0, #2 /* Put a 2 inside the register r0 */
    bx lr      /* Return from main */
