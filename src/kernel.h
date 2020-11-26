// mykernel.h - Header file for the mykernel.cpp main kernel source file.

#ifndef __KERNEL_H
#define __KERNEL_H

typedef void (*constructor)();

extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

extern "C" void callConstructors();

void kprint(char* s);

#endif 
