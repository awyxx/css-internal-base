#include "interface_scanner.hpp"

#include "utils.h"

interface_scanner::interface_scanner(uintptr_t connect_interfaces_fn) {

    uintptr_t cursor = connect_interfaces_fn;

    if (!cursor) {
        uintptr_t engine_dll = get_module_handle("engine.dll");

        // E8 ? ? ? ? 83 C4 08 6A 00 68 +1 DWORD
        uintptr_t start_address = find_pattern(engine_dll, 0x1FE000, "\x81\xE8\x04\x86\x83\xC4\x08\x6A\x00\x68");

        uint32_t relative_offset = *(uint32_t*) (start_address + 1);

        cursor = (start_address + 5) + relative_offset;
    }

    /*
    .text:1025B70A 5E                                                              pop     esi
    .text:1025B70B 5B                                                              pop     ebx
    .text:1025B70C 5D                                                              pop     ebp
    .text:1025B70D C3                                                              retn
    */

    uintptr_t end = find_pattern(cursor, 0x2C0, "\x84\x5E\x5B\x5D\xC3");

    while (cursor < end) {

        /*
        .text:1025B526 83 3D E4 A4 68 10 00                                            cmp     INTERFACE, 0
        .text:1025B52D 75 19                                                           jnz     short loc_1025B548
        */
        cursor = find_pattern(cursor, 100, "\x82\x83\x3D\x04\x81\x00", 2);

        if (!cursor)
            break;

        void* interface_ptr = **(void***) (cursor);

        /*
        .text:1025B554 6A 00                                                           push    0
        .text:1025B556 68 2C 66 38 10                                                  push    offset aVguiIvgui008 ;
        "VGUI_ivgui008" .text:1025B55B FF D0                                           call    eax
        */
        cursor = find_pattern(cursor, 20, "\x83\x6A\x00\x68\x04\x82\xFF\xD0", 3);

        if (!cursor)
            break;

        const char* interface_name = *(const char**) (cursor);

        if (interface_name && interface_ptr) {
            dbgprint("found interface ptr \"%s\" at %p\n", interface_name, interface_ptr);
            results.push_back({interface_name, interface_ptr});
        }
    }
}
interface_scanner::~interface_scanner() {}

