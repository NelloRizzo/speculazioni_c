#ifndef LIST_H
#define LIST_H

typedef struct list *List;

List list_create(void);
void list_destroy(List l, void (*free_fn)(void *));
void list_offer(List l, void *data);
void list_append(List l, void *data);
void list_visit(List l, void (*fn)(void *));
void list_selection_sort(List l, int (*comparer)(void *, void *));

#endif