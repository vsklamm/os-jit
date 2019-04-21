#ifndef HELLO_ASM_H
#define HELLO_ASM_H

#include <memory>
#include <variant>
#include <vector>

using machine_code = std::vector<uint8_t>;

union le_size_t
{
    uint32_t num;
    uint8_t bytes[4];
};

/*
    RDI, RSI, RCX, RDX, R8, R9 are used for integer and pointer arguments in that order left to right.

    Registers RAX, RCX, RDX, R8, R9, R10, and R11 are considered volatile and must be considered destroyed on function calls.
    RBX, RBP, R12, R14, R14, and R15 must be saved in any function using them.
*/
const machine_code create_stack_frame
{
    0x55,                                          // push      rbp
    0x48, 0x89, 0xe5,                              // mov       rbp, rsp
};

const machine_code restore_stack
{
    0x5d,                                           // pop      rbp
    0xc3                                            // ret
};

const std::size_t msg_size_pos = 28;
const std::size_t msg_pos = 36;

const machine_code write_message
{
    0x48, 0xc7, 0xc0, 0x01, 0x00, 0x00, 0x00,       // mov      rax, 0x01         // write system call
    0x48, 0xc7, 0xc7, 0x01, 0x00, 0x00, 0x00,       // mov      rdi, 0x01         // stdin descriptor
    0x48, 0x8d, 0x35, 0x0b, 0x00, 0x00, 0x00,       // leaq     rsi, [rip + 0x0a] // message place
    0x48, 0xc7, 0xc2, 0x00, 0x00, 0x00, 0x00,       // movq     rdx, 0x00         // initial size of string
    0x0f, 0x05,                                     // syscall
};

#endif // HELLO_ASM_H
