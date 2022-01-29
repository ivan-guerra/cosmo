global flush_idt

; Load the interrupt descriptor table (IDT).
; stack: [esp + 4] The address of the first entry in the IDT.
;        [esp    ] The return address.
flush_idt:
    mov eax, [esp+4] ; Set eax to the IDT address.
    lidt [eax]       ; Load the address of the IDT.
    ret
