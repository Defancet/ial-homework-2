/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */

// nastaví všechny položky v tabulce na hodnotu NULL
void ht_init(ht_table_t *table) {
    if (table == NULL) {    //if table is NULL, return
        return;
    } else {
        for (int i = 0; i < HT_SIZE; i++) {     //initialize all items to NULL
            (*table)[i] = NULL;
        }
    }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */

// projde všechny položky v tabulce, pokud najde shodný klíč, vrátí ukazatel na položku v tabulce
ht_item_t *ht_search(ht_table_t *table, char *key) {
    if (table == NULL || key == NULL) {     //if table or key is NULL, return NULL
        return NULL;
    } else {
        ht_item_t *item = (*table)[get_hash(key)];      //položka na indexu key
        while (item != NULL && item->key != key) {      //while the item is not null and the key is not the same
            item = item->next;                          //go to the next item
        }
        return item;    //return item
    }
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */

// vloží novou položku do tabulky, pokud již položka s daným klíčem existuje, nahradí ji novou
void ht_insert(ht_table_t *table, char *key, float value) {
    if (table == NULL || key == NULL) {     //if table or key is NULL, return
        return;
    } else {
        ht_item_t *item = ht_search(table, key);     //položka na indexu key
        if (item != NULL) {     //if the item is not null
            item->value = value;    //set the value
        } else {
            ht_item_t *new_item = (ht_item_t *) malloc(sizeof(struct ht_item));     //allocate memory for new item
            if (new_item == NULL) {     //check if memory allocation was successful
                return;
            } else {
                new_item->key = key;    //set the key
                new_item->value = value;    //set the value
                new_item->next = (*table)[get_hash(key)];     //set the next item to the item at the hash index
                (*table)[get_hash(key)] = new_item;   //set the item at the hash index to the new item
            }
        }
    }
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */

// vrátí hodnotu položky s daným klíčem
float *ht_get(ht_table_t *table, char *key) {
    if (table == NULL || key == NULL) {     //if table or key is NULL, return NULL
        return NULL;
    } else {
        ht_item_t *item = ht_search(table, key);     //get the item at the hash index
        if (ht_search(table, key) != NULL) {     //if the item is not NULL
            return (&(item->value));  //return the value
        } else {
            return NULL;    //return NULL
        }
    }
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */

// smaže položku s daným klíčem
void ht_delete(ht_table_t *table, char *key) {
    if (table == NULL || key == NULL) {     //if table or key is NULL, return
        return;
    } else {
        ht_item_t *item = (*table)[get_hash(key)];      //get the item at the hash index
        if (item == NULL) {     //if the item is null, return
            return;
        } else if (item->key == key) {      //if the key is the same
            (*table)[get_hash(key)] = item->next;      //set the item at the hash index to the next item
            free(item);     //free the item
        } else {
            while (item->next != NULL &&
                   item->next->key != key) {      //while the next item is not null and the key is not the same
                item = item->next;      //go to the next item
            }
            if (item->next != NULL) {       //if the next item is not null
                ht_item_t *temp = item->next;       //set temp to the next item
                item->next = item->next->next;      //set the next item to the next item's next item
                free(temp);     //free temp
            }
        }
    }
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */

// smaže všechny položky v tabulce a nastaví je na NULL
void ht_delete_all(ht_table_t *table) {
    if (table == NULL) {    //if table is NULL, return
        return;
    } else {
        for (int i = 0; i < HT_SIZE; i++) {     //for every item in the table
            ht_item_t *item = (*table)[i];      //get the item at the hash index
            while (item != NULL) {      //while the item is not null
                ht_item_t *temp = item;     //set temp to the item
                item = item->next;      //go to the next item
                free(temp);     //free temp
            }
            (*table)[i] = NULL;     //set the item at the hash index to null
        }
    }
}
