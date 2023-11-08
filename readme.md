Files modified include:
- [exec.c](kernel/exec.c)
    - prints the first proc's pagetable using `vmprint()`
- [sysproc.c](kernel/sysproc.c)
    - pointer copying, and getting the pagetable from the current proc and sending that as an argument for pgaccess
- [vm.c](kernel/vm.c)
    - setting the access bit - this happens twice: once in `walk()` and once in `mappages()`, ie when a page is accessed
    - `vmprint()`, based off of `freewalk()`, for each valid pagetable entry in a pagetable, prints the depth of the PTE, the index, the bits, and the physical address
    - `pgaccess()`, checks for the access bit (PTE_A) for each page in a pagetable, using `walk` to find the appropriate PTE, returns a bitmask in which the value of each bit indicates the value of the access bit of the correspondingly numbered page
- [pgaccess.c](user/pgaccess.c)
    - test file
- [riscv.h](kernel/riscv.h)
    - defines the access bit (PTE_A) as a long set to 1 shifted to the left 6 times (this is based on what the xv6 book says)

Additionally, the necessary header files and makefiles for all of these were modified with the new functions to get xv6 to compile. 

`vmprint()` should run when xv6 is run initially. `pgaccess()` can be tested by executing `pgaccess`.