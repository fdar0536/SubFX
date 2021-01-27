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

#pragma once

#include <inttypes.h>

#include "include/internal/map.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum NodeColor
{
    rad, black
} NodeColor;

typedef struct Node
{
    uint8_t *key;

    uint8_t *value;

    NodeColor color;

    struct Node *parent;

    struct Node *left;

    struct Node *right;
} Node;

typedef struct RBTree
{
    subfx_types id;

    Node *root;

    Node *nil;

    subfx_cmpFunc keyCmpFunc;

    subfx_freeFunc keyFreeFunc;

    subfx_freeFunc valueFreeFunc;
} RBTree;

subfx_map *subfx_map_init();

subfx_handle subfx_map_create(subfx_cmpFunc,
                              subfx_freeFunc,
                              subfx_freeFunc);

subfx_exitstate subfx_map_destroy(subfx_handle);

void *subfx_map_at(subfx_handle, void *);

subfx_exitstate subfx_map_insertNode(subfx_handle, void *, void *);

subfx_exitstate subfx_map_deleteNode(subfx_handle, void *);

Node *createNode(Node *nil);

void destroyNode(Node *, subfx_freeFunc, subfx_freeFunc);

void subfx_map_clear(RBTree *, Node *);

void subfx_map_leftRotation(RBTree *, Node *);

void subfx_map_rightRotation(RBTree *, Node *);

Node *subfx_map_searchNode(RBTree *, void *);

void subfx_map_insertFixedUp(RBTree *, Node *);

Node *subfx_map_nodeLeftmost(RBTree *, Node *);

Node *subfx_map_nodeSuccessor(RBTree *, Node *);

void subfx_map_deleteFixedUp(RBTree *, Node *);

#ifdef __cplusplus
}
#endif
