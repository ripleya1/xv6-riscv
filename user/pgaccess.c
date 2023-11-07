#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h" // defines PGSIZE
#include "user/user.h"

// tester for pgaccess
int main(int argc, char *argv[])
{
    char *pageTester;
    int accessBitmap = 0;
    pageTester = malloc(sizeof(int)*8*PGSIZE);
    for (int i = 0; i < sizeof(int)*8; ++i)
    {
        pageTester[i*PGSIZE]=1; // accessed
    }
    if (pgaccess(pageTester,sizeof(int) * 8,&accessBitmap) < 0)
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