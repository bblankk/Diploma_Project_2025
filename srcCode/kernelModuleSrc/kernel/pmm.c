//PHYSICAL MEMORY ALLOCATOR, ALSO CALLED PMM (PHYSICAL MEMORY MANAGER) - MAIN FUNCTIONALITY LIVES HERE <22
#include <stddef.h>
#include <stdint.h>

//define rom (read only memory) and dont touch it
// read only memory is a small block of code on the motherboard in some chip you might recognize as the BIOS chip, and it's commonly known for storing BIOS and other untouchable firmware. What's on there is decided by the manufacturer and NONE of our business, we won't be touching it.

// global memory  (.bss stuff) - a chunk that's allocated at program startup and not changed

//lexically allocated memory - memory which is automatically allocated in a block when a function begins and deallocated when it returns. It can be seen as an area of 'scratch memory' which the program can use for temporary values whose lifespan is determined by the running time of the function which is using it, and can always be automatically reclaimed for reuse after the function exits.

//dynamically allocated memory - hardest part here, beware memory holes and implement all the thread management stuff C will need later on.

// side quest - garbage collector  ( GC strat )

//
void initialize_Physical_Memory_Allocator(uint32_t totalMemorySize, uint32_t kernelEndAddress);
void* allocate_Physical_Memory(size_t size);
void free_Physical_Memory(void* address, size_t size);