#include "tlb.h"

static TLBEntry tlb[TLB_SIZE];
static int next_tlb_index = 0;

void init_tlb(void) {
    for (int i = 0; i < TLB_SIZE; i++) {
        tlb[i].page_number = -1;
        tlb[i].frame_number = -1;
    }

    next_tlb_index = 0;
}

int search_tlb(int page_number) {
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb[i].page_number == page_number) {
            return tlb[i].frame_number;
        }
    }

    return -1;
}

void add_tlb_entry(int page_number, int frame_number) {
    tlb[next_tlb_index].page_number = page_number;
    tlb[next_tlb_index].frame_number = frame_number;

    next_tlb_index = (next_tlb_index + 1) % TLB_SIZE;
}

int update_tlb_entry(int old_page, int new_page, int frame_number) {
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb[i].page_number == old_page) {
            tlb[i].page_number = new_page;
            tlb[i].frame_number = frame_number;
            return 1;
        }
    }

    return 0;
}
