#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {

    TreeMap *tree = (TreeMap *) malloc(sizeof(TreeMap));
    tree->current = NULL;
    tree->root = NULL;
    tree->lower_than = lower_than;
    return tree;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if(tree == NULL) return;

    TreeNode *aux = tree->root;
    while(aux)
    {
        if(is_equal(tree, key, aux->pair->key))
            return;
        if(tree->lower_than(key, aux->pair->key))
        {
            if(aux->left) aux = aux->left;
            else
            {
                aux->left = createTreeNode(key, value);
                aux->left->parent = aux;
                tree->current = aux->left;
            }
        }
        else
        {
            if(aux->right) aux = aux->right;
            else
            {
                aux->right = createTreeNode(key, value);
                aux->right->parent = aux;
                tree->current = aux->right;
            }
        }
    }

}

TreeNode * minimum(TreeNode * x){
    if(x == NULL) return NULL;

    while(x->left)
    {
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if(tree == NULL || node == NULL) return;

    if(node->left == NULL && node->right == NULL) // Caso sin hijos
    {
        if(tree->lower_than(node, node->parent))
            node->parent->left = NULL;
        else
            node->parent->right = NULL;
        free(node);
    }
    else if(node->left && node->right) // Caso 2 hijos
    {
        TreeNode *aux = minimum(node->right);
        node->pair->key = aux->pair->key;
        node->pair->value = aux->pair->value;
        removeNode(tree, aux);
    }
    else // Caso 1 hijo
    {
        TreeNode *child;
        if(node->left) child = node->left;
        else child = node->right;

        if(tree->lower_than(node, node->parent))
            node->parent->left = child;
        else
            node->parent->right = child;
            
        child->parent = node->parent;
        free(node);
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);
}

Pair * searchTreeMap(TreeMap * tree, void* key) {
    if(tree == NULL) return NULL;

    TreeNode *aux = tree->root;
    while(aux)
    {
        if(is_equal(tree, key, aux->pair->key)) break;
        if(tree->lower_than(key, aux->pair->key))
            aux = aux->left;
        else
            aux = aux->right;
    }

    tree->current = aux;
    if(aux) return aux->pair;
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    if(tree == NULL) return NULL;

    TreeNode *node = tree->root;
    TreeNode *ub_node;
    while(node)
    {
        if(is_equal(tree, node->pair->key, key))
            return node->pair;
        if(tree->lower_than(key, node->pair->key))
            node = node->left;
        else {
            node = node->right;
            ub_node = node;
        }
    }

    if(ub_node) return ub_node->pair;
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if(tree == NULL || tree->root == NULL)
        return NULL;

    TreeNode *aux = minimum(tree->root);
    return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if(tree == NULL || tree->current == NULL)
        return NULL;

    TreeNode *aux;
    if(tree->current->right)
        aux = minimum(tree->current->right);
    else
    {
        aux = tree->current->parent;
        while(aux && tree->lower_than(tree->current->pair->key, aux->pair->key))
            aux = aux->parent;
    }

    tree->current = aux;
    if(aux == NULL) return NULL;
    return aux->pair;
}
