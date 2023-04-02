#include "rbtree.h"
#include <stdlib.h>



rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
    node_t *NIL = (node_t*)calloc(1, sizeof(node_t));
    NIL -> color = RBTREE_BLACK;
    p->nil = NIL;
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
    // TODO: implement insert
    node_t *node = (node_t *)calloc(1,sizeof(node_t));
    node->key = key;
    node->right = t->nil;
    node->left = t->nil;
    if(t->root == NULL){
        t->root = node;
        node->color = RBTREE_BLACK;
        return t->root;
    }
    node_t *compare_node = t->root;
    node->color = RBTREE_RED;
    //insert node
    while(1){
        key_t compare_key = compare_node->key;
        if (compare_key < key){
            if (compare_node->right->key == NULL) {
                compare_node->right = node;
                node->parent = compare_node;
                break;
            }
            compare_node = compare_node->right;
        }
        if (compare_key >= key){
            compare_node = compare_node->left;
            if (compare_node->left->key == NULL) {
                node_t *NIL = compare_node->right;
                compare_node->left = node;
                node->parent = compare_node;
                break;
            }
            compare_node = compare_node->left;
        }
    }

    node_t *grand_parent = node->parent->parent;
    node_t *parent = node->parent;
    //find uncle
    node_t *uncle;
    if (grand_parent->left == parent){
        uncle = grand_parent->right;
    }else{
        uncle = grand_parent->left;
    }
    while(1) {
        //satisfy property
        if (node->parent->color == RBTREE_BLACK) {
            return t->root;
        }
        //Restructuring
        if (uncle->color == RBTREE_BLACK) {
            //linear
            if ((key > parent->key && key > grand_parent->key) || (key <= parent->key && key <= grand_parent->key)) {
                if (grand_parent->parent->right == grand_parent) {
                    grand_parent->parent->right = parent;
                } else {
                    grand_parent->parent->left = parent;
                }
                parent->parent = grand_parent->parent;
                grand_parent->parent = parent;
                if (grand_parent->left == parent) {
                    parent->right = grand_parent;
                    parent->left = node;
                    grand_parent->left = t->nil;
                } else {
                    parent->left = grand_parent;
                    parent->right = node;
                    grand_parent->right = t->nil;
                }
                parent->color = RBTREE_BLACK;
                grand_parent->color = RBTREE_RED;
            } else {
                //triangle
                if (grand_parent->parent->right == grand_parent) {
                    grand_parent->parent->right = node;
                } else {
                    grand_parent->parent->left = node;
                }
                node->parent = grand_parent->parent;
                grand_parent->parent = node;
                parent->parent = node;
                if (grand_parent->left == parent) {
                    node->left = parent;
                    node->right = grand_parent;
                    grand_parent->left = t->nil;
                } else {
                    node->right = parent;
                    node->left = grand_parent;
                    grand_parent->right = t->nil;
                }
                node->color = RBTREE_BLACK;
                parent->left = t->nil;
                parent->right = t->nil;
                grand_parent->color = RBTREE_RED;
            }
            continue;
        }
        //Recoloring
        if (uncle->color == RBTREE_RED){
           uncle->color = RBTREE_BLACK;
           parent->color = RBTREE_BLACK;
           grand_parent->color = RBTREE_RED;
           if (grand_parent == t->root){
               grand_parent->color = RBTREE_BLACK;
           }
            continue;
        }
    }
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}

int main(){
    return 0;
}
