#include "console.h"
#define entry_num 1024

static inline void lcr3(unsigned int val)
{   
  asm volatile("movl %0,%%cr3" : : "r" (val));
}

static inline void halt(void)
{
    asm volatile("hlt" : : );
}

void printHex(unsigned int addr) {
    char* s = "00000000000000000000000000000000";
    for (int i = 0; i < sizeof(unsigned int); i++) {
        int num = (1 << (sizeof(unsigned int) - i - 1)) & addr;
        if (num) {
            s[31 - i] = '1';
        } else {
            s[31 - i] = '0';
        }
    }
    printk(s);
}

struct PT {
  unsigned int* mappings;   
};

struct PTD {
    struct PT entries[1024];
} ptd;


unsigned int maps[2][1024] = {0};

int main(void)
{
    int i; 
    int sum = 0;

    // Initialize the console
    uartinit(); 
    printk("Hello from C\n");

    // Create your page table here
    for (int j = 0; j < 2; j++) {
        ptd.entries[j].mappings = (unsigned int*) &maps[j];
        for (int k = 0; k < 1024; k++) {
            maps[j][k] = (4096 * k + 4096 * 1024 * j);
        }
    }
    printHex((unsigned int)&ptd);
    /*lcr3();*/
    for (i = 0; i < 32 /*64*/; i++) {
        int *p = (int *)(i * 4096 * 32);
        sum += *p; 
        /*printk("page\n"); */
    }
    halt(); 
    return sum; 
}


