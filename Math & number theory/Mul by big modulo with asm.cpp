//Works with all uint64_t modulo's.
//                                         RCX,      RDX,       R8 
__attribute__((naked)) uint64_t mulmod64(uint64_t, uint64_t, uint64_t) 
{
    asm(R"(
        .intel_syntax noprefix
        mov rax, rcx
        mul rdx
        div r8
        mov rax, rdx
        ret
        .att_syntax noprefix
    )");
}
