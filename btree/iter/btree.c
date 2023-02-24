/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
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
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */

// projde strom a vrátí true, pokud je klíč nalezen, jinak vrátí false
bool bst_search(bst_node_t *tree, char key, int *value) {
    if (tree == NULL) {    //check if tree is empty
        return false;
    } else {
        while (tree != NULL) {  //while tree is not empty
            if (tree->key < key) {  //if key is bigger than tree key go to right
                tree = tree->right;
            } else if (tree->key > key) {   //if key is smaller than tree key go to left
                tree = tree->left;
            } else {
                *value = tree->value;   //if key is equal to tree key store the value
                return true;
            }
        }
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
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */

// vloží nový uzel do stromu
void bst_insert(bst_node_t **tree, char key, int value) {
    if ((*tree) == NULL) {  //check if tree is empty
        (*tree) = (bst_node_t *) malloc(sizeof(struct bst_node));    //allocate memory for element
        if (*tree == NULL) {    //check if memory allocation was successful
            return;
        }
        (*tree)->key = key;        //set key
        (*tree)->value = value;    //store value
        (*tree)->left = NULL;      //set left child to NULL
        (*tree)->right = NULL;     //set right child to NULL
    } else {
        while ((*tree) != NULL) {   //while tree is not empty
            if ((*tree)->key < key) {   //if key is bigger than tree key go to right
                tree = &(*tree)->right;
            } else if ((*tree)->key > key) {    //if key is smaller than tree key go to left
                tree = &(*tree)->left;
            } else {
                (*tree)->value = value;     //if key is equal to tree key set value
                return;
            }
        }
        (*tree) = (bst_node_t *) malloc(sizeof(struct bst_node));    //allocate memory for element
        if (*tree == NULL) {    //check if memory allocation was successful
            return;
        }
        (*tree)->key = key;        //set key
        (*tree)->value = value;    //store value
        (*tree)->left = NULL;      //set left child to NULL
        (*tree)->right = NULL;     //set right child to NULL
    }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */

// nahradí uzel najpravejším potomkom
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    if ((*tree) == NULL) {     //check if tree is empty
        return;
    } else {
        while ((*tree)->right != NULL) {
            tree = &(*tree)->right;    //look for the rightmost node
        }
        target->key = (*tree)->key;     //store key
        target->value = (*tree)->value;     //store value
        bst_node_t *tmp = (*tree);      //create temporary pointer to tree
        (*tree) = (*tree)->left;        //set tree to left child
        free(tmp);      //free memory
    }
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */

// odstráni uzel zo stromu
void bst_delete(bst_node_t **tree, char key) {
    if ((*tree) == NULL) {  //check if tree is empty
        return;
    } else {
        while ((*tree) != NULL) {   //while tree is not empty
            if ((*tree)->key < key) {   //if key is bigger than tree key go to right
                tree = &(*tree)->right;
            } else if ((*tree)->key > key) {    //if key is smaller than tree key go to left
                tree = &(*tree)->left;
            } else {    //if key is equal to tree key
                if ((*tree)->left == NULL && (*tree)->right == NULL) {    //if tree has no children
                    free(*tree);    //free memory
                    (*tree) = NULL;     //set tree to NULL
                    return;
                } else if ((*tree)->left == NULL) {     //check if tree has only right child
                    bst_node_t *tmp = (*tree);      //create temporary pointer to tree
                    (*tree) = (*tree)->right;       //set tree to right child
                    free(tmp);      //free memory
                    return;
                } else if ((*tree)->right == NULL) {    //check if tree has only left child
                    bst_node_t *tmp = (*tree);      //create temporary pointer to tree
                    (*tree) = (*tree)->left;        //set tree to left child
                    free(tmp);      //free memory
                    return;
                } else {    //if tree has two children
                    bst_replace_by_rightmost((*tree), &(*tree)->left);     //replace tree with rightmost element
                    return;
                }
            }
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
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */

// zruší celý strom
void bst_dispose(bst_node_t **tree) {
    if ((*tree) == NULL) {  //check if tree is empty
        return;
    } else {    //if tree is not empty
        stack_bst_t stack;    //create stack
        stack_bst_init(&stack);   //initialize stack
        while ((*tree) != NULL || !stack_bst_empty(&stack)) {    //while tree is not empty or stack is not empty
            if ((*tree) == NULL) {      //if tree is empty
                bst_node_t *tmp = stack_bst_top(&stack);     //create temporary pointer to stack top
                stack_bst_pop(&stack);      //pop element from stack
                (*tree) = tmp->right;       //set tree to right child
                free(tmp);      //free memory
            } else {
                stack_bst_push(&stack, (*tree));    //push element to stack
                (*tree) = (*tree)->left;        //set tree to left child
            }
        }
    }
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */

// pomocná funkcia pre iteratívny preorder
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
    if (tree == NULL) {     //check if tree is empty
        return;
    } else {
        while (tree != NULL) {      //while tree is not empty
            bst_print_node(tree);       //print node
            stack_bst_push(to_visit, tree);     //push element to stack
            tree = tree->left;      //set tree to left child
        }
    }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
    if (tree == NULL) {     //check if tree is empty
        return;
    } else {
        stack_bst_t stack;      //create stack
        stack_bst_init(&stack);   //initialize stack
        bst_leftmost_preorder(tree, &stack);     //go to left
        while (!stack_bst_empty(&stack)) {       //while stack is not empty
            bst_node_t *tmp = stack_bst_top(&stack);     //create temporary pointer to stack top
            stack_bst_pop(&stack);      //pop element from stack
            bst_leftmost_preorder(tmp->right, &stack);       //go to right
        }
    }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
    if (tree == NULL) {     //check if tree is empty
        return;
    } else {
        while (tree != NULL) {      //while tree is not empty
            stack_bst_push(to_visit, tree);     //push element to stack
            tree = tree->left;      //set tree to left child
        }
    }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
    if (tree == NULL) {     //check if tree is empty
        return;
    } else {
        stack_bst_t stack;   //create stack
        stack_bst_init(&stack);     //initialize stack
        bst_leftmost_inorder(tree, &stack);      //go to left
        while (!stack_bst_empty(&stack)) {       //while stack is not empty
            bst_node_t *tmp = stack_bst_top(&stack);     //create temporary pointer to stack top
            stack_bst_pop(&stack);      //pop element from stack
            bst_print_node(tmp);        //print node
            bst_leftmost_inorder(tmp->right, &stack);        //go to right
        }
    }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
    if (tree == NULL) {     //check if tree is empty
        return;
    } else {
        while (tree != NULL) {      //while tree is not empty
            stack_bst_push(to_visit, tree);     //push element to stack
            stack_bool_push(first_visit, true);     //push element to stack
            tree = tree->left;      //set tree to left child
        }
    }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
    if (tree == NULL) {     //check if tree is empty
        return;
    } else {
        stack_bst_t stack;    //create stack for nodes
        stack_bst_init(&stack);    //initialize stack
        stack_bool_t stack_bool;    //create stack with flags
        stack_bool_init(&stack_bool);   //initialize stack
        bst_leftmost_postorder(tree, &stack, &stack_bool);       //go to left
        while (!stack_bst_empty(&stack)) {       //while stack is not empty
            bst_node_t *tmp = stack_bst_top(&stack);     //create temporary pointer to stack top
            bool tmp_bool = stack_bool_top(&stack_bool);
            stack_bst_pop(&stack);      //pop element from stack
            stack_bool_pop(&stack_bool);
            if (tmp_bool) {     //if node was visited first time
                stack_bst_push(&stack, tmp);        //push element to stack
                stack_bool_push(&stack_bool, false);
                bst_leftmost_postorder(tmp->right, &stack, &stack_bool);     //go to right
            } else {        //if node was visited second time
                bst_print_node(tmp);        //print node
            }
        }
    }
}
