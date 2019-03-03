/* Linearne viazany zoznam
 * Autor: Peter Hornak
 */

#include <iostream>

using namespace std;

enum {vacsi, mensi, rovny};

//Trieda pre reprezentovanie dat v uzloch
class data{
public:
    data(int hodnota);
    ~data() {}
    int porovnat(const data &ine_data);
    void zobrazit();
private:
    int m_hodnota;
};

//Iba inicializuje
data::data(int hodnota):m_hodnota(hodnota){
}

//Porovna hodnotu 2 dat
int data::porovnat(const data &ine_data){
    if(m_hodnota > ine_data.m_hodnota)
        return vacsi;
    if(m_hodnota < ine_data.m_hodnota)
        return mensi;
    else
        return rovny;
}

void data::zobrazit(){
    cout << m_hodnota << endl;
}

//Objekt uzlu v zozname
class uzol{
public:
    uzol() {}
    virtual ~uzol() {}
    virtual uzol *vlozit(data *nove_data) = 0;
    virtual void zobrazit() = 0;
};

//Odvodena trieda reprezentujuca vnutorny uzol
class vnutorny_uzol: public uzol{
public:
    vnutorny_uzol(data *nove_data, uzol *dalsi);
    ~vnutorny_uzol();
    virtual uzol *vlozit(data *nove_data);
    virtual void zobrazit();
private:
    data *m_data;
    uzol *m_dalsi;
};

//Konstruktor iba inicializuje
vnutorny_uzol::vnutorny_uzol(data *nove_data, uzol *dalsi):m_data(nove_data), m_dalsi(dalsi){
}

vnutorny_uzol::~vnutorny_uzol(){
    delete m_data;
    delete m_dalsi;
}

//Pri vkladani dat porovnava 
uzol *vnutorny_uzol::vlozit(data *nove_data){
    int porovnanie = m_data->porovnat(*nove_data);

    switch (porovnanie){
        case rovny:
        case vacsi:{
                vnutorny_uzol *novy_uzol = new vnutorny_uzol(nove_data, this);
                return novy_uzol;
            }
        case mensi:{
            m_dalsi = m_dalsi->vlozit(nove_data);
            return this;
            }
    }
    return this;
}

//Metoda ktora zobrazi data a zavola tu istu metodu pre dalsi objekt
void vnutorny_uzol::zobrazit(){
    m_data->zobrazit();
    m_dalsi->zobrazit();
}

//Odvodena trieda reprezentujuca chvost zoznamu
class posledny_uzol: public uzol{
public:
    posledny_uzol() {}
    ~posledny_uzol() {}
    virtual uzol *vlozit(data *nove_data);
    virtual void zobrazit() {}
};

//Vyvori novy uzol kt. ukazuje na chvost
uzol * posledny_uzol::vlozit(data *nove_data){
    vnutorny_uzol *novy_uzol = new vnutorny_uzol(nove_data, this);
    return novy_uzol;
}

//Odvodena trieda reprezentujuca hlavu zoznamu
class prvy_uzol: public uzol{
public:
    prvy_uzol();
    ~prvy_uzol();
    virtual uzol *vlozit(data *nove_data);
    virtual void zobrazit();
private:
    uzol *m_dalsi;
};

//Vytvori zaroven aj chvost zoznamu
prvy_uzol::prvy_uzol(){
    m_dalsi = new posledny_uzol;
}

prvy_uzol::~prvy_uzol(){
    delete m_dalsi;
}

//Preda riadenie dalsiemu uzlu
uzol *prvy_uzol::vlozit(data *nove_data){
    m_dalsi = m_dalsi->vlozit(nove_data);
    return this;
}

//Preda riadenie dalsiemu uzlu
void prvy_uzol::zobrazit(){
    m_dalsi->zobrazit();
}

//Trieda reprezentujuca zoznam
class zoznam{
public:
    zoznam();
    ~zoznam();
    void vlozit(data *nove_data);
    void zobrazit();
private:
    prvy_uzol *hlava;
};

zoznam::zoznam(){
    hlava = new prvy_uzol;
}

zoznam::~zoznam(){
    delete hlava;
}

void zoznam::vlozit(data *nove_data){
    hlava->vlozit(nove_data);
}

void zoznam::zobrazit(){
    hlava->zobrazit();
}

int main()
{
    zoznam z;
    data *n_data;
    int hodnota;

    cout << "Pre ukoncenie vkladania vlozte 0" << endl;
    while(true){
        cout << "Vlozte hodnotu: ";
        cin >> hodnota;
        if(!hodnota)
            break;
        n_data = new data(hodnota);
        z.vlozit(n_data);
    }
    z.zobrazit();
    return 0;
}
