/*
 * @brief Vrati pocet zoznamov v tabulke
 */
unsigned htab_bucket_count(htab_t* t){
    return t->arr_size;
}
 
/*
 * @brief Vymaze zoznam a jeho prvky 
 */
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
            temp = item->next;
            free(item->key);
            free(item);
            t->size--;
            item = temp;
        }
        t->arr[i] = NULL;
        item = NULL;
    }
}

/*
 * @brief Hlada slovo v tabulke, ak najde vrati pointer, inak NULL
 * @param *key Retazec
 */
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

/*
 * @brief Zavola funkciu pre kazdy prvok tabulky
 * @param *func Ukazatel na funkciu 
 */
void htab_foreach(htab_t* t, void(*func)(const char *, unsigned *)){
    
    if(t == NULL)
        return;

    struct htab_listitem* item;

    //Iteruj cez kazdy zoznam
    for(unsigned i = 0; i < htab_bucket_count(t); i++){
        if(t->arr[i] != NULL){
            item = t->arr[i];
            //Iteruj cez zoznam
            while(item != NULL){
                func(item->key, &item->data); //Zavolaj funkciu pre kazdy item
                item = item->next;
            }
        }
    }
}

/*
 * @brief Uvolni alokovanu pamat pre tabulku
 */
void htab_free(htab_t* t){
    
    if(t == NULL)
        return;

    htab_clear(t);
    free(t);
    t = NULL;
}

/*
 * @brief Hashovacia funkcia
 * @param *key Retazec
 */
unsigned htab_hash_function(const char *key){
    unsigned h = 0;
    const unsigned char *p;
    for(p = (const unsigned char*)key; *p != '\0'; p++)
        h = 65599*h + *p;
    return h;
}

/*
 * @brief Inicializacia tabulky o danej velkosti
 * @param arr_size Velkost pola
 */
htab_t* htab_init(unsigned arr_size){
    //Alokacia- velkost tabulky + velkost (size) itemov
    htab_t* htab = (htab_t *) malloc(sizeof(htab_t) + arr_size*sizeof(struct htab_listitem *));

    if(htab == NULL)
        return NULL; // Zla alokacia
    
    htab->arr_size = arr_size;
    //Prvy prvok kazdeho zoznamu ukazuje na NULL
    for(unsigned i = 0; i < arr_size; i++)
        htab->arr[i] = NULL;

    return htab;
}

/*
 * @brief Hlada slovo v tabulke, ak ho najde zvacsi data, inak ho vlozi do tabulky
 * @param *key Retazec
 */
struct htab_listitem* htab_lookup_add(htab_t* t, const char* key){
    
    if(t == NULL || key == NULL)
        return NULL;

    //Index vypocitany cez hash modulo arr_size
    unsigned i = htab_hash_function(key) % htab_bucket_count(t);

    struct htab_listitem* item = t->arr[i];
    struct htab_listitem* previous = NULL;
        
    //Cyklus ktory hlada zhodu, pri zhode inc data a returne item
    while(item != NULL){
        if(!strcmp(key, item->key)){
            item->data++;
            return item;
        }
        previous = item;
        item = item->next;
    }

    //Inak vytvor novy item
    struct htab_listitem* new_item = (struct htab_listitem*) malloc(sizeof(struct htab_listitem));
        
    if(new_item == NULL)
        return NULL;

    //Alokacia pamate pre key a +1 byte pre ukoncovaciu nulu    
    new_item->key = (char*) malloc(sizeof(char)*(strlen(key)+1));
    
    if(new_item->key == NULL){
        free(new_item);
        return NULL;
    }
    //Nastavenie novych dat itemu
    strcpy(new_item->key, key);
    new_item->next = NULL;
    new_item->data = 1;
    t->size++;

    if(previous == NULL){
        t->arr[i] = new_item;
    }
    else{
        previous->next = new_item;
    }

    return new_item;
}

/*
 * @brief Vytvorenie a inicializacia tabulky na zaklade dat z tabulky t2, t2 zostane prazdna
 * @param newsize Nova velkost
 */
htab_t* htab_move(unsigned newsize, htab_t* t2){
    
    htab_t *t = htab_init(newsize);

    if(t2 == NULL || t == NULL)
        return NULL;

    struct htab_listitem* item = NULL;
    struct htab_listitem* temp = NULL;
    
    //Preiteruje cez kazdy zoznam
    for(unsigned i = 0; i < htab_bucket_count(t2); i++){
        item = t2->arr[i];
        //Iteracia zoznamom prida slovo do noveho zoznamu, skopiruje data
        while(item != NULL){
            temp = htab_lookup_add(t, item->key);
            temp->data = item->data;
            item = item->next;
        }
        t->size++;
    }
    //Vycisti zoznamy starej tabulky
    htab_clear(t2);
    return t;
}

/*
 * @brief Vyhlada a vymaze zadane slovo z tabulky
 * @param *key Retazec
 */
bool htab_remove(htab_t* t, const char* key){
    
    if(t == NULL || key == NULL)
        return false;

    unsigned i = htab_hash_function(key) % htab_bucket_count(t);

    struct htab_listitem* item = t->arr[i];
    struct htab_listitem* previous = NULL;
    
    while(item != NULL){
        if(!strcmp(key, item->key)){ //Ak zhoda
            free(item->key); // Uvolni key
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

/*
 * @brief Vracia pocet prvkov zoznamu
 */
unsigned htab_size(htab_t* t){
    return t->size;
}
