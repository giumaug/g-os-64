#ifndef GENERAL_H                
#define GENERAL_H

#define NUM_CPU 4
#define CPU_0 0
#define CPU_1 1
#define CPU_2 2
#define CPU_3 3
#define CPU_4 4
#define CPU_5 5
#define CPU_6 6
#define CPU_7 7
#define CPU_8 8
#define CPU_9 9
#define CPU_10 10
#define CPU_11 11
#define CPU_12 12
#define CPU_13 13
#define CPU_14 14
#define CPU_15 15
#define CPU_16 16
#define CPU_17 17
#define CPU_18 18
#define CPU_19 19
#define CPU_20 20
#define CPU_21 21
#define CPU_22 22
#define CPU_23 23
#define CPU_24 24
#define CPU_25 25
#define CPU_26 26
#define CPU_27 27
#define CPU_28 28
#define CPU_29 29
#define CPU_30 30
#define CPU_31 31

#ifdef DEBUG 
	#define PRINTK(...) printk(__VA_ARGS__); 
#else
	#define PRINTK(...)
#endif

#endif


