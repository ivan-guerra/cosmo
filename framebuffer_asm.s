global outb                  ; Make the label outb visible outside this file

    ; outb - send a byte to an I/O port.
    ; stack: [esp + 8] The data byte.
    ;        [esp + 4] The I/O port.
    ;        [esp    ] Return address.
    outb:
        mov al, [esp + 8]    ; Move the data to be sent into the al register.
        mov dx, [esp + 4]    ; Move the address of the I/O port into the dx register.
        out dx, al           ; Send the data to the I/O port.
        ret                  ; Return to the calling function.
