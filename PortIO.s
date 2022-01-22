global outb
global inb

; outb - send a byte to an I/O port.
; stack: [esp + 8] The data byte.
;        [esp + 4] The I/O port.
;        [esp    ] Return address.
outb:
    mov al, [esp + 8]    ; Move the data to be sent into the al register.
    mov dx, [esp + 4]    ; Move the address of the I/O port into the dx register.
    out dx, al           ; Send the data to the I/O port.
    ret                  ; Return to the calling function.

; inb - returns a byte from the given I/O port
; stack: [esp + 4] The address of the I/O port
;        [esp    ] The return address
inb:
mov dx, [esp + 4]       ; Move the address of the I/O port to the dx register.
in  al, dx              ; Read a byte from the I/O port and store it in the al register.
ret                     ; Return the read byte.
