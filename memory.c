#include <string.h>

#include "memory.h"
#include "tlb.h"

static int page_table[PAGES];
static signed char physical_memory[PHYSICAL_ADDRESS_SPACE];
static int frame_to_page[FRAMES];

static int next_free_frame = 0;
static int oldest_frame = 0;

int page_faults = 0;
int page_replacements = 0;

void init_memory(void) {
    for (int i = 0; i < PAGES; i++) {
        page_table[i] = -1;
    }

    for (int i = 0; i < FRAMES; i++) {
        frame_to_page[i] = -1;
    }

    memset(physical_memory, 0, sizeof(physical_memory));

    next_free_frame = 0;
    oldest_frame = 0;
    page_faults = 0;
    page_replacements = 0;
}

int get_frame_from_page_table(int page_number) {
    return page_table[page_number];
}

int load_page_into_memory(int page_number, signed char *backing_store) {
    int frame_number;

    if (next_free_frame < FRAMES) {
        frame_number = next_free_frame;
        next_free_frame++;
    } else {
        frame_number = oldest_frame;

        int old_page = frame_to_page[frame_number];
        page_table[old_page] = -1;

        if (!update_tlb_entry(old_page, page_number, frame_number)) {
            add_tlb_entry(page_number, frame_number);
        }

        oldest_frame = (oldest_frame + 1) % FRAMES;
        page_replacements++;
    }

    memcpy(
        physical_memory + frame_number * PAGE_SIZE,
        backing_store + page_number * PAGE_SIZE,
        PAGE_SIZE
    );

    page_table[page_number] = frame_number;
    frame_to_page[frame_number] = page_number;
    page_faults++;

    return frame_number;
}

signed char get_value_at_physical_address(int physical_address) {
    return physical_memory[physical_address];
}
