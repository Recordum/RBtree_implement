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
    if (t->root == t->nil) {
        free(t->nil);
        free(t);
        return;
    }
    traversal_node(t, cur_node);
    free(t->nil);
    free(t);
}

void insert_node(rbtree *t, node_t *node) {
    key_t key = node->key;
    node_t *compare_node = t->root;

    while (1) {
        key_t compare_key = compare_node->key;
        if (compare_key < key) {
            if (compare_node->right == t->nil) {
                compare_node->right = node;
                node->parent = compare_node;
                return;
            }
            compare_node = compare_node->right;
        }
        if (compare_key >= key) {
            if (compare_node->left == t->nil) {
                compare_node->left = node;
                node->parent = compare_node;
                return;
            }
            compare_node = compare_node->left;
        }
    }
}
node_t *find_uncle(rbtree* t, node_t* grand_parent, node_t* parent){

    if (grand_parent == t->nil){
        return t->root;
    }
    if (grand_parent->left == parent){
        return grand_parent->right;
    }else{
        return grand_parent->left;
    }
}
void init_node(rbtree *t, node_t *node, const key_t key){
    node->key = key;
    node->right = t->nil;
    node->left = t->nil;
    node->color = RBTREE_RED;
}
int is_rbtree_empty(rbtree *t, node_t *node){
    if(t->root == t->nil){
        t->root = node;
        node->color = RBTREE_BLACK;
        node->parent = t->nil;
        return 1;
    }
    return 0;
}
int satisfy_property(node_t *node){
    if (node->parent->color == RBTREE_BLACK){
        return 1;
    }
    return 0;
}
node_t *rbtree_insert(rbtree *t, const key_t key) {
    // TODO: implement insert
    node_t *node = (node_t *)calloc(1,sizeof(node_t));
    init_node(t, node, key);

    if(is_rbtree_empty(t, node)){
        return t->root;
    }
    insert_node(t,node);

    //satisfy property
    if (satisfy_property(node)) {
        return t->root;
    }

    node_t *grand_parent = node->parent->parent;
    node_t *parent = node->parent;
    node_t *uncle= find_uncle(t, grand_parent, parent);
    while(1) {
        //Restructuring
        if (uncle->color == RBTREE_BLACK) {
            //linear
            if ((grand_parent->left == parent && parent->left == node) || (grand_parent->right == parent && parent->right == node)) {
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
    node_t *compare_node = t->root;
    while(1){
        if (compare_node == t->nil){
            return NULL;
        }
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
    }
}

node_t *rbtree_min(const rbtree *t) {
    // TODO: implement find
    node_t *cur_node = t->root;
    while(1){
        if (cur_node->left == t->nil){
            return cur_node;
        }
        cur_node = cur_node->left;
    }
}

node_t *rbtree_max(const rbtree *t) {
    // TODO: implement find
    node_t *cur_node = t->root;
    while(1){
        if (cur_node->right == t->nil){
            return cur_node;
        }
        cur_node = cur_node->right;
    }
}

int has_fewer_child(rbtree *t, node_t *cur_node) {
    if (cur_node->left == t->nil || cur_node->right == t->nil){
        return 1;
    }
    return 0;
}

void left_rotate(rbtree* t, node_t *cur_node, node_t *parent_node) {
    cur_node->parent = parent_node->parent;
    if (parent_node->parent->left == parent_node) {
        parent_node->parent->left = cur_node;
    } else {
        parent_node->parent->right = cur_node;
    }
    parent_node->parent = cur_node;
    parent_node->right = cur_node->left;
    cur_node->left->parent = parent_node;
    cur_node->left = parent_node;
    if (parent_node==t->root){
        t->root = cur_node;
        t->root->color = RBTREE_BLACK;
    }
    return;
}
void right_rotate(rbtree* t,node_t *cur_node, node_t *parent_node) {
    cur_node->parent = parent_node->parent;
    if (parent_node->parent->left == parent_node) {
        parent_node->parent->left = cur_node;
    } else {
        parent_node->parent->right = cur_node;
    }
    parent_node->parent = cur_node;
    parent_node->left = cur_node->right;
    cur_node->right->parent = parent_node;
    cur_node->right = parent_node;
    if (parent_node==t->root){
        t->root = cur_node;
        t->root->color = RBTREE_BLACK;
    }
    return;
}
int is_red_and_black(node_t *child) {
    if (child->color == RBTREE_BLACK){
        return 0;
    }
    return 1;
}
int nephew_is_RBTREE_RED(node_t *brother_node, node_t* nephew_node){
    if(brother_node->color == RBTREE_BLACK && nephew_node->color == RBTREE_RED){
        return 1;
    }
    return 0;
}

int nephew_is_RBTREE_BLACK(node_t *brother_node, node_t *nephew_node) {
    if (!(brother_node->color == RBTREE_BLACK && nephew_node->color == RBTREE_BLACK)){
        return 0;
    }
    if (brother_node->right == nephew_node && brother_node->left->color == RBTREE_RED){
        return 1;
    }
    if (brother_node->left == nephew_node && brother_node->right->color == RBTREE_RED){
        return 1;
    }
    return 0;
}

int all_nephew_are_RBTREE_BLACK(node_t *brother_node, node_t *nephew_node){
    if (nephew_node->color == RBTREE_RED || brother_node->color == RBTREE_RED){
        return 0;
    }
    if (brother_node->right == nephew_node && brother_node->left->color == RBTREE_BLACK){
        return 1;
    }
    if (brother_node->left == nephew_node && brother_node->right->color == RBTREE_BLACK){
        return 1;
    }
    return 0;
}

int brother_is_RBTREE_RED(node_t* brother_node){
    if (brother_node->color == RBTREE_RED){
        return 1;
    }
    return 0;
}
node_t* define_brother_node(node_t *parent_node, node_t* cur_node){
    if (parent_node->left == cur_node) {
        return parent_node->right;
    }
    return parent_node->left;
}
node_t* define_nephew_node(node_t *parent_node, node_t* cur_node){
    if (parent_node->left == cur_node) {
        return parent_node->right->right;
    }
    return parent_node->left->left;
}
void fix_nephew_is_RBTREE_RED(rbtree *t, node_t *brother_node, node_t* parent_node, node_t* nephew_node){
    /**
     * nephew_is_rbtree_RED case 를 처리 하는 함수
     */
    brother_node->color = parent_node->color;
    parent_node->color = RBTREE_BLACK;
    nephew_node->color = RBTREE_BLACK;
    if (brother_node->right == nephew_node) {
        left_rotate(t, brother_node, parent_node);
    }else{
        right_rotate(t, brother_node, parent_node);
    }
}
void fix_nephew_is_RBTREE_BLACK(rbtree *t, node_t *brother_node, node_t* parent_node, node_t* nephew_node){
    /**
     * nephew_is_rbtree_BLACK case 를 처리 하는 함수
     */
    brother_node->color = RBTREE_RED;
    if (brother_node->right == nephew_node) {
        brother_node->left->color = RBTREE_BLACK;
        right_rotate(t, brother_node->left, brother_node);
    }else{
        brother_node->right->color = RBTREE_BLACK;
        left_rotate(t, brother_node->right, brother_node);
    }
}

void fix_brother_is_RBTREE_RED(rbtree *t, node_t *brother_node, node_t *parent_node, node_t *cur_node) {
    /**
     * brother_is_rbtree_RED case 를 처리 하는 함수
     */
    color_t temp = brother_node->color;
    brother_node->color = parent_node->color;
    parent_node->color = temp;
    if (parent_node->left == cur_node) {
        left_rotate(t, brother_node, parent_node);
        return;
    }
    right_rotate(t, brother_node, parent_node);
}

void fix_doubly_black(rbtree *t, node_t *cur_node, node_t *parent_node) {
    /**
     * double_black 을 각 case 에 따라 처리 하는 함수
    */
    node_t *brother_node;
    node_t *nephew_node;
    while(1){
        brother_node = define_brother_node(parent_node, cur_node);
        nephew_node = define_nephew_node(parent_node, cur_node);
        if (nephew_is_RBTREE_RED(brother_node,nephew_node)){
            fix_nephew_is_RBTREE_RED(t, brother_node, parent_node, nephew_node);
            return;
        }
        if (nephew_is_RBTREE_BLACK(brother_node, nephew_node)){
            fix_nephew_is_RBTREE_BLACK(t, brother_node, parent_node, nephew_node);
            continue;
        }
        if (all_nephew_are_RBTREE_BLACK(brother_node, nephew_node)){
            brother_node->color = RBTREE_RED;
            if (parent_node->color == RBTREE_RED || t->root == parent_node){
                parent_node->color = RBTREE_BLACK;
                return;
            }
            fix_doubly_black(t,parent_node,parent_node->parent);
            return;
        }
        if (brother_is_RBTREE_RED(brother_node)){
            fix_brother_is_RBTREE_RED(t, brother_node, parent_node, cur_node);
            continue;
        }
    }
}

void deleted_black(rbtree *t, node_t *cur_node, node_t *child_node, node_t *parent_node){
    /**
     * color 가 BLACK인 node를 삭제 할때 rbtree property를 만족 하도록 처리 하는 함수
     */
    free(cur_node);
    if(child_node->parent == t->nil){
        t->root = child_node;
        t->root->color = RBTREE_BLACK;
        return;
    }
    if(is_red_and_black(child_node)){
        child_node->color = RBTREE_BLACK;
        return;
    }
    //doubly_black
    fix_doubly_black(t, child_node, parent_node);
}
void replacement_right_child(node_t *cur_node){
    /**
     * child node 가 한개 이하인 node를 삭제할때 연결된 node를 재배치 하는 함수
     */
    cur_node->right->parent = cur_node->parent;
    if(cur_node->parent->left == cur_node){
        cur_node->parent->left = cur_node->right;
    }else{
        cur_node->parent->right = cur_node->right;
    }

}
void replacement_left_child(node_t *cur_node){
    /**
     * child node 가 한개 이하인 node를 삭제할때 연결된 node를 재배치 하는 함수
     */
    cur_node->left->parent = cur_node->parent;
    if(cur_node->parent->left == cur_node){
        cur_node->parent->left = cur_node->left;
    }else{
        cur_node->parent->right = cur_node->left;
    }
}

void delete_node_with_fewer_child(rbtree *t, node_t *cur_node) {
    /**
     * child node 가 한개 이하인 node를 삭제하는 함수
     */
    if (cur_node->left == t->nil) {
        replacement_right_child(cur_node);
        if (cur_node->color == RBTREE_BLACK) {
            deleted_black(t, cur_node, cur_node->right, cur_node->parent);
            return;
        }
        free(cur_node);
        return;
    }
    if (cur_node->right == t->nil) {
        replacement_left_child(cur_node);
        if (cur_node->color == RBTREE_BLACK) {
            deleted_black(t, cur_node, cur_node->left, cur_node->parent);
            return;
        }
        free(cur_node);
        return;
    }
    return;
}

node_t *find_successor(rbtree *t, node_t *successor) {
    /**
     * successor 를 찾는 함수
     */
    while (1) {
        if (successor->left == t->nil) {
            return successor;
        }
        successor = successor->left;
    }
}

void change_key_value(node_t *cur_node, node_t *successor) {
    /**
     * successor 와 원래 node 키값을 교환하는 함수
     */
    cur_node->key = successor->key;
    successor->right->parent = successor->parent;
}
void replacement_successor_child(node_t *sub_tree_root, node_t *successor){
    /**
     * successor 를 삭제할 때 연결된 node를 재배치 하는 함수
     */
    if(successor == sub_tree_root){
        successor->parent->right = successor->right;
        return;
    }
    successor->parent->left = successor->right;
}

void delete_successor(rbtree *t, node_t *cur_node) {
    /**
     * successor 를 삭제 하는 함수
     */
    node_t *sub_tree_root = cur_node->right;
    node_t *successor = sub_tree_root;

    successor = find_successor(t, successor);
    change_key_value(cur_node, successor);
    replacement_successor_child(sub_tree_root, successor);

    if (successor->color == RBTREE_RED) {
        free(successor);
        return;
    }
    if (successor->color == RBTREE_BLACK) {
        deleted_black(t, successor, successor->right, successor->parent);
    }
}
node_t* find_erase_node(rbtree* t,node_t *cur_node, node_t *target){
    /**
     * 삭제할 node와 같은 주소를 갖는 node를 찾는 함수
     */
    while(1){
        if (cur_node == target){
            return cur_node;
        }
        if (cur_node == t->nil){
            return cur_node;
        }
        if (cur_node->key > target->key){
            cur_node = cur_node->left;
            continue;
        }
        if (cur_node->key <= target->key){
            cur_node = cur_node->right;
            continue;
        }
    }
}

int rbtree_erase(rbtree *t, node_t *p) {
    /**
     * node를 삭제하는 함수
     */
    // TODO: implement erase
    node_t *cur_node = t->root;
    cur_node = find_erase_node(t, cur_node, p);
    if (has_fewer_child(t, cur_node)) {
        delete_node_with_fewer_child(t, cur_node);
        return 0;
    }
    if (!has_fewer_child(t, cur_node)) {
        delete_successor(t, cur_node);
        return 0;
    }
    return 0;
}

void inorder(const rbtree *t, node_t *cur_node, key_t *arr, int *index){
    /**
     * rbtree를 순회하며 오름차순으로 key값을 배열에 저장하는 함수
     */
    if (cur_node == t->nil) {
        return;
    }
    inorder(t,cur_node->left,arr,index);
    arr[(*index)++] = cur_node->key;
    inorder(t,cur_node->right,arr,index);
}
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
    /**
     * inodrer()를 호출하여 rbtree 를 배열로 변환하는 함수
     */
    // TODO: implement to_array
    node_t *cur_node = t->root;
    int index = 0;
    inorder(t, cur_node, arr, &index);
    return 0;
}