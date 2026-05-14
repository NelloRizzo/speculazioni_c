#include "item.h"

typedef struct list *List;

List newList(void);
void printList(List);
int isEmpty(List);
void addHead(List, Item);
Item removeHead(List);
Item getHead(List);
int sizeList(List);
int addListTail(List, Item);

