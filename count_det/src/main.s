.text

.global count_det

count_det:

    ldr r1, [r0], #4
    ldr r2, [r0], #4
    ldr r3, [r0], #4
    ldr r4, [r0], #4
    mul r5, r3, r4
    mul r6, r2, r4
    ldr r4, [r0], #4
    mul r7, r1, r4
    mul r3, r3, r4
    ldr r4, [r0], #4
    mul r2, r2, r4
    mul r1, r1, r4
    ldr r4, [r0], #4
    mul r3, r3, r4
    mul r2, r2, r4
    sub r2, r2, r3
    ldr r4, [r0], #4
    mul r1, r1, r4
    mul r5, r5, r4
    sub r1, r5, r1
    add r1, r1, r2
    ldr r4, [r0], #4
    mul r6, r6, r4
    mul r7, r7, r4
    sub r0, r1, r6
    add r0, r0, r7
    bx lr
