#ifndef MEMKIT_RECORDS_H
#define MEMKIT_RECORDS_H

#include <stdint.h>

// ============================================================================
// RECORDS API
// ============================================================================

#define MK_RECORD_ITEM_TIMESTAMP        (1 << 0)
#define MK_RECORD_ITEM_CALLER_LIB_NAME  (1 << 1)
#define MK_RECORD_ITEM_OP               (1 << 2)
#define MK_RECORD_ITEM_LIB_NAME         (1 << 3)
#define MK_RECORD_ITEM_SYM_NAME         (1 << 4)
#define MK_RECORD_ITEM_SYM_ADDR         (1 << 5)
#define MK_RECORD_ITEM_NEW_ADDR         (1 << 6)
#define MK_RECORD_ITEM_BACKUP_LEN       (1 << 7)
#define MK_RECORD_ITEM_ERRNO            (1 << 8)
#define MK_RECORD_ITEM_STUB             (1 << 9)
#define MK_RECORD_ITEM_FLAGS            (1 << 10)
#define MK_RECORD_ITEM_ALL              0x7FF

/* Get operation records as CSV string (caller must free) */
/* NOTE: The returned string is heap-allocated; caller is responsible for calling free() */
char *memkit_get_records(uint32_t item_flags);

/* Dump operation records to a file descriptor */
void memkit_dump_records_fd(int fd, uint32_t item_flags);

#endif // MEMKIT_RECORDS_H
