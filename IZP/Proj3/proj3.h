/**
 * \mainpage 3. Projekt
 * \file proj3.h
 * Kostra hlavickoveho souboru 3. projekt IZP 2017/18
 * a pro dokumentaci Javadoc.
 */

/**
 * \brief Struktura objektu: identifikator a suradnice.
 *
 * Objekt reprezentuje bod so suradnicami v rovine ktory 
 * ma unikatny identifikator.
 */
struct obj_t {
    int id;
    float x;
    float y;
};

/**
 * \brief Zhluk objektov.
 *
 * Zhluk je definovany kapacitou, momentalnou velkostou a
 * ukazovatelom na pole objektov.
 */
struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/**
 * \defgroup PracaSPamatou
 * Funkcie urcene na pracu s pamatou nad clustermi a objektmi.
 * \{
 */

/**
 * \brief Inicializacia zhluku 'c'.
 *
 * Alokuje pamat pro 'cap' objektov (kapacitu).
 * Ukazatel NULL u pola objektov znamena kapacitu 0.
 *
 * \param *c Ukazatel na zhluk pre ktory sa bude alokovat pamat.
 * \param cap Kapacita zhluku ktora sa bude alokovat.
 * \pre Kapacita nemoze byt zaporna.
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * \brief Odstranenie vsetkuch objektov zhluku.
 *
 * Odstrani vsetky objekty a inicializuje zhluk ako prazdny.
 *
 * \param *c Ukazatel na zhluk ktory bude vyprazdneny. 
 */
void clear_cluster(struct cluster_t *c);

/**
 * Hodnota odporucana na reallokovanie zhluku.
 */
extern const int CLUSTER_CHUNK;

/**
 * \brief Zmena kapacity zhluku 'c' na kapacitu 'new_cap'.
 *
 * \param *c Ukazatel na zhluk pre ktory bude funkcia menit velkost.
 * \param new_cap Nova kapacita zhluku.
 *
 * \pre Ukazatel '*c' neukazuje na NULL.
 * \pre Nova kapacita nie je zaporna.
 * \return Ukazatel na zhluk ktory bol realokovany.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * /brief Prida objekt na koniec zhluku.
 *
 * Prida objekt 'obj' na koniec zhluku 'c'. 
 * Rozsiri zhluk, pokial sa do neho objekt nezmesti.
 * 
 * \param *c Ukazatel na zhluk do ktoreho sa bude objekt pridavat.
 * \param obj Objekt ktory bude pridany na koniec zhluku.
 *
 * \pre Ukazatel '*c' neukazuje na NULL.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * \brief Zjednoti objekty dvoch zhlukov a zoradi ich.
 *
 * Do zhluku 'c1' prida objekty 'c2'. Zhluk 'c1' bude rozsireny 
 * ak to bude nutne. Objekty zhluku 'c1' budu zoradene vzostupne
 * podla id. Zhluk 'c2' zostane nezmeneny.
 *
 * \param *c1 Ukazatel na zhluk do ktoreho sa budu pridavat objekty.
 * \param *c2 Ukazatel na zhluk ktoreho objekty budu pridane do 'c1'.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);  
/** /} */

/**
 * \defgroup PracaNadPolomZhlukov
 * Funkcie urcene na pracu s polom zhlukov.
 * Matematicke operacie nad clustermi a objektmi.
 * \{
 */

/**
 * \brief Odsrani zhluk ktory je na urcitej pozicii c poli zhlukov.
 * 
 * Odstrani zhluk z pola zhlukov 'carr'. Pole zhlukov obsahuje 'narr'
 * poloziek (zhluku). Zhluk po odstraneni sa nachadza na indexe 'idx'.
 * \param *carr Ukazatel na pole zhlukov.
 * \param narr Velkost pola zhlukov.
 * \param *idx Index zhluku ktory sa ma odstranit.
 * \pre Velkost musi byt vacsia ako 0.
 * \pre Index nemoze byt vacsi ako pocet poloziek v 'carr'.
 * \return Funkcia vracia novy pocet zhlukov v poli.
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * \brief Vypocita Euklidovsku vzdialenost medzi dvoma objektmi.
 *
 * \param *o1 Ukazatel na prvy objekt.
 * \param *o2 Ukazatel na druhy objekt.
 * \pre Ukazatel '*o1' neukazuje na NULL.
 * \pre Ukazatel '*o2' neukazuje na NULL.
 * \return Funkcia vracia vzdialenost.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * \brief Pocita vzdialenost dvoch zhlukov.
 *
 * Vypocet priemernej vzdialenosti medzi kazdou dvojicou
 * z prveho a druheho zhluku.
 * \param *o1 Ukazatel na prvy zhluk.
 * \param *o2 Ukazatel na druhy zhluk.
 * \pre Ukazatel '*o1' neukazuje na NULL.
 * \pre Ukazatel '*o2' neukazuje na NULL.
 * \return Funkcia vracia vzdialenost.
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * \brief Funkcia hlada dva najblizsie zhluky
 *
 * V poli zhlukov 'carr' o velkosti 'narr' hlada dva najblzsie zhluky.
 * Najdene zhluky identifikuje a ich indexy v poli 'carr'. Funkcia
 * najdene zhluky(indexy do pola 'carr') uklada do pamati na adresu
 * 'c1' resp. 'c2'.
 * \param *carr Ukazatel na pole zhlukov.
 * \param narr Velkost pola zhlukov.
 * \param *c1 Ukazatel na prvy z dvoch najblizsich objektov.
 * \param *c2 Ukazatel na druhy objekt.
 * \pre 'narr' je vacsi ako 0.
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * \brief Radenie objektov v zhluku vzostupne podla ich identifikatoru.
 * \param *c Ukazatel na zhluk ktory sa ma byt zoradeny.
 */
void sort_cluster(struct cluster_t *c);

/**
 * \brief Tlacenie zhluku 'c' na stdout.
 * \param *c Ukazatel na zhluk ktory sa ma tlacit.
 */
void print_cluster(struct cluster_t *c);

/**
 * \brief Nacita objekty zo suboru.
 *
 * Zo suboru 'filename' nacita objekty. Pre kazdy objekty vytvori
 * zhluk a ulozi ich do pola zhlukov. Alokuje priestor pre pole vsetkych
 * zhlukov a ukazatel na prvu polozu pola(ukazatel na prvi zhluk v
 * alokovanom poli) ulozi do pamati, kam sa odkazuje na parameter 'arr'.
 * Funkcia vracia pocet nacitanych objektov (zhlukov).
 * \param *filename Nazov suboru.
 * \param **arr Ukazatel na pole zhlukov.
 * \pre Subor zo spravnymi parametrami.
 * \post Vytvorene pole je potrebne na zhlukovanie.
 * \return Funkcia vracia pocet nacitanych zhlukov.
 */
int load_clusters(char *filename, struct cluster_t **arr);

/**
 * \brief Tisk pola zhlukov.
 * 
 * \param *carr Ukazatel na prvu polozku (zhluk).
 * \param narr Pocet zhlukov ktore budu vytlacene.
 */
void print_clusters(struct cluster_t *carr, int narr);
/** \} */
