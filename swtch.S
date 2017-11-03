; Obligatory swtch.S for multitasking
;
; References
; https://github.com/mit-pdos/xv6-public/blob/master/swtch.S

; swtch(uint8_t *uesp, struct context **old, struct context *new)
GLOBAL swtch
swtch:
    mov eax, [esp + 8]      ; old
    mov edx, [esp + 12]     ; new

    push ebp
    push ebx
    push esi
    push edi

    mov [eax], esp
    mov esp, edx

    pop edi
    pop esi
    pop ebx
    pop ebp

    ret

; swtch_save(uint8_t *where)
GLOBAL swtch_save
swtch_save:
    mov eax, [esp + 4]
    mov [eax], edi
    mov [eax+4], esi
    mov [eax+8], ebx
    mov [eax+12], ebp
    ret
