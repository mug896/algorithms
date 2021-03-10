#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

enum COLOR {Red, Black};

typedef struct rb_node {
    int data;
    struct rb_node *right;
    struct rb_node *left;
    struct rb_node *parent;
    enum COLOR color;
} rb_node;

typedef struct rb_tree {
    rb_node *root;
    rb_node *NIL;
} rb_tree;

rb_node *new_rb_node (int data) 
{
    rb_node *n = malloc (sizeof (rb_node));
    n->left = NULL;
    n->right = NULL;
    n->parent = NULL;
    n->data = data;
    n->color = Red;

    return n;
}

rb_tree *new_rb_tree() 
{
    rb_tree *t = malloc (sizeof (rb_tree));
    rb_node *nil_node = malloc (sizeof (rb_node));
    nil_node->left = NULL;
    nil_node->right = NULL;
    nil_node->parent = NULL;
    nil_node->color = Black;
    nil_node->data = 0;
    t->NIL = nil_node;
    t->root = t->NIL;

    return t;
}

void left_rotate (rb_tree *t, rb_node *x) 
{
    rb_node *y = x->right;
    x->right = y->left;
    
    if (y->left != t->NIL)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == t->NIL)  //x is root
        t->root = y;
    else if (x == x->parent->left)  //x is left child
        x->parent->left = y;
    else  //x is right child
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

void right_rotate (rb_tree *t, rb_node *x) 
{
    rb_node *y = x->left;
    x->left = y->right;
    
    if (y->right != t->NIL)
        y->right->parent = x;
    
    y->parent = x->parent;
    
    if (x->parent == t->NIL)  //x is root
        t->root = y;
    else if (x == x->parent->right)  //x is left child
        x->parent->right = y;
    else  //x is right child
        x->parent->left = y;
    
    y->right = x;
    x->parent = y;
}

void insertion_fixup (rb_tree *t, rb_node *z) 
{
    while (z->parent->color == Red) 
    {
        if (z->parent == z->parent->parent->left) //z.parent is the left child
        {
            rb_node *y = z->parent->parent->right; //uncle of z

            if (y->color == Red) { //case 1
                z->parent->color = Black;
                y->color = Black;
                z->parent->parent->color = Red;
                z = z->parent->parent;
            }
            else { //case2 or case3
                if (z == z->parent->right) { //case2
                    z = z->parent; //marked z.parent as new z
                    left_rotate (t, z);
                }
                //case3
                z->parent->color = Black; //made parent black
                z->parent->parent->color = Red; //made parent red
                right_rotate (t, z->parent->parent);
            }
        }
        else { //z.parent is the right child
            rb_node *y = z->parent->parent->left; //uncle of z

            if (y->color == Red) {
                z->parent->color = Black;
                y->color = Black;
                z->parent->parent->color = Red;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->left) {
                    z = z->parent; //marked z.parent as new z
                    right_rotate (t, z);
                }
                z->parent->color = Black; //made parent black
                z->parent->parent->color = Red; //made parent red
                left_rotate (t, z->parent->parent);
            }
        }
    }
    t->root->color = Black;
}

void insert (rb_tree *t, rb_node *z) 
{
    rb_node *y = t->NIL; //variable for the parent of the added node
    rb_node *temp = t->root;

    while (temp != t->NIL) {
        y = temp;
        if (z->data < temp->data)
            temp = temp->left;
        else
            temp = temp->right;
    }
    z->parent = y;

    if (y == t->NIL) //newly added node is root
        t->root = z;
    else if (z->data < y->data) //data of child is less than its parent, left child
        y->left = z;
    else
        y->right = z;

    z->right = t->NIL;
    z->left = t->NIL;
    z->color = Red;

    insertion_fixup (t, z);
}

void transplant (rb_tree *t, rb_node *u, rb_node *v) 
{
    if (u->parent == t->NIL)
        t->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

rb_node *minimum (rb_tree *t, rb_node *x) // inorder successor
{
    while (x->left != t->NIL)
        x = x->left;
    return x;
}

void delete_fixup (rb_tree *t, rb_node *x) 
{
    while (x != t->root && x->color == Black) 
    {
        if (x == x->parent->left) 
        {
            rb_node *w = x->parent->right;
            if (w->color == Red) {
                w->color = Black;
                x->parent->color = Red;
                left_rotate (t, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == Black && w->right->color == Black) {
                w->color = Red;
                x = x->parent;
            }
            else {
                if (w->right->color == Black) {
                    w->left->color = Black;
                    w->color = Red;
                    right_rotate (t, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = Black;
                w->right->color = Black;
                left_rotate (t, x->parent);
                x = t->root;
            }
        }
        else {
            rb_node *w = x->parent->left;
            if (w->color == Red) {
                w->color = Black;
                x->parent->color = Red;
                right_rotate (t, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == Black && w->left->color == Black) {
                w->color = Red;
                x = x->parent;
            }
            else {
                if (w->left->color == Black) {
                    w->right->color = Black;
                    w->color = Red;
                    left_rotate (t, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = Black;
                w->left->color = Black;
                right_rotate (t, x->parent);
                x = t->root;
            }
        }
    }
    x->color = Black;
}

void delete (rb_tree *t, rb_node *z) 
{
    rb_node *y = z;
    rb_node *x;
    enum COLOR y_orignal_color = y->color;
    if (z->left == t->NIL) {
        x = z->right;
        transplant (t, z, z->right);
        free(z);
    }
    else if (z->right == t->NIL) {
        x = z->left;
        transplant (t, z, z->left);
        free(z);
    }
    else {
        y = minimum (t, z->right);
        y_orignal_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        }
        else {
            transplant (t, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant (t, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
        free(z);
    }
    if (y_orignal_color == Black)
        delete_fixup (t, x);
}




/////////////////////////////   tree 관련 함수들  ////////////////////////////

void pre_order (rb_tree *t, rb_node *n) 
{
    if (n != t->NIL) {
        printf ("%d ", n->data);
        pre_order (t, n->left);
        pre_order (t, n->right);
    }
}

void in_order (rb_tree *t, rb_node *n) 
{
    if (n != t->NIL) {
        in_order (t, n->left);
        printf ("%d ", n->data);
        in_order (t, n->right);
    }
}

rb_node *search (rb_tree *t, int data) 
{
    rb_node *node = t->root;
    while (node != t->NIL) {
        if (data < node->data)
            node = node->left;
        else if (data > node->data)
            node = node->right;
        else
            return node;
    }
    return NULL;
}

rb_node *first_node (rb_tree *t)
{
    rb_node *node = t->root;
    if (node == t->NIL) 
        return NULL;
    while (node->left != t->NIL)
        node = node->left;
    return node;
}

rb_node *last_node (rb_tree *t)
{
    rb_node *node = t->root;
    if (node == t->NIL) 
        return NULL;
    while (node->right != t->NIL)
        node = node->right;
    return node;
}

rb_node *next_node (rb_tree *t, rb_node *node)
{
    if (node == NULL || node == t->NIL) 
        return NULL;
    if (node->right == t->NIL) {
        if (node->parent != t->NIL && node == node->parent->left)
            return node->parent;
        while (node->parent != t->NIL && node == node->parent->right)
            node = node->parent;
        return node->parent;
    }
    node = node->right;
    while (node->left != t->NIL)
        node = node->left;
    return node;
}

rb_node *prev_node (rb_tree *t, rb_node *node)
{
    if (node == NULL || node == t->NIL) 
        return NULL;
    if (node->left == t->NIL) {
        if (node->parent != t->NIL && node == node->parent->right)
            return node->parent;
        while (node->parent != t->NIL && node == node->parent->left)
            node = node->parent;
        return node->parent;
    }
    node = node->left;
    while (node->right != t->NIL)
        node = node->right;
    return node;
}

int tree_height (rb_tree *t, rb_node *n)
{
    if (n == t->NIL)
        return 0;
    int x = tree_height (t, n->left);
    int y = tree_height (t, n->right);
    if (x > y)
        return x + 1;
    else
        return y + 1;
}

int count_all_node (rb_tree *t, rb_node *n)
{
    if (n == t->NIL)
        return 0;
    return count_all_node (t, n->left) + count_all_node (t, n->right) + 1;
}

int count_2degree (rb_tree *t, rb_node *n)
{
    if (n == t->NIL)
        return 0;
    if (n->left != t->NIL && n->right != t->NIL)
        return count_2degree (t, n->left) + count_2degree (t, n->right) + 1;
    else
        return count_2degree (t, n->left) + count_2degree (t, n->right);
}

int count_1degree (rb_tree *t, rb_node *n)
{
    if (n == t->NIL)
        return 0;
    if (n->left == t->NIL ^ n->right == t->NIL)
        return count_1degree (t, n->left) + count_1degree (t, n->right) + 1;
    else
        return count_1degree (t, n->left) + count_1degree (t, n->right);
}

int count_leaf (rb_tree *t, rb_node *n)
{
    if (n == t->NIL)
        return 0;
    if (n->left == t->NIL && n->right == t->NIL)
        return count_leaf (t, n->left) + count_leaf (t, n->right) + 1;
    else
        return count_leaf (t, n->left) + count_leaf (t, n->right);
}

int count_internal (rb_tree *t, rb_node *n)
{
    if (n == t->NIL)
        return 0;
    if (n->left != t->NIL || n->right != t->NIL)
        return count_internal (t, n->left) + count_internal (t, n->right) + 1;
    else
        return count_internal (t, n->left) + count_internal (t, n->right);
}

int sum_all_data (rb_tree *t, rb_node *n)
{
    if (n == t->NIL)
        return 0;
    return sum_all_data (t, n->left) + sum_all_data (t, n->right) + n->data;
}


bool is_binary_search_tree (rb_tree *t, rb_node *n, int min, int max)  
{  
    if (n == t->NIL)
        return true;
    
    if (n->data < min || n->data > max)  
        return false;
  
    return 
        is_binary_search_tree (t, n->left, min, n->data - 1)
        && is_binary_search_tree (t, n->right, n->data + 1, max);
}  

bool is_full_binary_tree (rb_tree *t, rb_node *n) 
{
    if (n == t->NIL)
        return true;

    if (n->left == t->NIL && n->right == t->NIL)
        return true;

    if (n->left != t->NIL && n->right != t->NIL)
        return is_full_binary_tree (t, n->left) 
            && is_full_binary_tree (t, n->right);

    return false;
}

int depth (rb_tree *t, rb_node *n) {
    int d = 0;
    while (n != t->NIL) {
        d++;
        n = n->left;
    }
    return d;
}

bool is_perfect_binary_tree (rb_tree *t, rb_node *n, int d, int level) 
{
    if (n == t->NIL)
        return true;

    if (n->left == t->NIL && n->right == t->NIL)
        return (d == level + 1);

    if (n->left == t->NIL || n->right == t->NIL)
        return false;

    return is_perfect_binary_tree (t, n->left, d, level + 1) 
        && is_perfect_binary_tree (t, n->right, d, level + 1);
}

bool is_complete_binary_tree (rb_tree *t, rb_node *n, int index, int nr_nodes) 
{
    if (n == t->NIL)
        return true;

    if (index >= nr_nodes)
        return false;

    return (is_complete_binary_tree (t, n->left, 2 * index + 1, nr_nodes) 
         && is_complete_binary_tree (t, n->right, 2 * index + 2, nr_nodes));
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
rb_node *arr [1000000];

int main() 
{
    int total = sizeof (data) / sizeof (data[0]);
    int i = 0;

    rb_tree *t = new_rb_tree();

    TIME_START();
    for (i = 0; i < total; i++) {
        arr[i] = new_rb_node (data[i]);
        insert (t, arr[i]);
    }
    TIME_END("Insert DONE");

    TIME_START();
    for (i = 0; i < total; i++) {
        if (search (t, arr[i]->data) == NULL)
            printf ("node NOT found : %d\n", arr[i]->data);
    }
    TIME_END("Search DONE");

    rb_node *tmp = first_node (t);
    printf ("first node : %d\n", tmp ? tmp->data : 0);
    tmp = last_node (t);
    printf ("last node : %d\n", tmp ? tmp->data : 0);
    tmp = prev_node (t, arr[3]);
    printf ("prev node : %d\n", tmp ? tmp->data : 0);
    tmp = next_node (t, arr[3]);
    printf ("next node : %d\n", tmp ? tmp->data : 0);


    printf ("tree height : %d\n", tree_height (t, t->root));
    printf ("node count all : %d\n", count_all_node (t, t->root));
    printf ("node count 2degree : %d\n", count_2degree (t, t->root));
    printf ("node count 1degree : %d\n", count_1degree (t, t->root));
    printf ("node count leaf : %d\n", count_leaf (t, t->root));
    printf ("node count internal : %d\n", count_internal (t, t->root));
    printf ("sum all node value : %d\n", sum_all_data (t, t->root));


    printf ("The tree is %s tree\n",
            is_binary_search_tree (t, t->root, INT_MIN, INT_MAX) ?
            "BINARY SEARCH" : "NOT BINARY SEARCH");

    printf ("The tree is %s binary tree\n",
            is_full_binary_tree (t, t->root) ? "FULL" : "NOT FULL");
    
    printf ("The tree is %s binary tree\n",
            is_perfect_binary_tree (t, t->root, depth (t, t->root), 0) ?
            "PERFECT" : "NOT PERFECT");

    printf ("The tree is %s binary tree\n",
            is_complete_binary_tree (t, t->root, 0, count_all_node (t, t->root)) ?
            "COMPLETE" : "NOT COMPLETE");

//     pre_order (t, t->root); puts("");
//     in_order (t, t->root); puts("");

    TIME_START();
    for (i = 0; i < total; i++) {
        delete (t, arr[i]);
    }
    TIME_END("Delete DONE");

    TIME_RESULT();

    free (t->NIL);
    free (t);

    return 0;
}

