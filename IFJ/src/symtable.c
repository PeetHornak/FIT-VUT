/**
 * @file: symtable.c
 * @date: 20.10.2018
 * @author: Peter Hornak - xhorna14
 * Collaborators:
 *          Nikolas Patrix - xpatri00
 *          Matej Jancek - xjance00
 *          Robert Hubinak - xhubin03
 *
 * @brief Implementacia tabulky symbolov pomocou hashovacej tabulky
 */

#include "header/symtable.h"
#include "header/garbagecollector.h"

#define HTSIZE 500

static htab_t *global_table;
static htab_t *local_table;
static TGLOBTab *glob_obj;
static TLOCTab *loc_obj;
static struct htab_listitem *item;

void htab_setup(){
    global_table = htab_init(HTSIZE);
    struct htab_listitem *item;
    /* Init main */
    htab_t *local_table = htab_init(HTSIZE);
    item = htab_lookup_add(global_table, "main", glob_create);
    glob_obj = (TGLOBTab*) item->object;
    glob_init(glob_obj, 0, INT, local_table, true);

    /* Init inputs */
    local_table = htab_init(HTSIZE);
    item = htab_lookup_add(global_table, "inputs", glob_create);
    glob_obj = (TGLOBTab*) item->object;
    glob_init(glob_obj, 0, INT, local_table, true);
    
    /* Init inputi */
    local_table = htab_init(HTSIZE);
    item = htab_lookup_add(global_table, "inputi", glob_create);
    glob_obj = (TGLOBTab*) item->object;
    glob_init(glob_obj, 0, INT, local_table, true);
    
    /* Init inputf*/
    local_table = htab_init(HTSIZE);
    item = htab_lookup_add(global_table, "inputf", glob_create);
    glob_obj = (TGLOBTab*) item->object;
    glob_init(glob_obj, 0, INT, local_table, true);
    
    /* Init print */
    local_table = htab_init(HTSIZE);
    item = htab_lookup_add(global_table, "print", glob_create);
    glob_obj = (TGLOBTab*) item->object;
    glob_init(glob_obj, -2, NIL, local_table, true);
    
    /* Init length */
    local_table = htab_init(HTSIZE);
    item = htab_lookup_add(global_table, "length", glob_create);
    glob_obj = (TGLOBTab*) item->object;
    glob_init(glob_obj, 1, INT, local_table, true);
    
    /* Init length PARAM */
    item = htab_lookup_add(glob_obj->loc_symtab, "s", loc_create);
    loc_obj = item->object;
    loc_init(loc_obj, STRING, true);
    
    /* Init substr */
    local_table = htab_init(HTSIZE);
    item = htab_lookup_add(global_table, "substr", glob_create);
    glob_obj = (TGLOBTab*) item->object;
    glob_init(glob_obj, 3, STRING, local_table, true);
    
    /* Init substr PARAM */
    item = htab_lookup_add(glob_obj->loc_symtab, "s", loc_create);
    loc_obj = item->object;
    loc_init(loc_obj, STRING, true);

    item = htab_lookup_add(glob_obj->loc_symtab, "i", loc_create);
    loc_obj = item->object;
    loc_init(loc_obj, INT, true);

    item = htab_lookup_add(glob_obj->loc_symtab, "n", loc_create);
    loc_obj = item->object;
    loc_init(loc_obj, INT, true);
    
    /* Init ord */
    local_table = htab_init(HTSIZE);
    item = htab_lookup_add(global_table, "ord", glob_create);
    glob_obj = (TGLOBTab*) item->object;
    glob_init(glob_obj, 2, INT, local_table, true);

    /* Init ord PARAM */
    item = htab_lookup_add(glob_obj->loc_symtab, "s", loc_create);
    loc_obj = item->object;
    loc_init(loc_obj, STRING, true);
    
    item = htab_lookup_add(glob_obj->loc_symtab, "i", loc_create);
    loc_obj = item->object;
    loc_init(loc_obj, INT, true);
    
    /* Init chr */
    local_table = htab_init(HTSIZE);
    item = htab_lookup_add(global_table, "chr", glob_create);
    glob_obj = (TGLOBTab*) item->object;
    glob_init(glob_obj, 1, INT, local_table, true);
    
    /* Init chr PARAM */
    item = htab_lookup_add(glob_obj->loc_symtab, "i", loc_create);
    loc_obj = item->object;
    loc_init(loc_obj, INT, true);
    
    item = htab_find(global_table, "main");
    glob_obj =item->object;
}

TGLOBTab* htab_return_pointer(){
    return glob_obj;
}

TGLOBTab* htab_def_func(char* key){
    if((item = htab_find(global_table, key)) == NULL){      //ak nenajdes funkciu
        item = htab_find(global_table, "main");
        TGLOBTab* my_glob_obj = item->object;
        if((item = htab_find(my_glob_obj->loc_symtab, key)) == NULL){ // nenajdes ID v maine
            local_table = htab_init(HTSIZE);                    
            item = htab_lookup_add(global_table, key, glob_create);
            glob_obj = (TGLOBTab*) item->object;
            glob_init(glob_obj, -1, INT, local_table, true);     //pridaj hu tam
        }
        else{
            gb_exit_process(3);
        }
    }
    else{
        glob_obj = (TGLOBTab*) item->object;        //zmen object s ktorym sa pracuje
        if(glob_obj->defined != true){
            glob_obj->defined = true;
        }
        else{
            gb_exit_process(3);             //RETURN ERROR CODE 3
        }
    }
    return glob_obj;
}

TGLOBTab* htab_call_func(char* key){
    if((item = htab_find(global_table, key)) == NULL){      //ak nenajdes funkciu
        item = htab_find(global_table, "main");
        TGLOBTab* my_glob_obj = item->object;
        if((item = htab_find(my_glob_obj->loc_symtab, key)) == NULL){   //nenajdes premennu
            htab_t *my_local_table = htab_init(HTSIZE);                    
            item = htab_lookup_add(global_table, key, glob_create); 
            my_glob_obj = (TGLOBTab*) item->object;
            glob_init(my_glob_obj, -1, INT, my_local_table, false);     //pridaj hu tam
        }
        else{
            gb_exit_process(3);
        }
        return item->object;
    }
    return item->object;
}


void htab_add_id(char *key){
    if((item = htab_find(global_table, key)) == NULL)
        if((item = htab_find(glob_obj->loc_symtab, key)) == NULL){      //ak nenajdes id
            item = htab_lookup_add(glob_obj->loc_symtab, key, loc_create);
            loc_obj = (TLOCTab*) item->object;
            loc_init(loc_obj, NONE, true);     //pridaj ho tam
        }
        else{
            loc_obj = (TLOCTab*) item->object;
        }
    else
        gb_exit_process(3);
}

void htab_set_param_count(TGLOBTab* my_glob_obj,int count){
    if(my_glob_obj->params_count == -1){
        my_glob_obj->params_count = count;
    }
    else if(my_glob_obj->params_count == -2)
        return;
    else if(my_glob_obj->params_count != count){
        gb_exit_process(5);
    }
}

void htab_set_main(){
    item = htab_find(global_table, "main");
    glob_obj = item->object;
}

void htab_find_id(char *key){
    if((item = htab_find(glob_obj->loc_symtab, key)) == NULL){  // Ak nenaslo ID
        if((item = htab_find(global_table, key)) == NULL){ // A nenaslo funkc
            htab_t* my_local_table = htab_init(HTSIZE);                    
            item = htab_lookup_add(global_table, key, glob_create);
            TGLOBTab* my_glob_obj = (TGLOBTab*) item->object;
            glob_init(my_glob_obj, 0, INT, my_local_table, false);  // Pridaj funkc s 0 params
        }
        else{
            TGLOBTab* my_glob_obj = (TGLOBTab*) item->object;
            if(my_glob_obj->params_count != -1){         // Skontroluj spravne volanie
                if(my_glob_obj->params_count != 0){
                    gb_exit_process(5);
                }
            else
                my_glob_obj->params_count = 0;
            }       
        }
    }
}

void htab_def_param(char *key){
    if((item = htab_find(glob_obj->loc_symtab, key)) == NULL){      //ak nenajdes id
        item = htab_lookup_add(glob_obj->loc_symtab, key, loc_create);
        loc_obj = (TLOCTab*) item->object;
        loc_init(loc_obj, INT, true);     //pridaj ho tam
    }
    else{
        gb_exit_process(3);
    }
}

void htab_check_param(char *key){
    if((item = htab_find(glob_obj->loc_symtab, key)) == NULL){      //ak nenajdes id
        gb_exit_process(3);
    }
}

void global_def(const char* key, void* object)
{
    TGLOBTab *tmp = object;
    if(tmp->defined == false){
        gb_exit_process(3);
    }
}

void check_defined(){
    htab_foreach(global_table, global_def);
}

bool check_id(char *key){
    if((item = htab_find(glob_obj->loc_symtab, key)) == NULL)      //ak nenajdes id
        return false;
    else
        return true;
}

unsigned htab_hash_function(const char *key)
{
    unsigned h = 0;
    const unsigned char *p;
    for(p = (const unsigned char*)key; *p != '\0'; p++)
        h = 65599*h + *p;
    return h;
}

htab_t* htab_init(unsigned arr_size)
{
    //Alokacia- velkost tabulky + velkost (size) itemov
    htab_t* htab = (htab_t *) gb_malloc(sizeof(htab_t) + arr_size*sizeof(struct htab_listitem *));

    if(htab == NULL)
        return NULL; // Zla alokacia
    
    htab->arr_size = arr_size;
    //Prvy prvok kazdeho zoznamu ukazuje na NULL
    for(unsigned i = 0; i < arr_size; i++)
        htab->arr[i] = NULL;

    return htab;
}

htab_t* htab_move(unsigned newsize, htab_t* t2,void* (*copy_deep)(void*))
{
    
    htab_t *t = htab_init(newsize);

    if(t-3 == NULL || t == NULL)
        return NULL;

    struct htab_listitem* item = NULL;
    struct htab_listitem* temp = NULL;
    
    //Preiteruje cez kazdy zoznam
    for(unsigned i = 0; i < htab_bucket_count(t2); i++){
        item = t2->arr[i];
        //Iteracia zoznamom prida slovo do noveho zoznamu, skopiruje data
        while(item != NULL){
            temp = htab_lookup_add(t, item->key,NULL);
            temp->object = copy_deep(item->object);
            item = item->next;
        }
        t->size++;
    }
    //Vycisti zoznamy starej tabulky
    htab_clear(t2);
    return t;
}

unsigned htab_size(htab_t* t){
    return t->size;
}

unsigned htab_bucket_count(htab_t* t){
    return t->arr_size;
}

struct htab_listitem* htab_lookup_add(htab_t* t, const char* key, void* (*o_create)())
{
    
    if(t == NULL || key == NULL)
        return NULL;

    //Index vypocitany cez hash modulo arr_size
    unsigned i = htab_hash_function(key) % htab_bucket_count(t);

    struct htab_listitem* item = t->arr[i];
    struct htab_listitem* previous = NULL;
        
    //Cyklus ktory hlada zhodu, pri zhode inc data a returne item
    while(item != NULL){
        if(!strcmp(key, item->key)){
            return item;
        }
        previous = item;
        item = item->next;
    }

    //Inak vytvor novy item
    struct htab_listitem* new_item = (struct htab_listitem*) gb_malloc(sizeof(struct htab_listitem));
        
    if(new_item == NULL)
        return NULL;

    //Alokacia pamate pre key a +1 byte pre ukoncovaciu nulu    
    new_item->key = (char*) gb_malloc(sizeof(char)*(strlen(key)+1));
    
    if(new_item->key == NULL){
        free(new_item);
        return NULL;
    }
    //Nastavenie novych dat itemu
    strcpy(new_item->key, key);
    new_item->next = NULL;
    if(o_create != NULL)
        new_item->object = o_create();
    else
        new_item->object = NULL;
    t->size++;

    if(previous == NULL){
        t->arr[i] = new_item;
    }
    else{
        previous->next = new_item;
    }

    return new_item;
}

struct htab_listitem* htab_find(htab_t* t, const char* key){
    
    if(t == NULL || key == NULL)
        return NULL;
    
    //Index vypocitany cez hash modulo arr_size
    unsigned i = htab_hash_function(key) % htab_bucket_count(t);

    struct htab_listitem *item = t->arr[i];

    //Cyklus ktory hlada zhodu
    while(item != NULL){
        if(!strcmp(key, item->key))
            break;
        item = item->next;
    }
    return item;
}

void htab_foreach(htab_t* t, void(*func)(const char *, void*)){
    
    if(t == NULL)
        return;

    struct htab_listitem* item;

    //Iteruj cez kazdy zoznam
    for(unsigned i = 0; i < htab_bucket_count(t); i++){
        if(t->arr[i] != NULL){
            item = t->arr[i];
            //Iteruj cez zoznam
            while(item != NULL){
                func(item->key, item->object); //Zavolaj funkciu pre kazdy item
                item = item->next;
            }
        }
    }
}

bool htab_remove(htab_t* t, const char* key){    
    if(t == NULL || key == NULL)
        return false;

    unsigned i = htab_hash_function(key) % htab_bucket_count(t);

    struct htab_listitem* item = t->arr[i];
    struct htab_listitem* previous = NULL;
    
    while(item != NULL){
        if(!strcmp(key, item->key)){ //Ak zhoda
            free(item->key); // Uvolni key
            free(item->object);
            t->size--; // Zmensi pocet itemov
            if(previous == NULL) //Ak je prvy, na zaciatok prirad next
                t->arr[i]=item->next; 
            else
                previous->next = item->next; //Inak previous prirad next
            free(item); //Uvolni item
            return true;
        }
        previous = item;
        item = item->next;
    }
    return false;
}
 
void htab_clear(htab_t* t){
    
    if(t == NULL)
        return;
    
    struct htab_listitem* item = NULL;
    struct htab_listitem* temp = NULL;

    // Prejde kazdy zoznam
    for(unsigned i = 0; i < htab_bucket_count(t); i++){
        item = t->arr[i];
        // Prejde zoznam az po koniec a uvolni ho
        while(item != NULL){
            temp = item->next; free(item->key); free(item->object);
            free(item);
            t->size--;
            item = temp;
        }
        t->arr[i] = NULL;
        item = NULL;
    }
}

void htab_free(htab_t* t){
    
    if(t == NULL)
        return;

    htab_clear(t);
    free(t);
    t = NULL;
}

void* glob_create()
{
    TGLOBTab *tmp = gb_malloc(sizeof(struct global_table_object));
    tmp->return_type = NONE;
    tmp->defined = true;
    return tmp;
}

void glob_init(TGLOBTab *t, int params_count, TYPES return_type,
        htab_t *loc_symtab, bool defined)
{
    t->params_count = params_count;
    t->return_type = return_type;
    t->loc_symtab = loc_symtab;
    t->defined = defined;
}

void* loc_create()
{
    TLOCTab *tmp = gb_malloc(sizeof(struct global_table_object));
    tmp->type = NONE;
    tmp->initialized = false;
    return tmp;
}

void loc_init(TLOCTab *t, TYPES type, bool initialized)
{
    t->type = type;
    t->initialized = initialized;
}

