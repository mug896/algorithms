#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>

typedef struct node {
    int key;
    int height;
    struct node *left;
    struct node *right;
} node_t;

int max (int a, int b) {
    return (a > b) ? a : b;
}

int height (node_t *n) {
    return n ? n->height : 0;
}

node_t *create_node (int key) {
    node_t *n = malloc (sizeof (struct node));
    n->height = 1;
    n->key = key;
    n->left = n->right = NULL;
    return n;
}

node_t *right_rotate (node_t *y) 
{
    node_t *x = y->left;
    node_t *bak = x->right;
    
    x->right = y;
    y->left = bak;
    
    y->height = max (height (y->left), height (y->right)) + 1;
    x->height = max (height (x->left), height (x->right)) + 1;
    
    return x;
}

node_t *left_rotate (node_t *x) 
{
    node_t *y = x->right;
    node_t *bak = y->left;
    
    y->left = x;
    x->right = bak;
    
    x->height = max (height (x->left), height (x->right)) + 1;
    y->height = max (height (y->left), height (y->right)) + 1;
    
    return y;
}

int get_balance (node_t *n) {
    if (n == NULL)
        return 0;
    return height (n->left) - height (n->right);
}

node_t *insert (node_t *node, int key) 
{
    if (node == NULL)
        return create_node (key);
    
    if (key < node->key)
        node->left = insert (node->left, key);
    else if (key > node->key)
        node->right = insert (node->right, key);
    else
        return node;
    
    // Update the balance factor of each node and balance the tree
    node->height = 1 + max (height (node->left), height (node->right));
    
    int balance = get_balance (node);
    if (balance > 1 && key < node->left->key)
        return right_rotate (node);
    
    if (balance < -1 && key > node->right->key)
        return left_rotate (node);
    
    if (balance > 1 && key > node->left->key) {
        node->left = left_rotate (node->left);
        return right_rotate (node);
    }

    if (balance < -1 && key < node->right->key) {
        node->right = right_rotate (node->right);
        return left_rotate (node);
    }

    return node;
}

node_t *successor (node_t *node) 
{
    if (node == NULL) 
        return node;
    while (node->left != NULL)
        node = node->left;
    return node;
}

node_t *delete (node_t *root, int key) 
{
    if (root == NULL)
        return root;

    if (key < root->key)
        root->left = delete (root->left, key);

    else if (key > root->key)
        root->right = delete (root->right, key);

    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            node_t *temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;
            free (temp);
        } 
        else {
            node_t *temp = successor (root->right);
            root->key = temp->key;
            root->right = delete (root->right, temp->key);
        }
    }

    if (root == NULL)
        return root;

    // Update the balance factor of each node and balance the tree
    root->height = 1 + max (height (root->left), height (root->right));

    int balance = get_balance (root);
    if (balance > 1 && get_balance (root->left) >= 0)
        return right_rotate (root);

    if (balance > 1 && get_balance (root->left) < 0) {
        root->left = left_rotate (root->left);
        return right_rotate (root);
    }

    if (balance < -1 && get_balance (root->right) <= 0)
        return left_rotate (root);

    if (balance < -1 && get_balance (root->right) > 0) {
        root->right = right_rotate (root->right);
        return left_rotate (root);
    }

    return root;
}




/////////////////////////////   tree 관련 함수들  ////////////////////////////

void pre_order (node_t *n) {
    if (n != NULL) {
        printf ("%d ", n->key);
        pre_order (n->left);
        pre_order (n->right);
    }
}

void in_order (node_t *n) {
    if (n != NULL) {
        in_order (n->left);
        printf ("%d ", n->key);
        in_order (n->right);
    }
}

node_t *search (node_t *root, int key) 
{
    while (root != NULL) {
        if (key < root->key)
            root = root->left;
        else if (key > root->key)
            root = root->right;
        else
            return root;
    }
    return NULL;
}

node_t *first_node (node_t *node)
{
    if (node == NULL)
        return NULL;
    while (node->left)
        node = node->left;
    return node;
}

node_t *last_node (node_t *node)
{
    if (node == NULL)
        return NULL;
    while (node->right)
        node = node->right;
    return node;
}

int tree_height (node_t *n)
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

int count_all_node (node_t *n)
{
    if (n == NULL)
        return 0;
    return count_all_node (n->left) + count_all_node (n->right) + 1;
}

int count_2degree (node_t *n)
{
    if (n == NULL)
        return 0;
    if (n->left && n->right)
        return count_2degree (n->left) + count_2degree (n->right) + 1;
    else
        return count_2degree (n->left) + count_2degree (n->right);
}

int count_1degree (node_t *n)
{
    if (n == NULL)
        return 0;
    if (n->left == NULL ^ n->right == NULL)
        return count_1degree (n->left) + count_1degree (n->right) + 1;
    else
        return count_1degree (n->left) + count_1degree (n->right);
}

int count_leaf (node_t *n)
{
    if (n == NULL)
        return 0;
    if (n->left == NULL && n->right == NULL)
        return count_leaf (n->left) + count_leaf (n->right) + 1;
    else
        return count_leaf (n->left) + count_leaf (n->right);
}

int count_internal (node_t *n)
{
    if (n == NULL)
        return 0;
    if (n->left || n->right)
        return count_internal (n->left) + count_internal (n->right) + 1;
    else
        return count_internal (n->left) + count_internal (n->right);
}

int sum_all_key (node_t *n)
{
    if (n == NULL)
        return 0;
    return sum_all_key (n->left) + sum_all_key (n->right) + n->key;
}


bool is_binary_search_tree (node_t *n, int min, int max)  
{  
    if (n == NULL)
        return true;
    
    if (n->key < min || n->key > max)  
        return false;
  
    return 
        is_binary_search_tree (n->left, min, n->key - 1)
        && is_binary_search_tree (n->right, n->key + 1, max);
}  

bool is_full_binary_tree (node_t *n) 
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

int depth (node_t *n) {
    int d = 0;
    while (n != NULL) {
        d++;
        n = n->left;
    }
    return d;
}

bool is_perfect_binary_tree (node_t *n, int d, int level) 
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

bool is_complete_binary_tree (node_t *n, int index, int nr_nodes) 
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

int main() 
{
    int total = sizeof (data) / sizeof (data[0]);
    int i = 0;

    node_t *root = NULL;

    TIME_START();
    for (i = 0; i < total; i++)
        root = insert (root, data[i]);
    TIME_END("Insert DONE");

    TIME_START();
    for (i = 0; i < total; i++) {
        if (search (root, data[i]) == NULL)
            printf ("node NOT found : %d\n", data[i]);
    }
    TIME_END("Search DONE");


    printf ("first node : %d\n", first_node (root)->key);
    printf ("last node : %d\n", last_node (root)->key);

    printf ("tree height : %d\n", tree_height (root));
    printf ("node count all : %d\n", count_all_node (root));
    printf ("node count 2degree : %d\n", count_2degree (root));
    printf ("node count 1degree : %d\n", count_1degree (root));
    printf ("node count leaf : %d\n", count_leaf (root));
    printf ("node count internal : %d\n", count_internal (root));
    printf ("sum all node value : %d\n", sum_all_key (root));


    printf ("The tree is %s tree\n",
            is_binary_search_tree (root, INT_MIN, INT_MAX) ?
            "BINARY SEARCH" : "NOT BINARY SEARCH");

    printf ("The tree is %s binary tree\n",
            is_full_binary_tree (root) ? "FULL" : "NOT FULL");
    
    printf ("The tree is %s binary tree\n",
            is_perfect_binary_tree (root, depth (root), 0) ?
            "PERFECT" : "NOT PERFECT");

    printf ("The tree is %s binary tree\n",
            is_complete_binary_tree (root, 0, count_all_node (root)) ?
            "COMPLETE" : "NOT COMPLETE");

    //pre_order (root); puts ("");
    //in_order (root); puts ("");

    TIME_START();
    for (i = 0; i < total; i++)
        root = delete (root, data[i]);
    TIME_END("Delete DONE");
    
    TIME_RESULT();

    return 0;
}
