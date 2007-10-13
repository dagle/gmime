/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*  GMime
 *  Copyright (C) 2000-2007 Jeffrey Stedfast
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */


#ifndef __LIST_H__
#define __LIST_H__

#include <glib.h>
#include <string.h>

G_BEGIN_DECLS

typedef struct _ListNode {
	struct _ListNode *next;
	struct _ListNode *prev;
} ListNode;

typedef struct {
	ListNode *head;
	ListNode *tail;
	ListNode *tailpred;
} List;

#define LIST_INITIALIZER(l) { (ListNode *) &l.tail, NULL, (ListNode *) &l.head }

G_GNUC_INTERNAL void list_init (List *list);

G_GNUC_INTERNAL int list_is_empty (List *list);

G_GNUC_INTERNAL int list_length (List *list);

G_GNUC_INTERNAL ListNode *list_unlink_head (List *list);
G_GNUC_INTERNAL ListNode *list_unlink_tail (List *list);

G_GNUC_INTERNAL ListNode *list_prepend_node (List *list, ListNode *node);
G_GNUC_INTERNAL ListNode *list_append_node  (List *list, ListNode *node);

G_GNUC_INTERNAL ListNode *list_node_unlink (ListNode *node);

G_END_DECLS

#endif /* __LIST_H__ */
