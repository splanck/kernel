// mykernel.h - Header file for the mykernel.cpp main kernel source file.

#ifndef __MYKERNEL_H
#define __MYKERNEL_H

typedef void (*constructor)();

extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

extern "C" void callConstructors();

void kprint(char* s);

#endif 
