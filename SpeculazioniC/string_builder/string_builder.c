#define _GNU_SOURCE
#include "string_builder.h"
#include <stdlib.h>
#include <string.h>

string_builder *builder_create(const char *s)
{
    string_builder *sb = malloc(sizeof(string_builder));
    if (!sb)
        return NULL;
    sb->strings = queue_create();
    if (!sb->strings)
    {
        free(sb);
        return NULL;
    }
    sb->len = 0;
    sb->error = BUILDER_NO_ERROR;
    return builder_append(sb, s);
}

string_builder *builder_append(string_builder *sb, const char *s)
{
    if (!sb)
        return NULL;
    if (sb->error != BUILDER_NO_ERROR)
        return sb;
    if (!s)
        return sb;
    char *copy = strdup(s);
    if (!copy)
    {
        sb->error = BUILDER_APPEND_ERROR;
    }
    else
    {
        queue_enqueue(sb->strings, copy);
        sb->len += (int)strlen(s);
    }
    return sb;
}

string_builder *builder_insert_at(string_builder *sb, const char *s, int pos)
{
    if (!sb)
        return NULL;
    if (!s)
        return sb;

    int count = 0;
    queue_node *cursor = sb->strings->head;
    while (cursor && count < pos - 1)
    {
        cursor = cursor->next;
        count++;
    }
    if (!cursor)
    {
        sb->error = BUILDER_POSITION_ERROR;
    }
    else
    {
        char *copy = strdup(s);
        if (!copy)
        {
            sb->error = BUILDER_APPEND_ERROR;
            return sb;
        }
        queue_node *node = malloc(sizeof(queue_node));
        if (!node)
        {
            free(copy);
            sb->error = BUILDER_APPEND_ERROR;
            return sb;
        }
        node->content = copy;
        node->next = cursor->next;
        cursor->next = node;
        sb->len += strlen(s);
        sb->strings->size++;
    }
    return sb;
}

string_builder *builder_prepend(string_builder *sb, const char *s, queue_node *parent)
{
    return nullptr;
}

char *builder_build(const string_builder *sb)
{
    if (!sb)
        return NULL;

    char *result = calloc(sizeof(char), sb->len + 1);
    if (!result)
        return NULL;

    queue_node *cursor = sb->strings->head;
    size_t current_len = 0;
    while (cursor)
    {
        char *content = cursor->content;
        size_t len = strlen(content);
        memcpy(result + current_len, (char *)cursor->content, len);
        current_len += len;
        cursor = cursor->next;
    }
    return result;
}

void builder_free(string_builder *sb)
{
    if (!sb)
        return;
    queue_free(sb->strings, free);
    free(sb);
}
