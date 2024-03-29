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
    TreeNode *node = tree->root;
    TreeNode *newNode = createTreeNode(key, value);

    while(1) {
        if (is_equal(tree, key, node->pair->key)) {
            return;
        }

        if (tree->lower_than(key, node->pair->key)) {
            if (node->left == NULL) {
                node->left = newNode;
                newNode->parent = node;
                tree->current = newNode;
                return;
            }
            node = node->left;
        }
        else {
            if (node->right == NULL) {
                node->right = newNode;
                newNode->parent = node;
                tree->current = newNode;
                return;
            }
            node = node->right;
        }
    }

}

TreeNode * minimum(TreeNode * x){
    TreeNode *node = x;

    while(1) {
        if (node->left == NULL) {
            return node;
        }

        node = node->left;
    }
}


void removeNode(TreeMap * tree, TreeNode* node) {
    TreeNode *parent = node->parent;

    if (node->left != NULL && node->right != NULL) {
        TreeNode *min = minimum(node->right);
        node->pair = min->pair;
        removeNode(tree, min);
        return;
    }

    if (node->left != NULL) {
        if (parent != NULL) {
            if (tree->lower_than(node->pair->key, parent->pair->key)) {
                parent->left = node->left;
            }
            else {
                parent->right = node->left;
            }
            node->left->parent = parent;
        } else {
            tree->root = node->left;
        }
        return;
    }

    if (node->right != NULL) {
        if (parent != NULL) {
            if (tree->lower_than(node->pair->key, parent->pair->key)) {
                parent->left = node->right;
            }
            else {
                parent->right = node->right;
            }
            node->right->parent = parent;
        } else {
            tree->root = node->right;
        }
        return;
    }

    if (tree->lower_than(node->pair->key, parent->pair->key)) {
        parent->left = NULL;
    }
    else {
        parent->right = NULL;
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode *node = tree->root;

    while(node != NULL) {
        if (is_equal(tree, key, node->pair->key)) {
            tree->current = node;
            return node->pair;
        }

        if (tree->lower_than(key, node->pair->key)) {
            node = node->left;
        }
        else {
            node = node->right;
        }
    }

    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode *node = tree->root;
    if (node == NULL) return NULL;

    Pair *pair = NULL;

    while(node != NULL) {
        if (is_equal(tree, node->pair->key, key)) {
            return node->pair;
        }

        if (tree->lower_than(key, node->pair->key)) {
            pair = node->pair;
            node = node->left;
        } else {
            node = node->right;
        }
    }

    return pair;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree->root == NULL) return NULL;
    TreeNode *node = minimum(tree->root);
    tree->current = node;

    return node->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode *node = tree->current;

    if (node->right != NULL) {
        tree->current = minimum(node->right);
    } else {
        while(1) {
            if(node->parent == NULL) return NULL;

            if(tree->lower_than(node->pair->key, node->parent->pair->key)) {
                tree->current = node->parent;
                break;
            }

            node = node->parent;
        }        
    }

    return tree->current->pair;
}
