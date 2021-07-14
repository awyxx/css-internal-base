#pragma once

#include <array>
#include <memory.h>

#include <Windows.h>

#ifdef _DEBUG
#include <stdio.h>
#define dbgprint           printf_s
#define dbgvar(x)          printf_s("%16s -> %8x\n", #x, (uintptr_t) x)
#define dbginterface(x, y) printf_s("%32s -> %8x\n", x, (uintptr_t) x)
#endif

struct vector {
    float x, y, z;
};

// Vmt hook class and other utils

class vmt_hook {
    bool enabled_;
    uintptr_t** base_class_;
    uintptr_t* old_vtable;
    uintptr_t* new_vtable;

public:
    vmt_hook(uintptr_t base_class, size_t size, std::initializer_list<std::pair<size_t, void*>> detours)
        : enabled_(false), base_class_((uintptr_t**) base_class), 
        new_vtable((uintptr_t*) malloc(size)),
        old_vtable(*base_class_) 
    {
        memcpy(new_vtable, old_vtable, size);
        for (auto& p : detours)
            new_vtable[p.first] = (uintptr_t) p.second;
    }
    ~vmt_hook() {}

    bool enabled() const {
        return enabled_;
    }
    void Enable() {
        enabled_     = true;
        *base_class_ = new_vtable;
    }
    void Disable() {
        enabled_     = false;
        *base_class_ = old_vtable;
    }
    uintptr_t GetOriginal(size_t n) const {
        return old_vtable[n];
    }
};


inline uintptr_t find_pattern(uintptr_t addr, size_t scan_len, const char* pattern, size_t offs = 0) {
    uintptr_t end = addr + scan_len;
    while (addr < end) {
        bool hit        = true;
        const char* c   = pattern;
        uintptr_t addr_ = addr;
        while (*c && hit) {
            char code = *c;
            char len  = code & 0x7F;
            c++;
            if (code & 0x80) {
                hit = !memcmp((const char*) addr_, c, len);
                c += len;
            }
            addr_ += len;
        }
        if (hit)
            return addr + offs;
        addr++;
    }
    return 0;
}

inline uintptr_t get_function_by_call(uintptr_t addr, size_t scan_len, const char* pattern)
{
    uintptr_t start_addr = (uintptr_t) find_pattern(addr, scan_len, pattern);
    uint32_t relative_offset = *(uint32_t*) (start_addr + 1);
    return (start_addr + relative_offset + 5);
}

template <typename T>
inline T get_proc_address(uintptr_t mod, const char* s) {
    return (T) GetProcAddress((HMODULE) mod, s);
}
inline uintptr_t get_module_handle(const char* s) {
    uintptr_t module_ = (uintptr_t) GetModuleHandleA(s);
    if (!module_) {
        MessageBox(NULL, L"LoadInterfaces( ) failed! Check dll modules.", L"cheat::LoadInterfaces( )", NULL);
        return 0;
    }
    dbgprint("%32s => %8x\n", s, module_); 
    return module_;
}

template <typename T1, typename T2>
inline T1 GetVFunction(T2* p, size_t index) {
    return (T1)((*(uintptr_t**) p)[index]);
}

template <typename F>
inline F GetInterfaceFromDll(uintptr_t dll, const char* interface_name) {
    typedef void*(__cdecl * GetInterface)(const char* s, int* a);
    GetInterface i = get_proc_address<GetInterface>(dll, "CreateInterface");
    return ((F) i(interface_name, nullptr));
}

typedef void* (*InstantiateInterfaceFn)();
struct InterfaceReg {
    InstantiateInterfaceFn m_CreateFn;
    const char* m_pName;
    InterfaceReg* m_pNext;
};

template <typename IT>
inline IT GetInterfacePtr(const char* name, InterfaceReg* interface_list) {

    for (InterfaceReg* current = interface_list; current; current = current->m_pNext) 
    {
        if (!strcmp(name, current->m_pName)) {
            dbginterface(current->m_pName, current->m_CreateFn());
            return (IT) current->m_CreateFn();
        }
    }
    printf("! Interface \"%s\" not found :(\n", name);
}
