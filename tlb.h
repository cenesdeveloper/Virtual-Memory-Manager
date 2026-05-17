#ifndef TLB_H
#define TLB_H

#include "config.h"

typedef struct {
    int page_number;
    int frame_number;
} TLBEntry;

void init_tlb(void);
int search_tlb(int page_number);
void add_tlb_entry(int page_number, int frame_number);
int update_tlb_entry(int old_page, int new_page, int frame_number);

#endif
