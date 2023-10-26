# Project 4: Virtual Memory Fun
Our goal in this project is to explore page tables and their use.

## Resources
- The xv6-riscv version of the xv6 book may be useful here, especially looking at the Page Tables chapter
- Within the xv6-riscv kernel code, the key files will be
- memlayout.h describing the memory layout
- vm.c which implements (most of) the virtual memory code
- kalloc.c which implements allocation and freeing of physical memory
- I'll just note that MIT also suggests their students refer to the RISC-V privileged architecture manual
## Big Ol' Note on Rollout Of the Assignment
My big self-learning of Project 3 was that I really need to write all the parts before assigning (which is to say, I really didn't think getting a verified test of the code working was going to be that hard :-)). I won't necessarily still know all the big stumbles, but I'll try to reduce those. As a result, I'm rolling out this assignment in some pieces, though likely completely out by the 23rd.

## Part 1: What does a page table in RISC-V look like?
Define a function called vmprint(). It should take a pagetable_t argument, and print that pagetable in the format described below. Insert if(p->pid==1) vmprint(p->pagetable) in exec.c just before the return argc (line 132), to print the first process's page table.

Your output should look something like this:
```
page table 0x0000000087f6b000
 ..0: pte 0x0000000021fd9c01 pa 0x0000000087f67000
 .. ..0: pte 0x0000000021fd9801 pa 0x0000000087f66000
 .. .. ..0: pte 0x0000000021fda01b pa 0x0000000087f68000
 .. .. ..1: pte 0x0000000021fd9417 pa 0x0000000087f65000
 .. .. ..2: pte 0x0000000021fd9007 pa 0x0000000087f64000
 .. .. ..3: pte 0x0000000021fd8c17 pa 0x0000000087f63000
 ..255: pte 0x0000000021fda801 pa 0x0000000087f6a000
 .. ..511: pte 0x0000000021fda401 pa 0x0000000087f69000
 .. .. ..509: pte 0x0000000021fdcc13 pa 0x0000000087f73000
 .. .. ..510: pte 0x0000000021fdd007 pa 0x0000000087f74000
 .. .. ..511: pte 0x0000000020001c0b pa 0x0000000080007000
init: starting sh
```
The first line displays the argument to vmprint. After that there is a line for each PTE, including PTEs that refer to page-table pages deeper in the tree. Each PTE line is indented by a number of " .." that indicates its depth in the tree. Each PTE line shows the PTE index in its page-table page, the pte bits, and the physical address extracted from the PTE. Don't print PTEs that are not valid. In the above example, the top-level page-table page has mappings for entries 0 and 255. The next level down for entry 0 has only index 0 mapped, and the bottom-level for that index 0 has entries 0, 1, and 2 mapped.

Your code might emit different physical addresses than those shown above. The number of entries and the virtual addresses should be the same.

#### Part 1 details
- place vmprint in kernel/vm.c; define the prototype for it in kernel/defs.h (which is included in exec.c)
- Use the macros in riscv.h to get the indices in the page table (and you'll want to look at the xv6-riscv book to understand the page table scheme)
- The big hint is that the function freewalk in vm.c "may be inspirational" (don't free the pages, but think about what free walk does...)
- %p is used in printf to print out the full 64-bit hex PTEs and addresses shown above
## Part 2: Detect which pages have been accessed
Some garbage collectors (a form of automatic memory management) can benefit from information about which pages have been accessed (read or write). You will add a new feature to xv6 that detects and reports this information to userspace by inspecting the access bits in the RISC-V page table. The RISC-V hardware page walker marks these bits in the PTE whenever it resolves a TLB miss.

Your job is to implement pgaccess(), a system call that reports which pages have been accessed. The system call takes three arguments. First, it takes the starting virtual address of the first user page to check. Second, it takes the number of pages to check. Finally, it takes a user address to a buffer to store the results into a bitmask (this is a data structure that uses one bit per page and where the first page corresponds to the least significant bit).
#### Part 2 details:
- See the example program below to determine the prototype for pgaccess.
- Start by implementing sys_pgaccess() in kernel/sysproc.c. (I put the prototype declaration of pgaccess in defs.h like other system calls.)
- You'll need to parse arguments using argaddr() and argint() -- like the getpinfo portion of the last project
- Place pgaccess in kernel/vm.c; define the prototype for it in kernel/defs.h (which is included in vm.c)
- For the output bitmask, it's easier to store a temporary buffer in the kernel and copy it to the user (via copyout()) after filling it with the right bits (again, like getpinfo in the last project).
- It's okay to set an upper limit on the number of pages that can be scanned (this lets you know how big your bitmask can be)
- `walk()` in `kernel/vm.c` is very useful for finding the right PTEs.
- You'll need to define PTE_A, the access bit, in `kernel/riscv.h`. Consult the xv6 book, in the chapter on Page Tables, to determine its value.
- Be sure to clear PTE_A after checking if it is set. Otherwise, it won't be possible to determine if the page was accessed since the last time `pgaccess()` was called (i.e., the bit will be set forever).
- `vmprint()` may come in handy to debug page tables.
- My personal order of work on this was to build the pgaccess function first, which of course did not show any pages accessed, and then added the code to cause a memory access to set the bit.
- bitwise operators:
1. The operator & is useful to test for a bit, e.g. pte & PTE_V
2. The operator | is useful to set a bit, e.g., bMap = bMap | PTE_V
3. The operator ^ (xor) is useful to unset a bit, e.g., pte = pte ^ PTE_V
- Here is a simplistic tester that should show every page was accessed:
```
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h" // defines PGSIZE
#include "user/user.h"

int main(int argc, char *argv[])
{
    char *pageTester;
    int accessBitmap = 0;
    pageTester = malloc(sizeof(int)*8*PGSIZE);
    for (int i = 0; i < sizeof(int)*8; ++i)
    {
        pageTester[i*PGSIZE]=1; // accessed
    }
    if (pgaccess(pageTester,sizeof(int),&accessBitmap) < 0)
    {
        printf("error in pgaccess\n");
    }
    else
    {
        for (int i =0; i < sizeof(int)*8; ++i)
        {
            printf("Page %d, accessed: %d\n",
                i, (accessBitmap & (1<<i))!=0);
        }
    }
    return 0;
}
```
