/* 2019 David DiPaola, licensed CC0 (public domain worldwide) */

.org 0x0000
	ld c, 0x00
loop:
	ld b, 0xFF
	call set_all

	ld b, 0x00
	call set_all

	jp loop



set_all:
	ld c, 0
1:
	ld a, b
	out (c), a
	inc c
	ld a, c
	cp 0
	jp nz, 1b

	ret

