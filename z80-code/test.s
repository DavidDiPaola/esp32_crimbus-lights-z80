/* 2019 David DiPaola, licensed CC0 (public domain worldwide) */

.org 0x0000
	ld c, 0x00
loop:
	call set_on
	jp loop



set_on:
	ld c, 0
_a:
	ld a, 0xFF
	out (c), a
	inc c
	ld a, c
	cp 0
	jp nz, _a

	ret



done:
	jp done

