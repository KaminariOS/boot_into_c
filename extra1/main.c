#include "console.h"

#define TABLE_SIZE 1024

extern int ptd_table[TABLE_SIZE] asm("ptd_table");
extern int pt_table[TABLE_SIZE] asm("pt_table");


static inline void lcr3(unsigned int val)
{   
  asm volatile("movl %0,%%cr3" : : "r" (val));
}

static inline void halt(void)
{
    asm volatile("hlt" : : );
}

unsigned int pt_tables[TABLE_SIZE * 64];
unsigned int ptd_tables[TABLE_SIZE];

int main(void)
{
    int i; 
    int sum = 0;

    // Initialize the console
    uartinit(); 

    printk("Hello from C\n");
//    int* pd = ptd_table;

    for (unsigned int k = 0; k < 1; k++){
	    for (unsigned int j = 0; j < TABLE_SIZE; j++){
		  pt_tables[k * TABLE_SIZE + j] = ((k * TABLE_SIZE + j) * 4096) | 0b11;  
		  //pt_tables[k * TABLE_SIZE + j] = pt_table[j];
		  if (pt_tables[k * TABLE_SIZE + j] != pt_table[j]){
				  printk("not the same\n");
		} else {
				  //printk("the same\n");
		}
	    }
    }
    for (unsigned int k = 0; k < 64; k++){
	    ptd_tables[k] = (unsigned int) ( pt_tables + k * TABLE_SIZE) | 0x3;
	    if (k == 0){
		   // continue;
	    }
	    for (int j = 0; j < TABLE_SIZE; j++){
		  pt_tables[k * TABLE_SIZE + j] = ((k * TABLE_SIZE + j) * 4096) | 0b11;  
	    }
    }
    // Create your page table here
    printk("loading \n");
    lcr3((unsigned int) ptd_tables);

    for (i = 0; i < 32 /*64*/; i++) {
        int *p = (int *)(i * 4096 * 32);
        sum += *p; 
                
        printk("page\n"); 
    }
    for (int m = 0; m < 32; m++){
	    int flag = 1 << (31 - m);
	    if (flag & sum){
		   printk("1");
	    }else {
		   printk("0");
	    }
    }
    printk("\n");
    halt(); 
    return sum; 
}


