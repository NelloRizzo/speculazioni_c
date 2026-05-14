#define _GNU_SOURCE
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"
#include "item.h"

void riorganizza(List l, Item x)
{
	// elementi minori di x
	List min = newList();
	// elementi maggiori di x
	List max = newList();
	// elementi uguali a x
	List eq = newList();

	// tolgo tutti gli elementi dalla lista
	while (!isEmpty(l))
	{
		// uno ad uno
		Item top = removeHead(l);
		int cmp = compareItem(top, x);
		printf("%s\t%s\t%d\n", (char *)top, (char *)x, cmp);
		// e li posiziono nelle liste appropriate (in coda per preservare l'ordine nel recupero successivo)
		if (cmp < 0)
		{
			addListTail(min, top);
		}
		else if (cmp > 0)
		{
			addListTail(max, top);
		}
		else
			addListTail(eq, top);
	}
	// adesso li rimetto a posto
	while (!isEmpty(min))
	{
		Item top = removeHead(min);
		// aggiungo in coda alla lista
		// invertendo nuovamente l'ordine per preservare l'ordine originale
		addListTail(l, top);
	}
	// aggiungo gli elementi uguali a x
	while (!isEmpty(eq))
	{
		Item top = removeHead(eq);
		addListTail(l, top);
	}
	// aggiungo gli elementi maggiori
	while (!isEmpty(max))
	{
		Item top = removeHead(max);
		addListTail(l, top);
	}
	// libro le liste di supporto allocate in testa al metodo
	// P.S.: solo la memoria della struttura lista, perché gli elementi sono
	// ancora presenti nella lista originaria
	free(min);
	free(max);
	free(eq);
}

int main(void)
{
	srand((unsigned int)time(NULL)); // lasciare questa riga all'inizio del main

	List l = newList();
	printf("Lista iniziale: [");
	printList(l);
	printf("]\n");

	Item x = strdup("casa");
	riorganizza(l, x);
	printf("Elemento: %s - Lista: [", (char *)x);
	printList(l);
	printf("]\n");

	addHead(l, x);
	addHead(l, strdup("ciao"));
	addHead(l, strdup("elefante"));
	addHead(l, strdup("dolce"));
	printf("Lista attuale: [");
	printList(l);
	printf("]\n");
	Item t = strdup("dire");
	riorganizza(l, t);
	printf("Elemento: %s - Lista: [", (char *)t);
	printList(l);
	printf("]\n");

	free(t);
	while (sizeList(l))
	{
		free(removeHead(l));
	}
	free(l);
}
