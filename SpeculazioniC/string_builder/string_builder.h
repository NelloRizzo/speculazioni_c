#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#include "../stack_queue/queue.h"

typedef enum
{
    BUILDER_NO_ERROR = 0,
    BUILDER_APPEND_ERROR = -1,
    BUILDER_POSITION_ERROR = -2
} errors;

typedef struct string_builder
{
    queue *strings;
    size_t len;
    errors error;
} string_builder;

string_builder *builder_create(const char *s);
string_builder *builder_append(string_builder *sb, const char *s);
string_builder *builder_insert_at(string_builder *sb, const char *s, int pos);
string_builder *builder_prepend(string_builder *sb, const char *s, queue_node *parent);
char *builder_build(const string_builder *sb);
void builder_free(string_builder *sb);
#endif