#ifndef BRAIN_COMPILER_H
#define BRAIN_COMPILER_H

#include <string>

#include "brain_asm.h"
#include "function_wrapper.h"

struct hello_compiler
{
public:
    hello_compiler();

    function_wrapper build(const std::string &s = "");
    void patch(function_wrapper &f, const std::string &s, std::size_t position = 0);

private:
    machine_code compile(const std::string &s);
    void put_msg_size(machine_code &code, std::size_t size);
    uint32_t get_msd_size(function_wrapper &f);
    void print_error(const std::string &err, bool with_errno);

};

#endif // BRAIN_COMPILER_H
