#ifndef STACK_ALLOCATOR_H
#define STACK_ALLOCATOR_H

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <ctype.h>
#include <stack>

class StackAllocator
{
public:

    StackAllocator(std::size_t size);

    ~StackAllocator();

    void clear();

    uint64_t* allocate(std::size_t size);

    void deallocate(uint64_t* p);

    std::size_t getSize() const;

    uint64_t* getHead() const;

    uint64_t* getMemory() const;

private:
    uint64_t*    mp_head = nullptr;
    uint64_t*    mp_memory = nullptr;
    uint64_t*    mp_end = nullptr;
    std::stack<uint64_t*> m_stack;
};

#endif // STACK_ALLOCATOR_H