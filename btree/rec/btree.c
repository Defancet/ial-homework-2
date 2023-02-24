/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */

// inicializuje strom
void bst_init(bst_node_t **tree) {
    if ((*tree) == NULL) {     //check if tree is empty
        return;
    } else {
        (*tree) = NULL;    //set tree to NULL
    }
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */

// projde strom a vrátí true, pokud je klíč nalezen, jinak vrátí false
bool bst_search(bst_node_t *tree, char key, int *value) {
    if (tree == NULL) {    //check if tree is empty
        return false;
    } else if (tree->key < key) {   //if key is bigger than tree key go to right
        return bst_search(tree->right, key, value);
    } else if (tree->key > key) {   //if key is smaller than tree key go to left
        return bst_search(tree->left, key, value);
    } else {
        *value = tree->value;   //if key is equal to tree key store the value
        return true;
    }
    return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */

// vloží do stromu nový uzel s klíčem key a hodnotou value
void bst_insert(bst_node_t **tree, char key, int value) {
    if ((*tree) == NULL) {    //check if tree is empty
        (*tree) = (bst_node_t *) malloc(sizeof(struct bst_node));    //allocate memory for element
        if (*tree == NULL) {    //check if memory allocation was successful
            return;
        }
        (*tree)->key = key;        //store key
        (*tree)->value = value;    //store value
        (*tree)->left = NULL;      //set left child to NULL
        (*tree)->right = NULL;     //set right child to NULL
        return;
    } else if ((*tree)->key < key) {    //if key is bigger than tree key go to right
        bst_insert(&(*tree)->right, key, value);
    } else if ((*tree)->key > key) {    //if key is smaller than tree key go to left
        bst_insert(&(*tree)->left, key, value);
    } else {
        (*tree)->value = value;         //if key is equal to tree key store the value
        return;
    }
}

/*
* Pomocná funkce která nahradí uzel nejpravějším potomkem.
  *
  * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího uzlu podstromu tree.
  * Nejpravější potomek bude odstraněn. Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
  *
  * Funkce předpokládá že hodnota tree není NULL.
  *
  * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
  *
  * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */

// nahradí uzel target najpravejším potomkom podstromu tree a odstráni tento uzel
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    if ((*tree)->right == NULL) {    //check if right child is empty
        target->key = (*tree)->key;    //store key
        target->value = (*tree)->value;    //store value
        bst_node_t *tmp = (*tree);    //create temporary pointer to tree
        (*tree) = (*tree)->left;    //set tree to left child
        free(tmp);    //free memory
        return;
    } else {        //if right child is not empty go to right
        bst_replace_by_rightmost(target, &(*tree)->right);    //recursive function calling for right child
    }
}

/*
  * Odstranění uzlu ve stromu.
  *
  * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
  * Pokud má odstraněný uzel jeden podstrom, zdědí jej otec odstraněného uzlu.
  * Pokud má odstraněný uzel oba podstromy, je nahrazen nejpravějším uzlem
  * levého podstromu. Nejpravější uzel nemusí být listem!
  * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
  *
  * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
  * použití vlastních pomocných funkcí.
 */

// odstráni uzel s klíčem key
void bst_delete(bst_node_t **tree, char key) {
    if ((*tree) == NULL) {    //check if tree is empty
        return;
    } else if ((*tree)->key < key) {    //if key is bigger than tree key go to right
        bst_delete(&(*tree)->right, key);
    } else if ((*tree)->key > key) {    //if key is smaller than tree key go to left
        bst_delete(&(*tree)->left, key);
    } else {    //if key is equal to tree key
        if ((*tree)->left == NULL && (*tree)->right == NULL) {    //check if tree has no children
            free(*tree);    //free memory
            (*tree) = NULL;    //set tree to NULL
            return;
        } else if ((*tree)->left == NULL) {    //check if tree has only right child
            bst_node_t *tmp = (*tree);    //store pointer to element
            (*tree) = (*tree)->right;    //set tree to right child
            free(tmp);    //free memory
            return;
        } else if ((*tree)->right == NULL) {    //check if tree has only left child
            bst_node_t *tmp = (*tree);    //store pointer to element
            (*tree) = (*tree)->left;    //set tree to left child
            free(tmp);    //free memory
            return;
        } else {    //if tree has two children
            bst_replace_by_rightmost((*tree), &(*tree)->left);    //replace tree with rightmost element
            return;
        }
    }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */

// zruší celý strom
void bst_dispose(bst_node_t **tree) {
    if ((*tree) == NULL) {    //check if tree is empty
        return;
    } else {    //if tree is not empty
        bst_dispose(&(*tree)->left);    //recursive function call for left child
        bst_dispose(&(*tree)->right);    //recursive function call for right child
        free(*tree);    //free memory
        (*tree) = NULL;    //set tree to NULL
        return;
    }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */

// preorder prechod stromom
void bst_preorder(bst_node_t *tree) {
    if (tree == NULL) {    //check if tree is empty
        return;
    } else {    //if tree is not empty
        bst_print_node(tree);    //print node
        bst_preorder(tree->left);    //recursive function call for left child
        bst_preorder(tree->right);    //recursive function call for right child
        return;
    }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */

// inorder prechod stromom
void bst_inorder(bst_node_t *tree) {
    if (tree == NULL) {    //check if tree is empty
        return;
    } else {    //if tree is not empty
        bst_inorder(tree->left);    //recursive function call for left child
        bst_print_node(tree);    //print node
        bst_inorder(tree->right);    //recursive function call for right child
        return;
    }
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */

// postorder prechod stromom
void bst_postorder(bst_node_t *tree) {
    if (tree == NULL) {    //check if tree is empty
        return;
    } else {    //if tree is not empty
        bst_postorder(tree->left);    //recursive function call for left child
        bst_postorder(tree->right);    //recursive function call for right child
        bst_print_node(tree);    //print node
        return;
    }
}
