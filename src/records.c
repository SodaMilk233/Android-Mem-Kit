#include "../include/memkit.h"
#include "../deps/shadowhook/shadowhook/src/main/cpp/include/shadowhook.h"

// ============================================================================
// RECORDS: GET AS STRING
// ============================================================================

char *memkit_get_records(uint32_t item_flags) {
    return shadowhook_get_records(item_flags);
}

// ============================================================================
// RECORDS: DUMP TO FILE DESCRIPTOR
// ============================================================================

void memkit_dump_records_fd(int fd, uint32_t item_flags) {
    shadowhook_dump_records(fd, item_flags);
}
