#ifndef MEMORY_H
#define MEMORY_H

#include "config.h"

void init_memory(void);
int get_frame_from_page_table(int page_number);
int load_page_into_memory(int page_number, signed char *backing_store);
signed char get_value_at_physical_address(int physical_address);

extern int page_faults;
extern int page_replacements;

#endif
