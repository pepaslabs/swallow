// swallow.c: buffer all of stdin until EOF, then dump to stdout.
// see https://github.com/pepaslabs/swallow
// copyright 2015 jason pepas (jasonpepas@gmail.com)
// released under the terms of the MIT license (https://opensource.org/licenses/MIT)

#include <stdlib.h> // for EXIT_SUCCESS, etc
#include <stdio.h> // for fread(), etc

#define BLOCK_SIZE (64*1024)

struct _buffer_list_t
{
    char bytes[BLOCK_SIZE];
    size_t used;
    struct _buffer_list_t *next;
};

typedef struct _buffer_list_t buffer_list_t;

buffer_list_t* buffer_list_new()
{
    buffer_list_t *buffer = malloc(sizeof(buffer_list_t));
    if (buffer == NULL) exit(EXIT_FAILURE);
    buffer->used = 0;
    buffer->next = NULL;
    return buffer;
}

void fill_buffer_node(buffer_list_t *buffer)
{
    char *buffer_ptr = buffer->bytes;
    size_t buffer_bytes_available = sizeof(buffer->bytes);
    size_t bytes_read = 0;

    while(buffer_bytes_available > 0 && feof(stdin) == 0)
    {
        bytes_read = fread(buffer_ptr, sizeof(char), buffer_bytes_available, stdin);
        buffer_bytes_available -= bytes_read;
        buffer_ptr += bytes_read;
    }

    buffer->used = bytes_read;
}

void exhaust_stdin(buffer_list_t *head_buffer)
{
    buffer_list_t *current_buffer = head_buffer;

    while(feof(stdin) == 0)
    {
        fill_buffer_node(current_buffer);
        
        if (feof(stdin) == 0)
        {
            current_buffer->next = buffer_list_new();
            current_buffer = current_buffer->next;
        }
    }
}

void dump_buffer_node_to_stdout(buffer_list_t *buffer)
{
    size_t bytes_written = fwrite(buffer->bytes, sizeof(char), buffer->used, stdout);
    if (bytes_written != buffer->used) exit(EXIT_FAILURE);
}

void dump_buffers_to_stdout(buffer_list_t *head_buffer)
{
    buffer_list_t *current_buffer = head_buffer;
    while(current_buffer != NULL)
    {
        dump_buffer_node_to_stdout(current_buffer);
        current_buffer = current_buffer->next;
    }
}

int main(int argc, const char *argv[])
{
    buffer_list_t *head_buffer = buffer_list_new();

    exhaust_stdin(head_buffer);
    dump_buffers_to_stdout(head_buffer);

    if (fflush(stdout) != 0) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
