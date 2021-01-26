/*
*    This file is part of SubFX,
*    Copyright(C) 2019-2020 fdar0536.
*
*    SubFX is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Lesser General Public License as
*    published by the Free Software Foundation, either version 2.1
*    of the License, or (at your option) any later version.
*
*    SubFX is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*    GNU Lesser General Public License for more details.
*
*    You should have received a copy of the GNU Lesser General
*    Public License along with SubFX. If not, see
*    <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>

#include "map.h"
#include "internal/types.h"

subfx_map *subfx_map_init()
{
    subfx_map *map = calloc(1, sizeof(subfx_map));
    if (!map)
    {
        return NULL;
    }

    map->create = subfx_map_create;
    map->at = subfx_map_at;
    map->insertNode = subfx_map_insertNode;
    map->deleteNode = subfx_map_deleteNode;
    return map;
}

subfx_handle subfx_map_create(subfx_cmpFunc keyCmpFunc,
                              subfx_freeFunc keyFreeFunc,
                              subfx_freeFunc valueFreeFunc)
{
    if (!keyCmpFunc)
    {
        return NULL;
    }

    RBTree *ret = calloc(1, sizeof(RBTree));
    if (!ret)
    {
        return NULL;
    }

    // create nil
    ret->nil = calloc(1, sizeof(Node));
    if (!ret->nil)
    {
        free(ret);
        return NULL;
    }

    ret->nil->key = NULL;
    ret->nil->value = NULL;
    ret->nil->color = black; // nil must be black
    ret->nil->parent = ret->nil;
    ret->nil->left = ret->nil;
    ret->nil->right = ret->nil;

    ret->id = subfx_types_map;
    ret->root = ret->nil;
    ret->keyCmpFunc = keyCmpFunc;
    ret->keyFreeFunc = keyFreeFunc;
    ret->valueFreeFunc = valueFreeFunc;

    return ret;
}

subfx_exitstate subfx_map_destroy(subfx_handle in)
{
    if (subfx_checkInput(in, subfx_types_map))
    {
        return subfx_failed;
    }

    RBTree *tree = (RBTree *)in;
    subfx_map_clear(tree, tree->root);

    // now root has been destroyed
    destroyNode(tree->nil, tree->keyFreeFunc, tree->valueFreeFunc);
    free(tree);
    return subfx_success;
}

void *subfx_map_at(subfx_handle in, void *key)
{
    if (subfx_checkInput(in, subfx_types_map))
    {
        return NULL;
    }

    if (!key)
    {
        return NULL;
    }

    RBTree *tree = (RBTree *)in;
    Node *res = subfx_map_searchNode(tree, key);
    if (res == tree->nil)
    {
        return NULL;
    }

    return res->value;
}

subfx_exitstate subfx_map_insertNode(subfx_handle in, void *key, void *value)
{
    if (subfx_checkInput(in, subfx_types_map))
    {
        return subfx_failed;
    }

    if (!key || !value)
    {
        return subfx_failed;
    }

    RBTree *tree = (RBTree *)in;
    Node *res = subfx_map_searchNode(tree, key);
    if (res != tree->nil)
    {
        // entry is already exist
        if (tree->valueFreeFunc)
        {
            tree->valueFreeFunc(res->value);
        }

        res->value = value;
        return subfx_success;
    }

    // here is the just simply BST insert
    Node *insert_node = createNode(tree->nil);
    if (!insert_node)
    {
        return subfx_failed;
    }

    insert_node->key = key;
    insert_node->value = value;

    Node *y = tree->nil;
    Node *x = tree->root;

    while (x != tree->nil)
    {
        y = x;
        if (tree->keyCmpFunc(insert_node->key, x->key) < 0)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }

    insert_node->parent = y;

    if (y == tree->nil)
    {
        tree->root = insert_node;
    }
    else if (tree->keyCmpFunc(insert_node->key, x->key) < 0)
    {
        y->left = insert_node;
    }
    else
    {
        y->right = insert_node;
    }

    subfx_map_insertFixedUp(tree, insert_node);
    return subfx_success;
} // end subfx_map_insertNode

subfx_exitstate subfx_map_deleteNode(subfx_handle in, void *key)
{
    if (subfx_checkInput(in, subfx_types_map))
    {
        return subfx_failed;
    }

    if (!key)
    {
        return subfx_failed;
    }

    RBTree *tree = (RBTree *)in;

    Node *delete_node = subfx_map_searchNode(tree, key);
    if (delete_node == tree->nil)
    {
        return subfx_failed;
    }

    Node *y = NULL; // to be deleted
    Node *x = NULL; // y's child

    if (delete_node->left == tree->nil || delete_node->right == tree->nil)
    {
        y = delete_node;
    }
    else
    {
        y = subfx_map_nodeSuccessor(tree, delete_node);
    }

    if (y->left != tree->nil)
    {
        x = y->left;
    }
    else
    {
        x = y->right;
    }

    if (x != tree->nil)
    {
        x->parent = y->parent;
    }

    if (y->parent == tree->nil)
    {
        tree->root = x;
    }
    else if (y == y->parent->left)
    {
        y->parent->left = x;
    }
    else
    {
        y->parent->right = x;
    }

    // for case3
    NodeColor color = y->color;
    if (y != delete_node)
    {
        // note that delete_node is not nil
        if (tree->keyFreeFunc) tree->keyFreeFunc(delete_node->key);
        delete_node->key = y->key;

        if (tree->valueFreeFunc) tree->valueFreeFunc(delete_node->value);
        delete_node->value = y->value;

        free(y);
    }
    else
    {
        // simply destroy y
        destroyNode(y, tree->keyFreeFunc, tree->valueFreeFunc);
    }

    if (color == black)
    {
        subfx_map_deleteFixedUp(tree, x);
    }

    return subfx_success;
} // end subfx_map_deleteNode

Node *createNode(Node *nil)
{
    if (!nil)
    {
        return NULL;
    }

    Node *ret = calloc(1, sizeof(Node));
    if (!ret)
    {
        return NULL;
    }

    ret->key = NULL;
    ret->value = NULL;
    ret->color = rad; // the default new node is rad
    ret->parent = nil;
    ret->left = nil;
    ret->right = nil;

    return ret;
}

void destroyNode(Node *node,
                 subfx_freeFunc keyFreeFunc,
                 subfx_freeFunc valueFreeFunc)
{
    if (!node)
    {
        return;
    }

    if (node->key)
    {
        if (keyFreeFunc) keyFreeFunc(node->key);
    }

    if (node->value)
    {
        if (valueFreeFunc) valueFreeFunc(node->value);
    }

    free(node);
}

void subfx_map_clear(RBTree *tree,
                     Node *in)
{
    // use LRV traversal to clean up
    if (in == tree->nil)
    {
        return;
    }

    subfx_map_clear(tree, in->left);
    subfx_map_clear(tree, in->right);
    destroyNode(in, tree->keyFreeFunc, tree->valueFreeFunc);
}

void subfx_map_leftRotation(RBTree *tree, Node *x)
{
    Node *y = x->right;

    x->right = y->left;

    if (y->left != tree->nil)
    {
        y->left->parent = x;
    }

    y->parent = x->parent;

    if (x->parent == tree->nil)
    {
        tree->root = y;
    }
    else if (x == x->parent->left)
    {
        x->parent->left = y;
    }
    else
    {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

void subfx_map_rightRotation(RBTree *tree, Node *y)
{
    Node *x = y->left;

    y->left = x->right;
    if (x->right != tree->nil)
    {
        x->right->parent = y;
    }

    x->parent = y->parent;

    if (y->parent == tree->nil)
    {
        tree->root = x;
    }
    else if (y == y->parent->left)
    {
        y->parent->left = x;
    }
    else
    {
        y->parent->right = x;
    }

    x->right = y;
    y->parent = x;
}

Node *subfx_map_searchNode(RBTree *tree, void *key)
{
    Node *current = tree->root;
    int res;
    while (current != tree->nil)
    {
        res = tree->keyCmpFunc(key, current->key);
        if (res == 0) // key == current->key
        {
            break;
        }
        else if (res < 0) // key < current->key
        {
            current = current->left;
        }
        else // key > current->key
        {
            current = current->right;
        }
    }

    return current;
} // end subfx_map_searchNode

void subfx_map_insertFixedUp(RBTree *tree, Node *current)
{
    // case0: the parent is black, so no need to enter the loop
    // in the other word
    // if the parent is rad, need to enter the loop
    while (current->parent->color == rad)
    {
        // the parent is the grandparent's left
        if (current->parent == current->parent->parent->left)
        {
            Node *uncle = current->parent->parent->right;
            // case1: uncle is rad
            if (uncle->color == rad)
            {
                current->parent->color = black;
                uncle->color = black;
                current->parent->parent->color = rad;
                current = current->parent->parent;
            }
            // case2 & 3: uncle is black
            else
            {
                if (current == current->parent->right) // case2
                {
                    current = current->parent;
                    subfx_map_leftRotation(tree, current);
                }

                // case3
                current->parent->color = black;
                current->parent->parent->color = rad;
                subfx_map_rightRotation(tree, current->parent->parent);
            }
        }
        // the parent is the grandparent's right
        else
        {
            Node *uncle = current->parent->parent->left;
            // case1: uncle is rad
            if (uncle->color == rad)
            {
                current->parent->color = black;
                uncle->color = black;
                current->parent->parent->color = rad;
                current = current->parent->parent;
            }
            // case2 & 3: uncle is black
            else
            {
                if (current == current->parent->left) // case2
                {
                    current = current->parent;
                    subfx_map_rightRotation(tree, current);
                }

                // case3
                current->parent->color = black;
                current->parent->parent->color = rad;
                subfx_map_leftRotation(tree, current->parent->parent);
            }
        }
    } // end while (current->parent->color == rad)

    tree->root->color = black; // insure root is black
} // end subfx_map_insertFixedUp

Node *subfx_map_nodeLeftmost(RBTree *tree, Node *current)
{
    while (current->left != tree->nil)
    {
        current = current->left;
    }

    return current;
}

Node *subfx_map_nodeSuccessor(RBTree *tree, Node *current)
{
    if (current->right != tree->nil)
    {
        return subfx_map_nodeLeftmost(tree, current->right);
    }

    Node *new_node = current->parent;

    while (new_node != tree->nil && current == new_node->right)
    {
        current = new_node;
        new_node = new_node->parent;
    }

    return new_node;
} // end subfx_map_nodeSuccessor

void subfx_map_deleteFixedUp(RBTree *tree, Node *current)
{
    // Case0:(i) if current is rad, let is black
    //       (ii) if current is root, let it black
    while (current != tree->root && current->color == black)
    {
        // current is left
        if (current == current->parent->left)
        {
            Node *sibling = current->parent->right;
            // Case1: sibling is rad
            if (sibling->color == rad)
            {
                sibling->color = black;
                current->parent->color = rad;
                subfx_map_leftRotation(tree, current->parent);
                sibling = current->parent->right;
            }

            // Case2、3、4: sibling is black
            // Case2: both sibling's children are black
            if (sibling->left->color == black &&
                sibling->right->color == black)
            {
                sibling->color = rad;

                // if current becomes the root after the update,
                // then exit the loop.
                current = current->parent;
            }
            // Case3 & 4: one of the sibling's child is black
            else
            {
                // case3: the sibling's right child is black,
                //        the other one is rad.
                if (sibling->right->color == black)
                {
                    sibling->left->color = black;
                    sibling->color = rad;
                    subfx_map_rightRotation(tree, sibling);
                    sibling = current->parent->right;
                }

                // After case3, it must enter case4.
                // case4: the sibling's right child is rad,
                //        the other one is black.
                sibling->color = current->parent->color;
                current->parent->color = black;
                sibling->right->color = black;
                subfx_map_leftRotation(tree, current->parent);

                // if current becomes the root after the update,
                // then exit the loop.
                current = tree->root;
            }
        }
        // current is right
        else
        {
            Node *sibling = current->parent->left;
            // Case1: sibling is rad
            if (sibling->color == rad)
            {
                sibling->color = black;
                current->parent->color = rad;
                subfx_map_rightRotation(tree, current->parent);
                sibling = current->parent->left;
            }

            // Case2 3 4: sibling is black
            // Case2: both sibling's children are black
            if (sibling->left->color == black &&
                sibling->right->color == black)
            {
                sibling->color = rad;

                // if current becomes the root after the update,
                // then exit the loop.
                current = current->parent;
            }
            // Case3 & 4: one of the sibling's child is black
            else
            {
                // case3: the sibling's left child is black,
                //        the other one is rad.
                if (sibling->left->color == black)
                {
                    sibling->right->color = black;
                    sibling->color = rad;
                    subfx_map_leftRotation(tree, sibling);
                    sibling = current->parent->left;
                }

                // After case3, it must enter case4.
                // case4: the sibling's left child is rad,
                //        the other one is black.
                sibling->color = current->parent->color;
                current->parent->color = black;
                sibling->left->color = black;
                subfx_map_rightRotation(tree, current->parent);

                // if current becomes the root after the update,
                // then exit the loop.
                current = tree->root;
            }
        } // end if (current == current->parent->left)
    } // end while (current != tree->root && current->color == black)

    current->color = black; // insure root is black
} // end subfx_map_deleteFixedUp;
