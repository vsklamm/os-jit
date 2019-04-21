#include <sys/mman.h>
#include <unistd.h>

#include "function_wrapper.h"

function_wrapper::function_wrapper(const machine_code &compiled)
{
    init_page_size();
    func_mem = compiled.size();
    mapped_mem = calc_mapped_size(func_mem);

    ptr_mem = alloc_mem();
    store_code(ptr_mem, compiled);
    protect(ptr_mem, PROT_READ | PROT_EXEC);

    f = std::function<void(void)>(reinterpret_cast<void (*)()>(ptr_mem));
}

function_wrapper::~function_wrapper()
{
    munmap(ptr_mem, mapped_mem);
}

void function_wrapper::run()
{
    f();
}

void function_wrapper::store_patch(std::size_t position, const machine_code &s)
{
    protect(ptr_mem, PROT_READ | PROT_WRITE);
    auto new_mapped_mem = calc_mapped_size(position + s.size());
    auto new_ptr = (uint8_t *) mremap(ptr_mem, mapped_mem, new_mapped_mem, MREMAP_MAYMOVE);
    if (new_ptr == MAP_FAILED)
    {
        throw std::runtime_error("Can't reallocate memory. ");
    }
    ptr_mem = new_ptr;
    std::copy(s.begin(), s.end(), ptr_mem + position);
    protect(ptr_mem, PROT_READ | PROT_EXEC);
    f = std::function<void(void)>(reinterpret_cast<void (*)()>(ptr_mem));
}

void function_wrapper::init_page_size()
{
    const auto psize = sysconf(_SC_PAGE_SIZE);
    PAGE_SIZE = psize == (-1) ? 4096 : psize;
}

std::size_t function_wrapper::calc_mapped_size(std::size_t sz)
{
     return (sz == 0 || sz % PAGE_SIZE)? (sz / PAGE_SIZE + 1) * PAGE_SIZE : sz;
}

uint8_t *function_wrapper::alloc_mem()
{
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_ANONYMOUS | MAP_PRIVATE;
    auto mem = (uint8_t *) mmap(NULL, mapped_mem, prot, flags, -1, 0);
    if (mem == MAP_FAILED)
    {
        throw std::runtime_error("Can't allocate memory. ");
    }
    return mem;
}

void function_wrapper::store_code(uint8_t *mapped_mem, const machine_code &compiled)
{
    std::copy(compiled.begin(), compiled.end(), mapped_mem);
}

void function_wrapper::protect(void *mapped_mem, int prot)
{
    const auto res_code = mprotect(mapped_mem, func_mem, prot);
    if (res_code != 0)
    {
         throw std::runtime_error("Can't set protection on a region of memory. ");
    }
}
