#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

#include "config.h"
#include "tlb.h"
#include "memory.h"

int main(int argc, char *argv[]) {
    const char *addresses_path = "addresses.txt";
    const char *backing_store_path = "BACKING_STORE.bin";

    if (argc == 3) {
        addresses_path = argv[1];
        backing_store_path = argv[2];
    } else if (argc != 1) {
        fprintf(stderr, "Usage: %s [addresses.txt BACKING_STORE.bin]\n", argv[0]);
        return 1;
    }

    FILE *addresses_file = fopen(addresses_path, "r");
    if (addresses_file == NULL) {
        perror("Could not open addresses file");
        return 1;
    }

    int backing_fd = open(backing_store_path, O_RDONLY);
    if (backing_fd < 0) {
        perror("Could not open backing store");
        fclose(addresses_file);
        return 1;
    }

    signed char *backing_store = mmap(
        NULL,
        LOGICAL_ADDRESS_SPACE,
        PROT_READ,
        MAP_PRIVATE,
        backing_fd,
        0
    );

    if (backing_store == MAP_FAILED) {
        perror("mmap failed");
        fclose(addresses_file);
        close(backing_fd);
        return 1;
    }

    init_tlb();
    init_memory();

    char buffer[BUFFER_SIZE];

    int total_addresses = 0;
    int tlb_hits = 0;

    while (fgets(buffer, BUFFER_SIZE, addresses_file) != NULL) {
        int logical_address = atoi(buffer);
        total_addresses++;

        int page_number = logical_address >> OFFSET_BITS;
        int offset = logical_address & OFFSET_MASK;

        int frame_number = search_tlb(page_number);

        if (frame_number != -1) {
            tlb_hits++;
        } else {
            frame_number = get_frame_from_page_table(page_number);

            if (frame_number == -1) {
                frame_number = load_page_into_memory(page_number, backing_store);
            }

            add_tlb_entry(page_number, frame_number);
        }

        int physical_address = (frame_number << OFFSET_BITS) | offset;
        signed char value = get_value_at_physical_address(physical_address);

        printf("Virtual address: %d Physical address: %d Value: %d\n",
               logical_address, physical_address, value);
    }

    double page_fault_rate = total_addresses == 0 ? 0.0 : (double) page_faults / total_addresses;
    double tlb_hit_rate = total_addresses == 0 ? 0.0 : (double) tlb_hits / total_addresses;

    printf("\nStatistics\n");
    printf("Translated addresses: %d\n", total_addresses);
    printf("Page faults: %d\n", page_faults);
    printf("Page fault rate: %.3f\n", page_fault_rate);
    printf("TLB hits: %d\n", tlb_hits);
    printf("TLB hit rate: %.3f\n", tlb_hit_rate);
    printf("Page replacements: %d\n", page_replacements);

    munmap(backing_store, LOGICAL_ADDRESS_SPACE);
    close(backing_fd);
    fclose(addresses_file);

    return 0;
}
