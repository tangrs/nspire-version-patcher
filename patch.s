@     Nspire Version Patcher
@     Copyright (C) 2012  Daniel Tang
@
@     This program is free software: you can redistribute it and/or modify
@     it under the terms of the GNU General Public License as published by
@     the Free Software Foundation, either version 3 of the License, or
@     (at your option) any later version.
@
@     This program is distributed in the hope that it will be useful,
@     but WITHOUT ANY WARRANTY; without even the implied warranty of
@     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
@     GNU General Public License for more details.
@
@     You should have received a copy of the GNU General Public License
@     along with this program.  If not, see <http://www.gnu.org/licenses/>.

.global patch_begin
.global patch_size
.global endinst_offset
.global ldrinst

@ This is a fake sprintf (really just a strcpy)
patch_begin:
    strptr: .word 0
    retaddr: .word 0

    payload:
    ldr r1,strptr
    cmp r1, #0
    beq endinst
    mov r2, r0
    mov r0, #0
    copy:
        ldrb r3,[r1]
        strb r3, [r2]
        add r0, r0, #1
        add r1, r1, #1
        add r2, r2, #1
        cmp r3, #0
        bne copy

    endinst: nop
    ldr pc,retaddr
patch_end:

patch_size: .word (patch_end-patch_begin)
endinst_offset: .word (endinst-payload)>>2
ldrinst: ldr pc,[pc, #-4]
