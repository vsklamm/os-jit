#ifndef FUNCTION_WRAPPER_H
#define FUNCTION_WRAPPER_H

#include <functional>
#include <memory>
#include <cstddef>
#include <vector>

using machine_code = std::vector<uint8_t>;

class function_wrapper
{
public:

    function_wrapper() = delete;
    function_wrapper(const function_wrapper &compiled) = delete;
    function_wrapper(const machine_code &compiled);
    ~function_wrapper();

    void run();
    void store_patch(std::size_t i, const machine_code &s);

private:

    void init_page_size();
    std::size_t calc_mapped_size(std::size_t sz);

    uint8_t * alloc_mem();
    void store_code(uint8_t * mapped_mem, const machine_code &compiled);
    void protect(void * mapped_mem, int prot);

    std::size_t PAGE_SIZE;

    std::size_t func_mem;
    std::size_t mapped_mem;
    uint8_t * ptr_mem;
    std::function<void()> f;

    friend class hello_compiler;
};

#endif // FUNCTION_WRAPPER_H
