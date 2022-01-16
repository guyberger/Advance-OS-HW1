# Advanced OS - HW3 (individual)
## Q1
KVM is a type II hypervisor written as a Linux kernel module, meaning it virtualizes the hardware. It essentially allows the kernel to act as a hypervisor.
It exposes /dev/kvm interface.
It uses QEMU to emulate hardware for the guest.
### User mode
Used to set up guest VM's address space.
Simulates I/O operations for the guest.
Calls ioctl to the KVM kernel to start/resume the guest.
### Kernel mode
Enters non-root mode to run the guest.
Upon receiving trap from guest the kernel tries to handle it and to call vmresume after.
If it can't handle the exception (if it is I/O related or handle signals) it moves to user mode to handle it.
### non-root mode
Guest VM runs in this mode.
### Trap and emulate
Trap lead to vmexit, passes control to root mode - kvm kernel. According to the exit condition the kernel then call the appropriate handler.
If the handler can't be handled inside the kernel, the kvm fetches the instructions from the guest virtual address (using some calculations), like MMU, decodes the instruction, like CPU, then tries to execute the command: The kvm emulates the command execusion.

## Q2
### (a)
If the guest runs out of memory, it swaps pages to the host swap area, which contains all the guests' swapped pages.
When the guest virtual cpu accesses guest memory swapped out by the guest, the host can then restore the page from the guests swapped area.

### (b)
When the host needs more memory it can swap pages, including guest VMs pages out to have more available memory.
When the guest tries to access memory swapped out by the host - its execution is suspended until memory is swapped back.

## Q3
### (a)
Pros
- Reduces the need for VMM to keep syncing shadow pages, less overhead.
- Less page faults.
- Reduce TLB flushes.

Cons
- Longer translations for every memory access

### (b)
In L3 micro-kernel tags are used for addresses in the TLB for the CPU to determine which process has the target HW address mapped, thus reducing the need to flush the TLB on each context switch. In the "turtles project" using EPT to translate guest virtual -> guest physical -> host physical as a third layer of translation reduces the number of TLB flushes as well because upon vm switch (vmexit) the TLB is not flushed since we have tags of translations to each VM!
