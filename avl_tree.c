#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

typedef struct avl_node {
    int data;
    struct avl_node *left;
    struct avl_node *right;
    struct avl_node *parent;
    int height;
} avl_node;

typedef struct avl_tree {
    avl_node *root;
} avl_tree;

avl_node *new_avl_node (int data) 
{
    avl_node *n = malloc (sizeof (avl_node));
    n->data = data;
    n->left = NULL;
    n->right = NULL;
    n->parent = NULL;
    n->height = 0;

    return n;
}

avl_tree *new_avl_tree() 
{
    avl_tree *t = malloc (sizeof (avl_tree));
    t->root = NULL;
    return t;
}

int max (int a, int b) 
{
    return a > b ? a : b;
}

int height (avl_node *n) 
{
    return n ? n->height : -1;
}

avl_node *minimum (avl_tree *t, avl_node *x) // inorder successor
{
    while (x->left != NULL)
        x = x->left;
    return x;
}

void left_rotate (avl_tree *t, avl_node *x) 
{
    avl_node *y = x->right;
    x->right = y->left;

    if (y->left != NULL)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == NULL)   //x is root
        t->root = y;
    else if (x == x->parent->left)   //x is left child
        x->parent->left = y;
    else   //x is right child
        x->parent->right = y;

    y->left = x;
    x->parent = y;

    x->height = 1 + max (height (x->left), height (x->right));
    y->height = 1 + max (height (y->left), height (y->right));
}

void right_rotate (avl_tree *t, avl_node *x) 
{
    avl_node *y = x->left;
    x->left = y->right;

    if (y->right != NULL)
        y->right->parent = x;

    y->parent = x->parent;

    if (x->parent == NULL)   //x is root
        t->root = y;
    else if (x == x->parent->right)   //x is left child
        x->parent->right = y;
    else   //x is right child
        x->parent->left = y;

    y->right = x;
    x->parent = y;

    x->height = 1 + max (height (x->left), height (x->right));
    y->height = 1 + max (height (y->left), height (y->right));
}

int balance_factor (avl_node *n)
{
    if (n == NULL)
        return 0;
    return height (n->left) - height (n->right);
}

void insert (avl_tree *t, avl_node *n) 
{
    avl_node *y = NULL;
    avl_node *temp = t->root;
    while (temp != NULL) {
        y = temp;
        if (n->data < temp->data)
            temp = temp->left;
        else
            temp = temp->right;
    }
    n->parent = y;

    if (y == NULL) //newly added node is root
        t->root = n;
    else if (n->data < y->data)
        y->left = n;
    else
        y->right = n;

    avl_node *z = n;

    while (y != NULL) {
        y->height = 1 + max (height (y->left), height (y->right));

        avl_node *x = y->parent;

        if (balance_factor(x) <= -2 || balance_factor(x) >= 2) { //grandparent is unbalanced
            if (y == x->left) {
                if (z == x->left->left)  //case 1
                    right_rotate (t, x);
                else if (z == x->left->right) {  //case 3
                    left_rotate (t, y);
                    right_rotate (t, x);
                }
            }
            else if (y == x->right) {
                if (z == x->right->right)  //case 2
                    left_rotate (t, x);
                else if (z == x->right->left) {  //case 4
                    right_rotate (t, y);
                    left_rotate (t, x);
                }
            }
            break;
        }
        y = y->parent;
        z = z->parent;
    }
}

void transplant (avl_tree *t, avl_node *u, avl_node *v) 
{
    if (u->parent == NULL)  //u is root
        t->root = v;
    else if (u == u->parent->left)  //u is left child
        u->parent->left = v;
    else  //u is right child
        u->parent->right = v;

    if (v != NULL)
        v->parent = u->parent;
}

void delete_fixup (avl_tree *t, avl_node *n) 
{
    avl_node *p = n;

    while (p != NULL) 
    {
        p->height = 1 + max (height (p->left), height (p->right));

        if (balance_factor(p) <= -2 || balance_factor(p) >= 2) { //grandparent is unbalanced
            avl_node *x, *y, *z;
            x = p;

            //taller child of x will be y
            if (height (x->left) > height (x->right))
                y = x->left;
            else
                y = x->right;

            //taller child of y will be z
            if (height (y->left) > height (y->right))
                z = y->left;
            else if (height (y->left) < height (y->right))
                z = y->right;
            else { //same height, go for single rotation
                if (y == x->left)
                    z = y->left;
                else
                    z = y->right;
            }

            if (y == x->left) {
                if (z == x->left->left) //case 1
                    right_rotate (t, x);
                else if (z == x->left->right) {  //case 3
                    left_rotate (t, y);
                    right_rotate (t, x);
                }
            }
            else if (y == x->right) {
                if (z == x->right->right) //case 2
                    left_rotate (t, x);
                else if (z == x->right->left) {  //case 4
                    right_rotate (t, y);
                    left_rotate (t, x);
                }
            }
        }
        p = p->parent;
    }
}

void delete (avl_tree *t, avl_node *z) 
{
    if  (z->left == NULL) {
        transplant (t, z, z->right);
        if (z->right != NULL)
            delete_fixup (t, z->right);
        free(z);
    }
    else if (z->right == NULL) {
        transplant (t, z, z->left);
        if (z->left != NULL)
            delete_fixup (t, z->left);
        free(z);
    }
    else {
        avl_node *y = minimum (t, z->right); //minimum element in right subtree
        if (y->parent != z) {
            transplant (t, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant (t, z, y);
        y->left = z->left;
        y->left->parent = y;
        if (y != NULL)
            delete_fixup (t, y);
        free(z);
    }
}




/////////////////////////////   tree 관련 함수들  ////////////////////////////

void pre_order (avl_node *n)
{
    if (n != NULL) {
        printf ("%d ", n->data);
        pre_order (n->left);
        pre_order (n->right);
    }
}

void in_order (avl_node *n) 
{
    if (n != NULL) {
        in_order (n->left);
        printf ("%d ", n->data);
        in_order (n->right);
    }
}

avl_node *search (avl_tree *t, int data) 
{
    avl_node *n = t->root;
    while (n != NULL) {
        if (data < n->data)
            n = n->left;
        else if (data > n->data)
            n = n->right;
        else
            return n;
    }
    return NULL;
}

avl_node *first_node (avl_tree *t)
{
    avl_node *n = t->root;
    if (n == NULL)
        return NULL;
    while (n->left)
        n = n->left;
    return n;
}

avl_node *last_node (avl_tree *t)
{
    avl_node *n = t->root;
    if (n == NULL)
        return NULL;
    while (n->right)
        n = n->right;
    return n;
}

avl_node *next_node (avl_node *n)
{
    if (n == NULL) 
        return NULL;
    if (n->right == NULL) {
        if (n->parent && n == n->parent->left)
            return n->parent;
        while (n->parent && n == n->parent->right)
            n = n->parent;
        return n->parent;
    }
    n = n->right;
    while (n->left)
        n = n->left;
    return n;
}

avl_node *prev_node (avl_node *n)
{
    if (n == NULL) 
        return NULL;
    if (n->left == NULL) {
        if (n->parent && n == n->parent->right)
            return n->parent;
        while (n->parent && n == n->parent->left)
            n = n->parent;
        return n->parent;
    }
    n = n->left;
    while (n->right)
        n = n->right;
    return n;
}

int tree_height (avl_node *n)
{
    if (n == NULL)
        return 0;
    int x = tree_height (n->left);
    int y = tree_height (n->right);
    if (x > y)
        return x + 1;
    else
        return y + 1;
}

int count_all_node (avl_node *n)
{
    if (n == NULL)
        return 0;
    return count_all_node (n->left) + count_all_node (n->right) + 1;
}

int count_2degree (avl_node *n)
{
    if (n == NULL)
        return 0;
    if (n->left && n->right)
        return count_2degree (n->left) + count_2degree (n->right) + 1;
    else
        return count_2degree (n->left) + count_2degree (n->right);
}

int count_1degree (avl_node *n)
{
    if (n == NULL)
        return 0;
    if (n->left == NULL ^ n->right == NULL)
        return count_1degree (n->left) + count_1degree (n->right) + 1;
    else
        return count_1degree (n->left) + count_1degree (n->right);
}

int count_leaf (avl_node *n)
{
    if (n == NULL)
        return 0;
    if (n->left == NULL && n->right == NULL)
        return count_leaf (n->left) + count_leaf (n->right) + 1;
    else
        return count_leaf (n->left) + count_leaf (n->right);
}

int count_internal (avl_node *n)
{
    if (n == NULL)
        return 0;
    if (n->left || n->right)
        return count_internal (n->left) + count_internal (n->right) + 1;
    else
        return count_internal (n->left) + count_internal (n->right);
}

int sum_all_data (avl_node *n)
{
    if (n == NULL)
        return 0;
    return sum_all_data (n->left) + sum_all_data (n->right) + n->data;
}


bool is_binary_search_tree (avl_node *n, int min, int max)  
{  
    if (n == NULL)
        return true;
    
    if (n->data < min || n->data > max)  
        return false;
  
    return 
        is_binary_search_tree (n->left, min, n->data - 1)
        && is_binary_search_tree (n->right, n->data + 1, max);
}  

bool is_full_binary_tree (avl_node *n) 
{
    if (n == NULL)
        return true;

    if (n->left == NULL && n->right == NULL)
        return true;

    if (n->left && n->right)
        return is_full_binary_tree (n->left) 
            && is_full_binary_tree (n->right);

    return false;
}

int depth (avl_node *n) {
    int d = 0;
    while (n != NULL) {
        d++;
        n = n->left;
    }
    return d;
}

bool is_perfect_binary_tree (avl_node *n, int d, int level) 
{
    if (n == NULL)
        return true;

    if (n->left == NULL && n->right == NULL)
        return (d == level + 1);

    if (n->left == NULL || n->right == NULL)
        return false;

    return is_perfect_binary_tree (n->left, d, level + 1) 
        && is_perfect_binary_tree (n->right, d, level + 1);
}

bool is_complete_binary_tree (avl_node *n, int index, int nr_nodes) 
{
    if (n == NULL)
        return true;

    if (index >= nr_nodes)
        return false;

    return is_complete_binary_tree (n->left, 2 * index + 1, nr_nodes) 
        && is_complete_binary_tree (n->right, 2 * index + 2, nr_nodes);
}




////////////////////////////////  main 함수  //////////////////////////////

struct timespec tstart, tend;
double time_total = 0;
char time_str [1024] = {}, 
     *ptr = time_str;

#define TIME_START() clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &tstart)

#define TIME_END( $msg ) do { \
    clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &tend); \
    double res = ((double)tend.tv_sec + 1.0e-9 * tend.tv_nsec) - \
                 ((double)tstart.tv_sec + 1.0e-9 * tstart.tv_nsec); \
    ptr += sprintf (ptr, "%d " $msg " : %.5f seconds\n", i, res); \
    time_total += res; \
} while (0)

#define TIME_RESULT() printf ("=========================================\n%s\
Insert + Search + Delete : %.5f seconds\n", time_str, time_total)


//int data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
#include "randoms.h"

avl_node *arr [1000000];

int main() 
{
    int total = sizeof (data) / sizeof (data[0]);
    int i = 0;

    avl_tree *t = new_avl_tree();

    TIME_START();
    for (i = 0; i < total; i++) {
        arr[i] = new_avl_node (data[i]);
        insert (t, arr[i]);
    }
    TIME_END("Insert DONE");

    TIME_START();
    for (i = 0; i < total; i++) {
        if (search (t, arr[i]->data) == NULL)
            printf ("node NOT found : %d\n", arr[i]->data);
    }
    TIME_END("Search DONE");

    avl_node *tmp = first_node (t);
    printf ("first node : %d\n", tmp ? tmp->data : 0);
    tmp = last_node (t);
    printf ("last node : %d\n", tmp ? tmp->data : 0);
    tmp = prev_node (arr[3]);
    printf ("prev node : %d\n", tmp ? tmp->data : 0);
    tmp = next_node (arr[3]);
    printf ("next node : %d\n", tmp ? tmp->data : 0);

    printf ("tree height : %d\n", tree_height (t->root));
    printf ("node count all : %d\n", count_all_node (t->root));
    printf ("node count 2degree : %d\n", count_2degree (t->root));
    printf ("node count 1degree : %d\n", count_1degree (t->root));
    printf ("node count leaf : %d\n", count_leaf (t->root));
    printf ("node count internal : %d\n", count_internal (t->root));
    printf ("sum all node value : %d\n", sum_all_data (t->root));


    printf ("The tree is %s tree\n",
            is_binary_search_tree (t->root, INT_MIN, INT_MAX) ?
            "BINARY SEARCH" : "NOT BINARY SEARCH");

    printf ("The tree is %s binary tree\n",
            is_full_binary_tree (t->root) ? "FULL" : "NOT FULL");
    
    printf ("The tree is %s binary tree\n",
            is_perfect_binary_tree (t->root, depth (t->root), 0) ?
            "PERFECT" : "NOT PERFECT");

    printf ("The tree is %s binary tree\n",
            is_complete_binary_tree (t->root, 0, count_all_node (t->root)) ?
            "COMPLETE" : "NOT COMPLETE");

    //pre_order (t->root); puts ("");
    //in_order (t->root); puts ("");

    TIME_START();
    for (i = 0; i < total; i++) {
        delete (t, arr[i]);
    }
    TIME_END("Delete DONE");

    TIME_RESULT();

    free (t);
    return 0;
}
