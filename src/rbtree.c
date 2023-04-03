#include "rbtree.h"
#include <stdlib.h>



rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
    node_t *NIL = (node_t*)calloc(1, sizeof(node_t));
    NIL -> color = RBTREE_BLACK;
    p->nil = NIL;
    p->root = NIL;
  return p;
}

void traversal_node(rbtree *t, node_t *cur_node){
    if(cur_node->left != t->nil){
        traversal_node(t, cur_node->left);
        cur_node->left = t->nil;
    }
    if(cur_node->right != t->nil){
        traversal_node(t, cur_node->right);
        cur_node->right = t->nil;
    }
    if (cur_node ->left == t->nil && cur_node -> right == t->nil){
        free(cur_node);
        return;
    }
    return;
}
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  node_t *cur_node = t->root;
  if (t->root == t->nil){
      free(t);
      return;
  }
  traversal_node(t, cur_node);
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
    // TODO: implement insert
    node_t *node = (node_t *)calloc(1,sizeof(node_t));
    node->key = key;
    node->right = t->nil;
    node->left = t->nil;
    if(t->root == t->nil){
        t->root = node;
        node->color = RBTREE_BLACK;
        node->parent = t->nil;
        return t->root;
    }
    node_t *compare_node = t->root;
    node->color = RBTREE_RED;
    //insert node
    while(1){
        key_t compare_key = compare_node->key;
        if (compare_key < key){
            if (compare_node->right == t->nil) {
                compare_node->right = node;
                node->parent = compare_node;
                break;
            }
            compare_node = compare_node->right;
        }
        if (compare_key >= key){
            if (compare_node->left == t->nil) {
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
    if (grand_parent == t->nil){
        return t->root;
    }
    if (grand_parent->left == parent){
        uncle = grand_parent->right;
    }else{
        uncle = grand_parent->left;
    }
    //satisfy property
    if (node->parent->color == RBTREE_BLACK) {
        return t->root;
    }
    while(1) {
        //Restructuring
        if (uncle->color == RBTREE_BLACK) {
            //linear
            if ((node->key > parent->key && key > grand_parent->key) || (node->key <= parent->key && key <= grand_parent->key)) {
                if (grand_parent->parent->right == grand_parent) {
                    grand_parent->parent->right = parent;
                } else {
                    grand_parent->parent->left = parent;
                }
                parent->parent = grand_parent->parent;
                grand_parent->parent = parent;
                //left-linear
                if (grand_parent->left == parent) {
                    grand_parent -> left = parent->right;
                    parent->right->parent = grand_parent;
                    parent->right = grand_parent;
                    parent->left = node;
                //right-linear
                } else {
                    grand_parent -> right = parent->left;
                    parent->left->parent = grand_parent;
                    parent->left = grand_parent;
                    parent->right = node;
                }

                parent->color = RBTREE_BLACK;
                grand_parent->color = RBTREE_RED;
                if (parent -> parent == t->nil){
                    t->root = parent;
                }
                return t->root;
            } else {
                //triangle
                node->parent = grand_parent;
                parent->parent = node;
                if (grand_parent->left == parent) {
                    grand_parent->left = node;
                    node -> left->parent = parent;
                    parent->right = node->left;
                    node->left = parent;
                } else {
                    grand_parent->right = node;
                    node->right->parent = parent;
                    parent->left = node->right;
                    node->right = parent;
                }
                node = parent;
                parent = node->parent;
                grand_parent = parent->parent;
                if (grand_parent->left == parent){
                    uncle = grand_parent->right;
                }else{
                    uncle = grand_parent->left;
                }
                continue;
            }
        }
        //Recoloring
        if (uncle->color == RBTREE_RED){
           uncle->color = RBTREE_BLACK;
           parent->color = RBTREE_BLACK;
           grand_parent->color = RBTREE_RED;
            if (grand_parent == t->root){
                grand_parent->color = RBTREE_BLACK;
            }
            if (grand_parent->parent->color == RBTREE_RED) {
                node = grand_parent;
                parent = node->parent;
                grand_parent = parent->parent;
                if (grand_parent->left == parent){
                    uncle = grand_parent->right;
                }else{
                    uncle = grand_parent->left;
                }
                continue;
            }
           return t->root;
        }
    }
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
    while(1){
        node_t *compare_node = t->root;
        key_t compare_key = compare_node->key;
        if (compare_key == key){
            return compare_node;
        }
        if (compare_key < key){
            if (compare_node->right->key == key){
                return compare_node->right;
            }
            compare_node = compare_node->right;
        }
        if (compare_key > key){
            if (compare_node->left->key == key){
                return compare_node->left;
            }
            compare_node = compare_node->left;
        }
        if (compare_node == t->nil){
            return NULL;
        }
    }
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

void inorder(const rbtree *t, node_t *cur_node, key_t *arr, int *index){
    if (cur_node == t->nil) {
        return;
    }
    inorder(t,cur_node->left,arr,index);
    arr[(*index)++] = cur_node->key;
    inorder(t,cur_node->right,arr,index);
}
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  node_t *cur_node = t->root;
  int index = 0;
  inorder(t, cur_node, arr, &index);
  return 0;
}
