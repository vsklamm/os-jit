#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
#include <functional>

#include "compiler.h"

hello_compiler::hello_compiler()
{
}

function_wrapper hello_compiler::build(const std::string &s)
{
    try
    {
        const auto compiled = compile(s);
        return function_wrapper(compiled);
    }
    catch (std::runtime_error e)
    {
        print_error(e.what(), true);
        return function_wrapper({0xc3}); // ret function
    }
}

void hello_compiler::patch(function_wrapper &f, const std::string &s, std::size_t position)
{
    machine_code sz;
    machine_code code(s.size());
    auto caster = [](auto& c){ return static_cast<uint8_t>(c); };
    std::transform(s.begin(), s.end(), code.begin(), caster);

    auto old_size = get_msd_size(f);
    le_size_t les = { std::max(old_size, uint32_t(position + s.size())) };
    sz.assign(les.bytes, les.bytes + 4);
    f.store_patch(msg_size_pos, sz);
    f.store_patch(msg_pos + position, code);
}

machine_code hello_compiler::compile(const std::string &s)
{
    machine_code code;
    code.insert(code.end(), create_stack_frame.begin(), create_stack_frame.end());
    code.insert(code.end(), write_message.begin(), write_message.end());
    code.insert(code.end(), restore_stack.begin(), restore_stack.end());

    put_msg_size(code, s.size());
    auto caster = [](auto& c){ return static_cast<uint8_t>(c); };
    std::transform(s.begin(), s.end(), std::back_inserter(code), caster);
    return code;
}

void hello_compiler::put_msg_size(machine_code &code, std::size_t size)
{
    le_size_t sz = { uint32_t(size) };
    std::copy(sz.bytes, sz.bytes + 4, code.begin() + msg_size_pos);}

uint32_t hello_compiler::get_msd_size(function_wrapper &f)
{
    le_size_t les = { 0 };
    std::copy(f.ptr_mem + msg_size_pos, f.ptr_mem + msg_size_pos + 4, les.bytes);
    return les.num;
}

void hello_compiler::print_error(const std::string &err = "", bool with_errno = true)
{
    std::cerr << "compiler: " << err << (with_errno ? strerror(errno) : "") << std::endl;
}
