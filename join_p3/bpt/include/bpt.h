#ifndef __BPT_H__
#define __BPT_H__

// Uncomment the line below if you are compiling on Windows.
// #define WINDOWS
#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "buffer.h"
#ifdef WINDOWS
#define bool char
#define false 0
#define true 1
#endif

// Default order is 4.
#define DEFAULT_ORDER 4

// Minimum order is necessarily 3.  We set the maximum
// order arbitrarily.  You may change the maximum order.
#define MIN_ORDER 3
#define MAX_ORDER 20

// Constants for printing part or all of the GPL license.
#define LICENSE_FILE "LICENSE.txt"
#define LICENSE_WARRANTEE 0
#define LICENSE_WARRANTEE_START 592
#define LICENSE_WARRANTEE_END 624
#define LICENSE_CONDITIONS 1
#define LICENSE_CONDITIONS_START 70
#define LICENSE_CONDITIONS_END 625


typedef struct record {
	int value;
} record;


typedef struct node {
	void ** pointers;
	int * keys;
	struct node * parent;
	bool is_leaf;
	int num_keys;
	struct node * next; // Used for queue.
} node;

// GLOBALS.

extern int order;

extern node * queue;

extern bool verbose_output;





void usage_1(void);
void usage_2(void);
void usage_3(void);
void enqueue(pagenum_t, Queue);
pagenum_t dequeue(Queue);

void print_tree(int table_id);
int path_to_root(pagenum_t);




pagenum_t find_leaf(int table_id,uint64_t key);
int db_find(int table_id,int64_t key, char *dest);
int cut(int length);
int path_to_root(int table_id,pagenum_t child);
// Insertion.

int get_insertion_index(int table_id,pagenum_t parent, pagenum_t left);
pagenum_t insert_into_leaf(int table_id,uint64_t leaf, k_v record);
void insert_into_leaf_after_splitting(int table_id,pagenum_t leaf, k_v record);
void insert_into_node(int table_id,pagenum_t parent,
	int left_index, int64_t key, pagenum_t right);
void insert_into_node_after_splitting(int table_id,pagenum_t old_node, int left_index,
	int key, pagenum_t right);
void insert_into_parent(int table_id,pagenum_t left, int64_t key, pagenum_t right);
void insert_into_new_root(int table_id,pagenum_t left, int64_t key, pagenum_t right);//

void db_insert(int table_id,int64_t key, char *value);

// Deletion.
void remove_entry_from_node(int table_id,pagenum_t n,int64_t key);
int get_neighbor_index(int table_id,pagenum_t n);
void adjust_root(int table_id);
void coalesce_nodes(int table_id,pagenum_t n, pagenum_t neighbor, int neighbor_index, int64_t k_prime);
void redistribute_nodes(int table_id,pagenum_t n, pagenum_t neighbor, int neighbor_index,
	int k_prime_index, int64_t k_prime);
void delete_entry(int table_id,pagenum_t n, int64_t key);
int db_delete(int table_id,int64_t key);


#endif /* __BPT_H__*/
