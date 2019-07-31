/**
 * Kostra pro cviceni operaci nad jednosmerne vazanym seznamem.
 * IZP, 2017
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
 
/**
 * struktura Object
 */
typedef struct {
  int id;
  char *name;
} Object;
 
/**
 * Polozka seznamu
 */
typedef struct item Item;
struct item {
  Object data;
  Item *next;
};
 
/**
 * Typ seznam
 */
typedef struct {
  Item *first;
} List;
 
/**
 * Inicializace seznamu. Vytvori prazdny seznam.
 */
List list_ctor()
{
	List list;
	list.first = NULL;
	return list;
}
 
/**
 * Inicializace polozky seznamu. Z objektu vytvori polozku bez naslednika.
 */
Item *item_ctor(Object data)
{
	Item *p_item;
	p_item = malloc(sizeof(Item));
	if(p_item == NULL)
		return NULL;
	else
	p_item->data = data;
	p_item->next = NULL;
	
	return p_item;
}
 
 
/**
 * Vlozi polozku na zacatek seznamu.
 */
void list_insert_first(List *list, Item *i)
{
	Item *temp;
	temp = list->first;
	list->first = i;
	i->next = temp;
}
 
/**
 * Vrati true, pokud je seznam prazdny.
 */
bool list_empty(List *list)
{
	if(list->first == NULL)
		return true;
	else 
		return false;
}
 
/**
 * Odstrani prvni prvek seznamu, pokud je.
 */
void list_delete_first(List *list)
{
	if(list->first != NULL)
	{
		Item *p = list->first->next;
		free(list->first);
		list->first = p;
	}
}
 
/**
 * Vrati pocet polozek seznamu.
 */
unsigned list_count(List *list)
{
	unsigned i = 0;
	Item *p;
	p = list->first;

	while(p != NULL)
	{
		i++;
		p = p->next;
	}

	return i;
}
 
 
/**
 * Najde polozku seznamu s nejmensim identifikatorem. Vraci NULL, pokud je
 * seznam prazdny.
 */
Item *list_find_minid(List *list)
{
	Item *min;
	Item *p;

	p = list->first;
	min = p;
	
	while(p != NULL)
	{
		if(min->data.id > p->data.id)
			min = p;

		p = p->next;
	}

	return min;
}
 
/**
 * Najde polozku seznamu s odpovidajicim jmenem objektu. Vraci NULL, pokud
 * takova polozka v seznamu neni.
 */
Item *list_find_name(List *list, char *name)
{
	Item *p;
	p = list->first;

	while(p != NULL)
	{
		if(!strcmp(p->data.name, name))
			break;
		p = p->next;
	}

	return p;
}
 
/**
 * Uvolneni seznamu.
 */
void list_dtor(List *list)
{
	Item *next;
	Item *p;
	
	p = list->first;
	
	while(p != NULL)
	{
		next = p->next;
		free(p);
		p = next;
	}

	list->first = NULL;
}

/*void list_join(List *list1, List *list2)
{
	if(list1->first == NULL && list2->first == NULL)
		return;

	if(list1->first == NULL)
	{
		list1->first = list2->first;
		return;
	}
		
	Item *last;
	last = list1->first;
	while(last->next != NULL)
		last = last->next;
	
	last->next = list2->first;
}*/
 
int main()
{
printf("list_ctor...\n");
    List list = list_ctor();
 
    printf("list_empty...\n");
    printf("Seznam prazdny: %s\n", list_empty(&list) ? "ano" : "ne");
 
    Item *item;
 
    Object o1 = {42, "Honza"};
    printf("item_ctor...\n");
    item = item_ctor(o1);
    printf("list_insert_first...\n");
    list_insert_first(&list, item);
 
    printf("Seznam prazdny: %s\n", list_empty(&list) ? "ano" : "ne");
    printf("list_count...\n");
    printf("Pocet prvku v seznamu: %d\n", list_count(&list));
 
    Object o2 = {2, "Malem"};
    item = item_ctor(o2);
    printf("list_insert_first...\n");
    list_insert_first(&list, item);
 
    Object o3 = {0, "Kralem"};
    item = item_ctor(o3);
    printf("list_insert_first...\n");
    list_insert_first(&list, item);
 
    printf("Pocet prvku v seznamu: %d\n", list_count(&list));
 
    printf("Odstraneni prvniho prvku ze seznamu \n");
    list_delete_first(&list);
    printf("Pocet prvku v seznamu: %d\n", list_count(&list));
 
 
    // opetovne vlozeni objektu o1		
    item = item_ctor(o1);
    printf("list_insert_first...\n");
    list_insert_first(&list, item);
 
    printf("list_find_minid...\n");
    item = list_find_minid(&list);
    if (item != NULL) {
        printf("Polozka s nejmensim identifikatorem: {%d, \"%s\"}\n",
            item->data.id, item->data.name);
	}    
	else
            printf("Polozka s nejmensim identifikatorem nenalezena\n");
 
    printf("list_find_name...\n");
    char* name = "Honza";
    item = list_find_name(&list, name);
    if (item != NULL) {
             printf("Polozka s daty %s nalezena\n", name);
	}    
	else
             printf("Polozka s daty %s nenalezena.\n",name);
/*//////////////////////////////////////////////
	printf("list2_ctor...\n");
    List list2 = list_ctor();
 
    printf("list2_empty...\n");
    printf("Seznam2 prazdny: %s\n", list_empty(&list) ? "ano" : "ne");
 
    Object o4 = {420, "Peto"};
    printf("item_ctor...\n");
    item = item_ctor(o4);
    printf("list2_insert_first...\n");
    list_insert_first(&list2, item);
    printf("Pocet prvku v seznamu: %d\n", list_count(&list));
	printf("list_join...\n");
	list_join(&list,&list2);
    printf("Pocet prvku v seznamu: %d\n", list_count(&list));
 *//////////////////////////////////////////
    printf("list_dtor...\n");
    list_dtor(&list);
    printf("Seznam prazdny: %s\n", list_empty(&list) ? "ano" : "ne");

    return 0;
}
