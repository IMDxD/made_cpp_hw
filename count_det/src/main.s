.text

.global count_det

count_det:
    push {r4}
    push {r5}
    push {r6}
    ldr r1, [r0], #4 //x11
    ldr r2, [r0], #4 //x12
    ldr r3, [r0], #4 //x13
    ldr r4, [r0], #4 //x21
    mul r5, r3, r4 //x13x21
    mul r6, r2, r4 //x12x21
    ldr r4, [r0], #4 //x22
    mul r3, r3, r4 //x13x22
    mul r4, r1, r4 //x11x22
    sub r6, r4, r6 //x11x22 - x12x21
    ldr r4, [r0], #4 //x23
    mul r2, r2, r4 //x12x23
    sub r2, r2, r3 //x12x23 - x13x22
    mul r1, r1, r4 //x11x23
    sub r1, r5, r1 //x13x21 - x11x23
    ldr r4, [r0], #4 //x31
    mul r2, r4, r2 //x12x23x31 - x13x22x31
    ldr r4, [r0], #4 //x32
    mul r1, r4, r1 //x13x21x32 - x11x23x32
    add r1, r2, r1 //x13x21x32 - x11x23x32 + x12x23x31 - x13x22x31
    ldr r4, [r0], #4 //x33
    mul r3, r6, r4  //x11x22x33 - x12x21x33
    add r0, r1, r3 //x13x21x32 - x11x23x32 + x12x23x31 - x13x22x31 + x11x22x33 - x12x21x33
    pop {r6}
    pop {r5}
    pop {r4}
    bx lr
