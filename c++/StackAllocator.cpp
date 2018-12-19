#include <cassert>
#include <stdexcept>
#include <iostream>

#include "StackAllocator.h"

StackAllocator::StackAllocator(std::size_t size)
{
    assert (size != 0);

    std::cout << "StackAllocator::StackAllocator()" << std::endl;

    mp_memory = new uint64_t[size];
    mp_end = mp_memory + size;
    mp_head = mp_memory;
}

StackAllocator::~StackAllocator()
{
    std::cout << "StackAllocator::~StackAllocator()" << std::endl;
    delete[] mp_memory;
}

void StackAllocator::clear()
{
    mp_head = 0;
    while (!m_stack.empty() )
    {
        m_stack.pop();
    }    
}

uint64_t* StackAllocator::allocate(std::size_t size)
{
    assert (size != 0);
    assert (mp_head + size < mp_end);
    assert (mp_memory);
    if (mp_head + size >= mp_end || !mp_memory || size == 0)
    {
        throw std::bad_alloc();
    }

    uint64_t* pointer = mp_head;
    m_stack.push(pointer);
    mp_head += size;

    std::cout << "Allocated:   " << pointer << " : " << size << std::endl;

    return pointer;
}

void StackAllocator::deallocate(uint64_t* p)
{
    std::cout << "Deallocating: " << p <<  std::endl;
    assert (mp_memory);

    if (!mp_memory)
    {
        throw std::invalid_argument("deallocate");
    }
    uint64_t* pointer = m_stack.top();
    assert(p == pointer);
    m_stack.pop();
    mp_head = pointer;

    std::cout << "Deallocating: " << p <<  std::endl;
}

std::size_t StackAllocator::getSize() const
{
    return mp_head - mp_memory;
}

uint64_t* StackAllocator::getHead() const
{
    return mp_head;
}

uint64_t* StackAllocator::getMemory() const
{
    return mp_memory;
}
