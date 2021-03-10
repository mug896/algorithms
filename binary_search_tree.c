#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

typedef struct bs_node {
    int data;
    struct bs_node *left;
    struct bs_node *right;
    struct bs_node *parent;
} bs_node;

typedef struct bs_tree {
    bs_node *root;
} bs_tree;

bs_node *new_bs_node (int data) 
{
    bs_node *n = malloc(sizeof(bs_node));
    n->data = data;
    n->left = NULL;
    n->right = NULL;
    n->parent = NULL;
    return n;
}

bs_tree *new_bs_tree() 
{
    bs_tree *t = malloc (sizeof (bs_tree));
    t->root = NULL;
    return t;
}

bs_node* minimum (bs_tree *t, bs_node *x) 
{
    while (x->left != NULL)
        x = x->left;
    return x;
}

void insert (bs_tree *t, bs_node *n) 
{
    bs_node *p = NULL;
    bs_node *temp = t->root;
    while (temp != NULL) {
        p = temp;
        if (n->data < temp->data)
            temp = temp->left;
        else
            temp = temp->right;
    }
    n->parent = p;

    if (p == NULL) //newly added node is root
        t->root = n;
    else if (n->data < p->data)
        p->left = n;
    else
        p->right = n;
}

void transplant (bs_tree *t, bs_node *u, bs_node *v) 
{
    if (u->parent == NULL) //u is root
        t->root = v;
    else if (u == u->parent->left) //u is left child
        u->parent->left = v;
    else //u is right child
        u->parent->right = v;

    if (v != NULL)
        v->parent = u->parent;
}

void delete (bs_tree *t, bs_node *z) 
{
    if (z->left == NULL) {                   // one degree
        transplant (t, z, z->right);
        free(z);
    }
    else if (z->right == NULL) {             // one degree
        transplant (t, z, z->left);
        free(z);
    }
    else {                                   // two degree
        bs_node *y = minimum (t, z->right);  // inorder successor
        if (y->parent != z) {
            transplant (t, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant (t, z, y);
        y->left = z->left;
        y->left->parent = y;
        free(z);
    }
}




/////////////////////////////   tree 관련 함수들  ////////////////////////////

void pre_order (bs_node *n) 
{
    if (n != NULL) {
        printf ("%d ", n->data);
        pre_order (n->left);
        pre_order (n->right);
    }
}

void in_order (bs_node *n) 
{
    if (n != NULL) {
        in_order (n->left);
        printf ("%d ", n->data);
        in_order (n->right);
    }
}

bs_node *search (bs_tree *t, int data) 
{
    bs_node *node = t->root;
    while (node != NULL) {
        if (data < node->data)
            node = node->left;
        else if (data > node->data)
            node = node->right;
        else
            return node;
    }
    return NULL;
}

bs_node *first_node (bs_tree *t)
{
    bs_node *node = t->root;
    if (node == NULL)
        return NULL;
    while (node->left)
        node = node->left;
    return node;
}

bs_node *last_node (bs_tree *t)
{
    bs_node *node = t->root;
    if (node == NULL)
        return NULL;
    while (node->right)
        node = node->right;
    return node;
}

bs_node *next_node (bs_node *node)
{
    if (node == NULL) 
        return NULL;
    if (node->right == NULL) {
        if (node->parent && node == node->parent->left)
            return node->parent;
        while (node->parent && node == node->parent->right)
            node = node->parent;
        return node->parent;
    }
    node = node->right;
    while (node->left != NULL)
        node = node->left;
    return node;
}

bs_node *prev_node (bs_node *node)
{
    if (node == NULL) 
        return NULL;
    if (node->left == NULL) {
        if (node->parent && node == node->parent->right)
            return node->parent;
        while (node->parent && node == node->parent->left)
            node = node->parent;
        return node->parent;
    }
    node = node->left;
    while (node->right != NULL)
        node = node->right;
    return node;
}

int tree_height (bs_node *n)
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

int count_all_node (bs_node *p)
{
    if (p == NULL)
        return 0;
    return count_all_node (p->left) + count_all_node (p->right) + 1;
}

int count_2degree (bs_node *n)
{
    if (n == NULL)
        return 0;
    if (n->left && n->right)
        return count_2degree (n->left) + count_2degree (n->right) + 1;
    else
        return count_2degree (n->left) + count_2degree (n->right);
}

int count_1degree (bs_node *n)
{
    if (n == NULL)
        return 0;
    if (n->left == NULL ^ n->right == NULL)
        return count_1degree (n->left) + count_1degree (n->right) + 1;
    else
        return count_1degree (n->left) + count_1degree (n->right);
}

int count_leaf (bs_node *n)
{
    if (n == NULL)
        return 0;
    if (n->left == NULL && n->right == NULL)
        return count_leaf (n->left) + count_leaf (n->right) + 1;
    else
        return count_leaf (n->left) + count_leaf (n->right);
}

int count_internal (bs_node *n)
{
    if (n == NULL)
        return 0;
    if (n->left || n->right)
        return count_internal (n->left) + count_internal (n->right) + 1;
    else
        return count_internal (n->left) + count_internal (n->right);
}

int sum_all (bs_node *n)
{
    if (n == NULL)
        return 0;
    return sum_all (n->left) + sum_all (n->right) + n->data;
}


bool is_binary_search_tree (bs_node *n, int min, int max)  
{  
    if (n == NULL)
        return true;
    
    if (n->data < min || n->data > max)  
        return false;
  
    return 
        is_binary_search_tree (n->left, min, n->data - 1)
        && is_binary_search_tree (n->right, n->data + 1, max);
}  

bool is_full_binary_tree (bs_node *n) 
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

int depth (bs_node *n) {
    int d = 0;
    while (n != NULL) {
        d++;
        n = n->left;
    }
    return d;
}

bool is_perfect_binary_tree (bs_node *n, int d, int level) 
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

bool is_complete_binary_tree (bs_node *n, int index, int nr_nodes) 
{
    if (n == NULL)
        return true;

    if (index >= nr_nodes)
        return false;

    return (is_complete_binary_tree (n->left, 2 * index + 1, nr_nodes) 
         && is_complete_binary_tree (n->right, 2 * index + 2, nr_nodes));
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



#include "randoms.h"
//int data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
bs_node *arr [1000000];

int main() 
{
    int total = sizeof (data) / sizeof (data[0]);
    int i = 0;

    bs_tree *t = new_bs_tree();

    TIME_START();
    for (i = 0; i < total; i++) {
        arr[i] = new_bs_node (data[i]);
        insert (t, arr[i]);
    }
    TIME_END("Insert DONE");

    TIME_START();
    for (i = 0; i < total; i++) {
        if (search (t, arr[i]->data) == NULL)
            printf ("node NOT found : %d\n", arr[i]->data);
    }
    TIME_END("Search DONE");

    bs_node *tmp = first_node (t);
    printf ("first node : %d\n", tmp ? tmp->data : 0);
    tmp = last_node (t);
    printf ("last node : %d\n", tmp ? tmp->data : 0);
    tmp = prev_node (arr[3]);
    printf ("prev node : %d\n", tmp ? tmp->data : 0);


    printf ("tree height : %d\n", tree_height (t->root));
    printf ("node count all : %d\n", count_all_node (t->root));
    printf ("node count 2degree : %d\n", count_2degree (t->root));
    printf ("node count 1degree : %d\n", count_1degree (t->root));
    printf ("node count leaf : %d\n", count_leaf (t->root));
    printf ("node count internal : %d\n", count_internal (t->root));
    printf ("sum all node value : %d\n", sum_all (t->root));


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
