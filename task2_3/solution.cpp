#include <iostream>
#include <set>
#include <cstring>

struct memspan 
{
    uintptr_t start;
    size_t size;
    memspan(uintptr_t start, size_t size):
        start(start), size(size)
    { };    
};

struct memspan_len_cmp
{
    bool operator() (const memspan &m1, const memspan &m2) const
    {
        return m1.size < m2.size || m1.start < m2.start;
    }
};

struct memspan_addr_cmp
{
    bool operator() (const memspan &m1, const memspan &m2) const
    {
        return m1.start < m2.start;
    }
};

class SmallAllocator {
private:
    const static unsigned int MEMSIZE = 1048576;
    char Memory[MEMSIZE];
    std::set<memspan, memspan_len_cmp> free_spans;
    std::set<memspan, memspan_addr_cmp> allocated_spans;
public:
    SmallAllocator()
    {
        free_spans.insert(memspan((uintptr_t) Memory, MEMSIZE));
    }
    void *Alloc(unsigned int size)
    {        
        for (auto i = free_spans.begin(); i != free_spans.end(); i++)
        {
            if ((*i).size >= size)
            {
                allocated_spans.insert(memspan((*i).start, size));
                if ((*i).size > size)
                    free_spans.insert(memspan((*i).start + size, (*i).size - size));
                void *result = (void *) (*i).start;
                free_spans.erase(i);
                return result;
            }
        }
        return nullptr; 
    };
    
    void *ReAlloc(void *pointer, unsigned int size)
    {
        if (pointer == nullptr) return nullptr;
        auto i = allocated_spans.find(memspan((uintptr_t) pointer, 0));
        if (i == allocated_spans.end()) return nullptr;
        size_t old_size = (*i).size; 
        void *new_pointer = Alloc(size);
        memcpy(new_pointer, pointer, old_size);
        Free(pointer);
        return new_pointer;    
    };
    
    void Free(void *pointer) 
    {
        if (pointer == nullptr) return;
        auto i = allocated_spans.find(memspan((uintptr_t) pointer, 0));
        if (i == allocated_spans.end()) return;
        free_spans.insert((*i));
        allocated_spans.erase(i);
    };
    
    size_t freememlen()
    {
        size_t r = 0;
        for (auto i = free_spans.begin(); i != free_spans.end(); i++)
        {
            r += (*i).size;
        }
        return r;
    }
};


int main()
{
    SmallAllocator A1;
    int * A1_P1 = (int *) A1.Alloc(sizeof(int));
    A1_P1 = (int *) A1.ReAlloc(A1_P1, 2 * sizeof(int));
    A1.Free(A1_P1);
    SmallAllocator A2;
    int * A2_P1 = (int *) A2.Alloc(10 * sizeof(int));
    for(unsigned int i = 0; i < 10; i++) A2_P1[i] = i;
    for(unsigned int i = 0; i < 10; i++) if(A2_P1[i] != i) std::cout << "ERROR 1" << std::endl;
    int * A2_P2 = (int *) A2.Alloc(10 * sizeof(int));
    for(unsigned int i = 0; i < 10; i++) A2_P2[i] = -1;
    for(unsigned int i = 0; i < 10; i++) if(A2_P1[i] != i) std::cout << "ERROR 2" << std::endl;
    for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 3" << std::endl;
    A2_P1 = (int *) A2.ReAlloc(A2_P1, 20 * sizeof(int));
    for(unsigned int i = 10; i < 20; i++) A2_P1[i] = i;
    for(unsigned int i = 0; i < 20; i++) if(A2_P1[i] != i) std::cout << "ERROR 4" << std::endl;
    for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 5" << std::endl;
    A2_P1 = (int *) A2.ReAlloc(A2_P1, 5 * sizeof(int));
    for(unsigned int i = 0; i < 5; i++) if(A2_P1[i] != i) std::cout << "ERROR 6" << std::endl;
    for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 7" << std::endl;
    A2.Free(A2_P1);
    A2.Free(A2_P2);
    return 0;
}