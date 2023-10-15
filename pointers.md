So the basic issue when trying to pass data from the kernel back to the user is exactly that processes each have their own page tables and memory addresses. Proc.c already has a function written to handle this.
## User side of things

Suppose you create a struct pstat variable: `struct pstat pinfo;`

You want to copy the data from the kernel into that pinfo object, so you call `getpinfo(&pinfo)`

getpinfo is a system call so control now perhaps transfers to `sys_getpinfo` in the kernel.
## Kernel side of things

sys_getpinfo needs to pull the address out. It does this using argaddr, e.g.,
```
struct pstat *p; 
argaddr(0, (uint64) p); 
return getpinfo(p);
```

The interesting part is then in getpinfo where you gather all your data. What you'd like to do is something like
```
for (int i; i < NPROCS; ++i)
 {
   p->inuse[i] = proc[i].state != UNUSED;
   // etc.. 
 }
```    

Alas, this does not work because p is an address in the user space and the system call is run by the kernel.

Instead you use the proc.c function either_copyout. In general the function works like this:
```
int pretendSysCall(int *pFromUser)
{
   int localVal = 5;
   either_copyout(1,(uint64)pFromUser, &localVal, sizeof(int));
   // rather than *pFromUser=localVal
}
```
For this function, I recommend you do something like

 
```
int getpinfo(struct pstat *pFromUser)
{
  struct pstat pinfo; // local to kernel
  for (int i; i < NPROCS; ++i)
   {
     pinfo.inuse[i] = (proc[i].state != UNUSED);
     // etc.. 
   }
 // now copy to user space
  either_copyout(1, (uint64) pFromUser, &pinfo, sizeof(struct pstat));
  return 0;
 }
```