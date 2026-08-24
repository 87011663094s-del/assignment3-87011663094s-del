/**
 * @file aesd-circular-buffer.c
 * @brief Functions and data related to a circular buffer implementation
 *
 * @author Dan Walkes
 * @date 2020-03-01
 * @copyright Copyright (c) 2020
 *
 */

#ifdef __KERNEL__
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "aesd-circular-buffer.h"

/**
 * @param buffer the buffer to search for corresponding offset.
 * @param char_offset the position to search for in the buffer list
 * @param entry_offset_byte_rtn location to store byte offset
 * @return matching buffer entry or NULL
 */
struct aesd_buffer_entry *aesd_circular_buffer_find_entry_offset_for_fpos(
        struct aesd_circular_buffer *buffer,
        size_t char_offset,
        size_t *entry_offset_byte_rtn)
{
    uint8_t index = buffer->out_offs;
    size_t current_offset = 0;
    uint8_t count = 0;

    while (count < AESDCHAR_MAX_WRITE_OPERATIONS_SUPPORTED) {

        if (!buffer->full && index == buffer->in_offs) {
            break;
        }

        if (buffer->entry[index].buffptr != NULL) {

            if (char_offset < current_offset + buffer->entry[index].size) {
                *entry_offset_byte_rtn = char_offset - current_offset;
                return &buffer->entry[index];
            }

            current_offset += buffer->entry[index].size;
        }

        index++;

        if (index >= AESDCHAR_MAX_WRITE_OPERATIONS_SUPPORTED) {
            index = 0;
        }

        count++;
    }

    return NULL;
}

/**
 * Adds an entry to the circular buffer.
 */
void aesd_circular_buffer_add_entry(
        struct aesd_circular_buffer *buffer,
        const struct aesd_buffer_entry *add_entry)
{
    buffer->entry[buffer->in_offs] = *add_entry;

    if (buffer->full) {
        buffer->out_offs++;

        if (buffer->out_offs >= AESDCHAR_MAX_WRITE_OPERATIONS_SUPPORTED) {
            buffer->out_offs = 0;
        }
    }

    buffer->in_offs++;

    if (buffer->in_offs >= AESDCHAR_MAX_WRITE_OPERATIONS_SUPPORTED) {
        buffer->in_offs = 0;
    }

    if (buffer->in_offs == buffer->out_offs) {
        buffer->full = true;
    }
}

/**
 * Initializes the circular buffer to empty.
 */
void aesd_circular_buffer_init(struct aesd_circular_buffer *buffer)
{
    memset(buffer, 0, sizeof(struct aesd_circular_buffer));
}
