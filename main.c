//     Nspire Version Patcher
//     Copyright (C) 2012  Daniel Tang
//
//     This program is free software: you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
//
//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
//
//     You should have received a copy of the GNU General Public License
//     along with this program.  If not, see <http://www.gnu.org/licenses/>.



#include <os.h>

// Keep this short and awesome
// Strings that are 9 bytes or less are definitely safe.
// From a quick read of the disassembly, I believe strings
// up to 127 bytes + 1 null byte *should* be safe.
// Anything longer than that and you'll probably meet
// undefined behavior
#define VERSION_STRING "Android 4.1.1 Jellybean"

// 3.1.0.392 CAS CX
#define PATCH_ADDR ((unsigned*)0x107ff028)

extern const unsigned patch_begin, patch_size, endinst_offset, ldrinst;

struct patch_code {
    char * str;
    void * retaddr;
    unsigned payload[];
};

struct patch_code * make_patch(unsigned* patchaddr) {
    struct patch_code * patch = malloc(patch_size);
    if (!patch) return NULL;

    memcpy(patch, &patch_begin, patch_size);
    patch->retaddr = patchaddr+2;
    patch->payload[endinst_offset] = patchaddr[1];
    return patch;
}


int do_patch() {
    unsigned *patchaddr = PATCH_ADDR;
    if (patchaddr[0] == ldrinst) return 0; //Already patched
    struct patch_code * patch = make_patch(patchaddr);
    if (!patch) return -1;
    patchaddr[0] = ldrinst;
    patchaddr[1] = (unsigned)(&patch->payload);
    return 0;
}

struct patch_code * get_patch(unsigned *patchaddr) {
    if (*patchaddr != ldrinst) return NULL;
    patchaddr++;
    return (struct patch_code *)(*patchaddr - offsetof(struct patch_code, payload) );
}

void set_string(char *str) {
    struct patch_code * patch = get_patch(PATCH_ADDR);
    if (!patch) return;

    if (patch->str) free(patch->str);
    //show_msg_user_input("Enter custom version string.", "Warning: long strings could cause undef behavior", "Woot",strptr);
    patch->str = malloc(strlen(str)+2);
    strcpy(patch->str, " ");
    strcat(patch->str, str);
}

int main() {
    if (!do_patch()) set_string(VERSION_STRING);
	return 0;
}
