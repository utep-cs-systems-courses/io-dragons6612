	.arch msp430g2553
	.p2align 1,0
	.text

	.global toggle_green
	.extern red_on, green_on

toggle_green:
	cmp #0, &red_on
	jz out
	XOR &green_on, #1
	mov #1, r12
	pop r0

out:
	mov #0, r12
	pop r0
