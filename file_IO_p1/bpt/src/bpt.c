#include "file.h"
#include<string.h>
#include<inttypes.h>
#define Version "1.14"
#define leaf_order 4
#define node_order 4

#include <stdlib.h>
#include<stdio.h>
#include "bpt.h"
void usage_1(void) {
	//printf("B+ Tree of Order %d.\n", order);
	printf("Following Silberschatz, Korth, Sidarshan, Database Concepts, "
		"5th ed.\n\n"
		"To build a B+ tree of a different order, start again and enter "
		"the order\n"
		"as an integer argument:  bpt <order>  ");
	printf("(%d <= order <= %d).\n", MIN_ORDER, MAX_ORDER);
	printf("To start with input from a file of newline-delimited integers, \n"
		"start again and enter the order followed by the filename:\n"
		"bpt <order> <inputfile> .\n");
}
void usage_2(void) {
	printf("Enter any of the following commands after the prompt > :\n"
		"\ti <k>  -- Insert <k> (an integer) as both key and value).\n"
		"\tf <k>  -- Find the value under key <k>.\n"
		"\tp <k> -- Print the path from the root to key k and its associated "
		"value.\n"
		"\tr <k1> <k2> -- Print the keys and values found in the range "
		"[<k1>, <k2>\n"
		"\td <k>  -- Delete key <k> and its associated value.\n"
		"\tx -- Destroy the whole tree.  Start again with an empty tree of the "
		"same order.\n"
		"\tt -- Print the B+ tree.\n"
		"\tl -- Print the keys of the leaves (bottom row of the tree).\n"
		"\tv -- Toggle output of pointer addresses (\"verbose\") in tree and "
		"leaves.\n"
		"\tq -- Quit. (Or use Ctl-D.)\n"
		"\t? -- Print this help message.\n");
}
void usage_3(void) {
	printf("Usage: ./bpt [<order>]\n");
	printf("\twhere %d <= order <= %d .\n", MIN_ORDER, MAX_ORDER);
}
int cut(int length) {
	if (length % 2 == 0)
		return length / 2;
	else
		return length / 2 + 1;
}
Queue make_new_queue() {
	Queue q = (Queue)malloc(sizeof(struct Node));
	q->next = NULL;
	return q;
}
int is_empty(Queue queue) {
	return queue->next == NULL;
}
void enqueue(pagenum_t new_pagenum, Queue queue) {
	Queue c = queue;
	Queue end_c = (Queue)malloc(sizeof(struct Node));
	while (c->next != NULL) {
		c = c->next;
	}
	end_c->id = new_pagenum;
	end_c->next = NULL;
	c->next = end_c;
}
/* Helper function for printing the
* tree out.  See print_tree.
*/
pagenum_t dequeue(Queue queue) {
	Queue temp = queue->next;
	pagenum_t ret = temp->id;
	queue->next = temp->next;
	free(temp);
	temp = NULL;
	return ret;
}

int height(node * root) {
	int h = 0;
	node * c = root;
	while (!c->is_leaf) {
		c = c->pointers[0];
		h++;
	}
	return h;
}
int path_to_root(pagenum_t child) {
	int length = 0;

	//file_read_page(0, (page_t*)temp_h); not
	pagenum_t root_page = temp_h->Root;
	page_t* page = (page_t*)malloc(sizeof(page_t));
	file_read_page(child, page);
	pagenum_t parent_page = page->link;
	if (!parent_page) {
		free(page);
		return 0;
	}
	while (parent_page != root_page) {
		file_read_page(parent_page, page);
		parent_page = page->link;
		length++;
	}
	free(page);
	return length + 1;

}
void print_tree() {
	//file_read_page(0, (page_t*)temp_h);
	pagenum_t root = temp_h->Root;
	page_t* temp_p1 = (page_t*)malloc(sizeof(page_t));
	page_t* temp_pp = (page_t*)malloc(sizeof(page_t));
	int i = 0;
	int rank = 0;
	int new_rank = 0;

	if (temp_h->Root == 0) {
		printf("Empty tree.\n");
		return;
	}
	Queue q = make_new_queue();;
	enqueue(root, q);
	rank = path_to_root(root);
	while (!is_empty(q)) {
		pagenum_t n = dequeue(q);
		file_read_page(n, temp_p1);
		//file_read_page(temp_p1->link, temp_pp);

		//if (temp_p1->link != 0 && n == temp_pp->right) {
		new_rank = path_to_root(n);
		if (new_rank != rank) {
			rank = new_rank;
			printf("\n");
		}
		//}
		if (temp_p1->leaf == 0) {
			enqueue(temp_p1->right, q);
		}
		//if (!temp_p1->leaf)
		for (i = 0; i < temp_p1->k_num; i++)
			if (temp_p1->leaf == 0) {
				printf("%"PRId64" ", temp_p1->line[i].key);
				enqueue(temp_p1->line[i].child, q);
			}
			else {
				printf("%"PRId64" ", temp_p1->pack[i].key);
			}
			printf(", ");
			printf("%"PRId64" ", n);
			printf("| ");

	}
	//print_leaves();
	free(temp_p1);
	free(temp_pp);
	printf("\n");
}

void db_insert(int64_t key, char *value) {
	k_v record;
	pagenum_t leaf;
	page_t* temp_p1 = (page_t*)malloc(sizeof(page_t));

	if (db_find(key, value) == 0) {
		free(temp_p1);
		return ;
	}
	record.key = key;
	memcpy(record.value, value, 120 * sizeof(char));
	
	if (temp_h->Root == NULL) {
		pagenum_t allo = file_alloc_page();
		temp_p1->pack[0].key = record.key;
		memcpy(temp_p1->pack[0].value, record.value, 120*sizeof(char));
		//temp_p1->pack[0] = record;
		temp_h->Root = allo;
		temp_p1->k_num++;
		temp_p1->right = NULL;
		temp_p1->leaf = 1;
		temp_p1->link = NULL;
		file_write_page(0,temp_h);
		file_write_page(allo, temp_p1);
		free(temp_p1);
		return;
	}
	leaf = find_leaf( key);
	file_read_page(leaf, temp_p1);
	if (temp_p1->k_num < leaf_order - 1) {
		leaf = insert_into_leaf(leaf,record);

		free(temp_p1);
		return ;
	}
	insert_into_leaf_after_splitting(leaf,record);

	free(temp_p1);
	return;
}

int db_find(int64_t key, char *dest) {
	int i = 0;
	page_t *temp_p1 = (page_t*)malloc(sizeof(page_t));
	pagenum_t root = temp_h->Root;
	pagenum_t c = find_leaf(key);
	if (c == 0) return 1;
	file_read_page(c, temp_p1);
	for (i = 0; i < temp_p1->k_num; i++) {
		if (temp_p1->pack[i].key == key) {
			//strcpy(dest, temp_p1->pack[i].value);
			return 0;
		}
	}
	free(temp_p1);
	return 1;
}

pagenum_t find_leaf(uint64_t key) {
	pagenum_t root = temp_h->Root;
	if (!root)
		return 0;
	int i = 0;
	
	page_t* temp_p1 = (page_t*)malloc(sizeof(page_t));
	file_read_page(root, temp_p1);
	
	
	while (!temp_p1->leaf) {

		i = temp_p1->k_num - 1;
		while (i >= 0) {
			if (key < temp_p1->line[i].key) i--;
			else break;
		}
		if (i == -1) {
			root = temp_p1->right;
		}
		else {
			root = temp_p1->line[i].child;//!!
		}
		file_read_page(root, temp_p1);
	}
	free(temp_p1);
	return root;
}
pagenum_t insert_into_leaf(uint64_t leaf, k_v record) {
	int i, insertion_point;
	page_t* temp_p1 = (page_t*)malloc(sizeof(page_t));
	file_read_page(leaf, temp_p1);
	insertion_point = 0;
	while (insertion_point < temp_p1->k_num && temp_p1->pack[insertion_point].key < record.key)
		insertion_point++;

	for (i = temp_p1->k_num; i > insertion_point; i--) {
		temp_p1->pack[i].key = temp_p1->pack[i - 1].key;
		memcpy(temp_p1->pack[i].value, temp_p1->pack[i - 1].value, 120 * sizeof(char));

	}
	temp_p1->pack[insertion_point].key = record.key;
	memcpy(temp_p1->pack[insertion_point].value, record.value, 120);
	temp_p1->k_num++;
	file_write_page(leaf, temp_p1);
	free(temp_p1);
	return leaf;

}
void insert_into_leaf_after_splitting(pagenum_t leaf,k_v record) {
	pagenum_t new_leaf = file_alloc_page();
	int64_t * temp_keys;
	char ** temp_pointers;
	int insertion_index=0, split, new_key, i, j;
	page_t* temp_p1 = (page_t*)malloc(sizeof(page_t));
	page_t* temp_p2 = (page_t*)malloc(sizeof(page_t));
	file_read_page(new_leaf,temp_p2);
	
	file_read_page(leaf, temp_p1);
	temp_keys = malloc((leaf_order+1) * sizeof(char) * 8);
	if (temp_keys == NULL) {
		perror("Temporary keys array.");
		exit(EXIT_FAILURE);
	}

	temp_pointers = malloc((leaf_order+1) * sizeof(char*));
	for (int i = 0; i <(leaf_order+1); i++) {
		temp_pointers[i] = (char*)malloc(120);
	}
	if (temp_pointers == NULL) {
		perror("Temporary pointers array.");
		exit(EXIT_FAILURE);
	}

	while (insertion_index < leaf_order -1&& temp_p1->pack[insertion_index].key < record.key){
		insertion_index++;
	}
	
	for (i = 0, j = 0; i < temp_p1->k_num; i++, j++) {
		if (j == insertion_index) j++;

		temp_keys[j] = temp_p1->pack[i].key;
		memcpy(temp_pointers[j], temp_p1->pack[i].value, 120 * sizeof(char));

	}
	printf("%d",insertion_index);
	temp_keys[insertion_index] = record.key;
	memcpy(temp_pointers[insertion_index], record.value, 120);

	temp_p1->k_num = 0;
	temp_p2->k_num = 0;
	split = cut(leaf_order-1);
	for (i = 0; i < split; i++) {
		memcpy(temp_p1->pack[i].value, temp_pointers[i], 120 * sizeof(char));
		temp_p1->pack[i].key = temp_keys[i];
		temp_p1->k_num++;

	}

	for (i = split, j = 0; i < leaf_order; i++, j++) {
		memcpy(temp_p2->pack[j].value, temp_pointers[i], 120 * sizeof(char));
		temp_p2->pack[j].key = temp_keys[i];
		temp_p2->k_num++;
		
	}
	for (int i = 0; i < (leaf_order + 1); i++) {
		free(temp_pointers[i]);
	}
	free(temp_pointers);

	free(temp_keys);
	temp_p2->right = temp_p1->right;
	temp_p1->right = new_leaf;
	temp_p2->link = temp_p1->link;
	new_key = temp_p2->pack[0].key;
	temp_p2->leaf = 1;
	file_write_page(leaf, temp_p1);
	file_write_page(new_leaf, temp_p2);
	free(temp_p1);
	free(temp_p2);
	insert_into_parent(leaf, new_key, new_leaf);
	return;
}
void insert_into_parent(pagenum_t left, int64_t key, pagenum_t right) {
	pagenum_t node_insertion;
	pagenum_t parent;
	page_t* temp_pl = (page_t*)malloc(sizeof(page_t));
	page_t* temp_pr = (page_t*)malloc(sizeof(page_t));
	page_t* temp_pp = (page_t*)malloc(sizeof(page_t));
	file_read_page(left, temp_pl);
	file_read_page(right, temp_pr);
	parent = temp_pl->link;
	file_read_page(parent,temp_pp);
	if (parent == 0) {
		free(temp_pl);
		free(temp_pr);
		free(temp_pp);
		insert_into_new_root(left, key, right);
		return;
	}
	node_insertion = get_insertion_index(parent, left);
	if (temp_pp->k_num < node_order-1) {
		free(temp_pl);
		free(temp_pr);
		free(temp_pp);
		 insert_into_node(parent, node_insertion, key, right);
		 return;
	}
	
	free(temp_pl);
	free(temp_pr);
	free(temp_pp);
	insert_into_node_after_splitting(parent, node_insertion, key, right);
	return;

}

void insert_into_new_root(pagenum_t left, int64_t key, pagenum_t right) {
	page_t* temp_pl = (page_t*)malloc(sizeof(page_t));
	page_t* temp_pr = (page_t*)malloc(sizeof(page_t));
	
	page_t* root = (page_t*)malloc(sizeof(page_t));
	memset(root, 0, 4096);
	file_read_page(left, temp_pl);
	file_read_page(right, temp_pr);
	uint64_t allo = file_alloc_page();
	temp_h->Root = allo;
	root->line[0].key = key;
	root->line[0].child = right;
	root->right = left;
	root->k_num++;
	root->link = NULL;
	temp_pl->link = allo;
	temp_pr->link = allo;
	file_write_page(left, temp_pl);
	file_write_page(right, temp_pr);
	file_write_page(0, temp_h);
	file_write_page(allo, root);
	free(temp_pl);
	free(temp_pr);

	free(root);
	return;
}

int get_insertion_index(pagenum_t parent, pagenum_t left) {

	int left_index = 0;
	page_t *temp_pl = (page_t*)malloc(sizeof(page_t));
	page_t *temp_pp = (page_t*)malloc(sizeof(page_t));
	file_read_page(left, temp_pl);
	file_read_page(parent, temp_pp);
	if (temp_pp->right == left) {
		
		free(temp_pl);
		free(temp_pp);
		return 0;
	
	}
	while (left_index < /*??*/temp_pp->k_num &&  temp_pp->line[left_index].child != left)
		left_index++;
	left_index++;
	free(temp_pl);
	free(temp_pp);
	return left_index;
}

void insert_into_node(pagenum_t parent,
	int left_index, int64_t key, pagenum_t right) {
	int i = 0;
	page_t *temp_p1 = (page_t*)malloc(sizeof(page_t));
	page_t *temp_p2 = (page_t*)malloc(sizeof(page_t));
	pagenum_t root = temp_h->Root;
	file_read_page(parent, temp_p1);
	file_read_page(right, temp_p2);
	for (i = temp_p1->k_num; i > left_index; i--) {
		
		temp_p1->line[i+1].child = temp_p1->line[i].child;
		temp_p1->line[i + 1].key = temp_p1->line[i].key;

	}
	temp_p1->line[left_index].child = right;
	temp_p1->line[left_index].key = key;
	temp_p1->k_num++;
	file_write_page(parent, temp_p1);
	file_write_page(right, temp_p2);
	free(temp_p1);
	free(temp_p2);
	return;
}

void insert_into_node_after_splitting(pagenum_t old_node, int left_index,
	int key, pagenum_t right) {
	int i, j, split, k_prime;
	page_t* temp_pp = (page_t*)malloc(sizeof(page_t));
	file_read_page(old_node, temp_pp);
	
	int64_t * temp_keys;
	pagenum_t * temp_pointers;


	temp_keys = malloc((node_order) * sizeof(char) * 8);
	temp_pointers = malloc((node_order) * sizeof(char) * 8);



	for (i = 0, j = 0; i < temp_pp->k_num; i++, j++) {
		if (j == left_index ) j++;
		temp_pointers[j]= temp_pp->line[i].child;
		temp_keys[j] = temp_pp->line[i].key;
	}

	temp_pointers[left_index] = right;
	temp_keys[left_index] = key;

	/* Create the new node and copydd 
	* half the keys and pointers to the
	* old and half to the new.
	*/
	split = cut(node_order);
	pagenum_t new_node = file_alloc_page();
	page_t* temp_pn = (page_t*)malloc(sizeof(page_t));
	file_read_page(new_node, temp_pn);
	temp_pp->k_num =0;
	temp_pn->k_num = 0;
	for (i = 0; i < split - 1; i++) {
		temp_pp->line[i].child = temp_pointers[i];
		temp_pp->line[i].key = temp_keys[i];
		temp_pp->k_num++;

	}
	temp_pn->right = temp_pointers[i++];
	k_prime = temp_keys[split - 1];

	for (i, j = 0; i < node_order; i++, j++) {
		temp_pn->line[j].child = temp_pointers[i];
		temp_pn->line[j].key = temp_keys[i];
		temp_pn->k_num++;
	}

	free(temp_pointers);
	free(temp_keys);
	temp_pn->leaf = 0;
	
	temp_pn->link = temp_pp->link;
	page_t* temp_pc = (page_t*)malloc(sizeof(page_t));
	file_read_page(temp_pn->right, temp_pc);
	temp_pc->link = new_node;
	file_write_page(temp_pn->right, temp_pc);
	for (i = 0; i < temp_pn->k_num; i++) {
		file_read_page(temp_pn->line[i].child, temp_pc);
		temp_pc->link = new_node;
		file_write_page(temp_pn->line[i].child, temp_pc);
	}
	file_write_page(old_node, temp_pp);
	file_write_page(new_node,temp_pn);
	free(temp_pn);
	free(temp_pp);
	
	free(temp_pc);
	insert_into_parent(old_node, k_prime, new_node);
	return;
}

/////////////////

int db_delete(int64_t key) {

	char *value = (char*)malloc(sizeof(char) * 120);
	pagenum_t key_leaf;
	int ck = db_find(key, value);
	key_leaf = find_leaf( key);
	if (ck == 0 && key_leaf != NULL) {
		delete_entry(key_leaf, key);//
	}
	free(value);
	return 0;
}
void delete_entry(pagenum_t n, int64_t key) {
	page_t* temp_p1 = (page_t*)malloc(sizeof(page_t));
	page_t* temp_pp = (page_t*)malloc(sizeof(page_t));
	int min_keys;
	pagenum_t neighbor;
	int neighbor_index;
	int k_prime_index;
	pagenum_t k_prime;
	int capacity;
	remove_entry_from_node(n, key);
	file_read_page(n,temp_p1);
	file_read_page(temp_p1->link,temp_pp);;
	if (n == temp_h->Root) {
		free(temp_p1);
		free(temp_pp);
		adjust_root();
		return;
	}
	min_keys = 1;
	if (temp_p1->k_num >= min_keys) {

		free(temp_p1);
		free(temp_pp);
		return;
	}
	neighbor_index = get_neighbor_index(n);
	
	if (neighbor_index == -2) {
		neighbor_index = 0;
		k_prime_index = 0;
		neighbor = temp_pp->line[neighbor_index].child;
		k_prime = temp_pp->line[k_prime_index].key;
	}
	else if (neighbor_index == -1) {
		k_prime_index = neighbor_index + 1;
		neighbor = temp_pp->right;
		k_prime = temp_pp->line[k_prime_index].key;
	}
	else {
		k_prime_index = neighbor_index + 1;
		neighbor = temp_pp->line[neighbor_index].child;
		k_prime = temp_pp->line[k_prime_index].key;
	}
	page_t* temp_pne = (page_t*)malloc(sizeof(page_t));
	file_read_page(neighbor, temp_pne);
	
	if (temp_pne->k_num + temp_p1->k_num <= 1) {

		free(temp_p1);
		free(temp_pp);
		coalesce_nodes(n, neighbor, neighbor_index, k_prime);
		return;
	}
	else {

		free(temp_p1);
		free(temp_pp);
		redistribute_nodes(n, neighbor, neighbor_index, k_prime_index, k_prime);
		return;
	}
}

void remove_entry_from_node(pagenum_t n, int64_t key) {

	int i, num_pointers;
	page_t* temp_p1 = (page_t*)malloc(sizeof(page_t));
	file_read_page(n, temp_p1);
	// Remove the key and shift other keys accordingly.
	i = 0;
	if (temp_p1->leaf) {

		while (temp_p1->pack[i].key != key)
			i++;
		for (++i; i < temp_p1->k_num; i++)
			temp_p1->pack[i - 1] = temp_p1->pack[i];
	}
	else {
		while (temp_p1->line[i].key != key)
			i++;
		for (++i; i < temp_p1->k_num; i++)
			temp_p1->line[i - 1] = temp_p1->line[i];

	}

	// Remove the pointer and shift other pointers accordingly.
	// First determine number of pointers.

	

	// One key fewer.
	temp_p1->k_num--;
	file_write_page(n, temp_p1);
	free(temp_p1);
	return ;
}

void adjust_root() {
	pagenum_t root = temp_h->Root;
	
	page_t* temp_p2 = (page_t*)malloc(sizeof(page_t));
	
	page_t* temp_p1 = (page_t*)malloc(sizeof(page_t));
	file_read_page(root, temp_p1);
	/* Case: nonempty root.
	* Key and pointer have already been deleted,
	* so nothing to be done.
	*/
	file_read_page(temp_p1->right,temp_p2);
	
	if (temp_p1->k_num > 0) {
		free(temp_p1);
		free(temp_p2);
		return;

	}

	/* Case: empty root.
	*/

	// If it has a child, promote 
	// the first (only) child
	// as the new root.
	file_free_page(temp_h->Root);
	if (!temp_p1->leaf) {
		temp_h->Root = temp_p1->right;
		temp_p2->link = NULL;
		file_write_page(0,temp_h);
		file_write_page(temp_h->Root , temp_p2);
		free(temp_p1);
		free(temp_p2);
		return ;
	}

	// If it is a leaf (has no children),
	// then the whole tree is empty.

	else{
		temp_h->Root = NULL;
		file_write_page(0,temp_h);
		free(temp_p1);
		free(temp_p2);
		return;
		}
	
	

}

int get_neighbor_index(pagenum_t n) {

	int i;
	page_t* temp_p1 = (page_t*)malloc(sizeof(page_t));
	file_read_page(n, temp_p1);
	page_t* temp_pp = (page_t*)malloc(sizeof(page_t));
	file_read_page(temp_p1->link, temp_pp);
	/* Return the index of the key to the left
	* of the pointer in the parent pointing
	* to n.
	* If n is the leftmost child, this means
	* return -1.
	*/
	
	if (temp_pp->right == n){
		free(temp_p1);
		free(temp_pp);
		return -2;

}
	
	for (i = 0; i <= temp_pp->k_num; i++){
		if (temp_pp->line[i].child == n){
			int k =i-1;
			free(temp_p1);
			free(temp_pp);
		
			return k;
			}
	}
}

void coalesce_nodes(pagenum_t n, pagenum_t neighbor, int neighbor_index, int64_t k_prime) {
	pagenum_t root = temp_h->Root;
	int i, j, neighbor_insertion_index, n_end;
	pagenum_t tmp;
	page_t* temp_pp = (page_t*)malloc(sizeof(page_t));
	page_t* temp_pn = (page_t*)malloc(sizeof(page_t));
	page_t* temp_pne = (page_t*)malloc(sizeof(page_t));
	file_read_page(n, temp_pn);
	file_read_page(temp_pn->link, temp_pp);
	if (temp_pp->right == n) {
		tmp = n;
		n = neighbor;
		neighbor = tmp;
	}

	file_read_page(n, temp_pn);
	file_read_page(neighbor, temp_pne);
	neighbor_insertion_index = temp_pne->k_num;

	if (!temp_pn->leaf) {

		/* Append k_prime.
		*/

		temp_pne->line[neighbor_insertion_index].key = k_prime;//
		temp_pne->line[neighbor_insertion_index].child = temp_pn->right;
		temp_pne->k_num++;
		

		n_end = temp_pn->k_num;

		for (i = neighbor_insertion_index + 1, j = 0; j < n_end; i++, j++) {
			temp_pne->line[i] = temp_pn->line[j];
			temp_pne->k_num++;
			temp_pn->k_num--;
		}
		
		/* The number of pointers is always
		* one more than the number of keys.
		*/



		/* All children must now point up to the same parent.
		*/
		page_t* tmp = (page_t*)malloc(sizeof(page_t));
		file_read_page(temp_pne->right, tmp);
		tmp->link = neighbor;
		file_write_page(temp_pne->right, tmp);
		for (i = 0; i < temp_pne->k_num; i++) {
			file_read_page(temp_pne->line[i].child, tmp);
			tmp->link = neighbor;
			file_write_page(temp_pne->right, tmp);
		}
		
		file_write_page(n, temp_pn);
		file_write_page(neighbor, temp_pne);
		file_write_page(temp_pne->link, temp_pp);
		file_free_page(n);
		free(temp_pn);
		free(temp_pp);
		free(temp_pne);
		free(tmp);
		return;

	}

	/* In a leaf, append the keys and pointers of
	* n to the neighbor.
	* Set the neighbor's last pointer to point to
	* what had been n's right neighbor.
	*/

	else {
		for (i = neighbor_insertion_index, j = 0; j < temp_pn->k_num; i++, j++) {
			temp_pne->line[i] = temp_pn->line[j];
			temp_pne->k_num++;
			temp_pn->k_num--;
		}
		temp_pne->right = temp_pn->right;
	}
	file_write_page(n, temp_pn);
	file_write_page(neighbor, temp_pne);
	file_write_page(temp_pne->link, temp_pp);
	file_free_page(n);
	delete_entry(temp_pn->link, k_prime);
	
	free(temp_pn);
	free(temp_pne);
	free(temp_pp);
	return ;
}

void redistribute_nodes( pagenum_t n, pagenum_t neighbor, int neighbor_index,
	int k_prime_index, int64_t k_prime) {
	page_t* temp_pp = (page_t*)malloc(sizeof(page_t));
	page_t* temp_pr = (page_t*)malloc(sizeof(page_t));
	page_t* temp_pne = (page_t*)malloc(sizeof(page_t));
	page_t* temp_pn = (page_t*)malloc(sizeof(page_t));
	pagenum_t root = temp_h->Root;
	
	file_read_page(root, temp_pr);
	file_read_page(n, temp_pn);
	file_read_page(neighbor, temp_pne);
	file_read_page(temp_pn->link, temp_pp);
	/* Case: n has a neighbor to the left.
	* Pull the neighbor's last key-pointer pair over
	* from the neighbor's right end to n's left end.
	*/
	int i = 0;
	printf("\n");
	if (!(neighbor_index==0&&k_prime_index==0))//hgh
	{
	

		if (!temp_pn->leaf) {
			for (i = temp_pn->k_num; i > 0; i--) {
				temp_pn->line[i] = temp_pn->line[i - 1];

			}
			temp_pn->line[0].child = temp_pn->right;
			
		}
		if (!temp_pn->leaf) {
			
			temp_pn->right = temp_pne->line[temp_pne->k_num - 1].child;
			page_t* tmp = (page_t*)malloc(sizeof(page_t));
			file_read_page(temp_pn->right, tmp);
			tmp->link = n;
			temp_pne->line[temp_pne->k_num - 1].child = NULL;
			temp_pn->line[0].key = k_prime;
			temp_pp->line[k_prime_index].key = temp_pne->line[temp_pne->k_num - 1].key;

		}
		else {
			printf("%d",neighbor_index);
			for (i = temp_pn->k_num; i > 0; i--) {
				temp_pn->pack[i] = temp_pn->pack[i - 1];

			}
			temp_pn->pack[0] = temp_pne->pack[temp_pne->k_num - 1];
			temp_pp->line[k_prime_index].key = temp_pne->pack[temp_pne->k_num - 1].key;

		}
	}

	/* Case: n is the leftmost child.
	* Take a key-pointer pair from the neighbor to the right.
	* Move the neighbor's leftmost key-pointer pair
	* to n's rightmost position.
	*/

	else {
		
		if (temp_pn->leaf) {
			printf("%d\n",temp_pn->k_num);
			printf("%d\n",temp_pp->line[k_prime_index].key);
			printf("%d\n",temp_pne->pack[1].key);
			temp_pn->pack[temp_pn->k_num] = temp_pne->pack[0];
			temp_pp->line[k_prime_index].key = temp_pne->pack[1].key;
			for (i = 0; i <= temp_pne->k_num - 2; i++) {
				temp_pne->pack[i] = temp_pne->pack[i + 1];
			}
		}
		else {
			temp_pn->line[temp_pn->k_num].key = k_prime;
			temp_pn->line[temp_pn->k_num].child = temp_pne->right;
			temp_pp->line[k_prime_index].key = temp_pne->line[0].key;

			temp_pne->right = temp_pne->line[0].child;
			for (i = 0; i <= temp_pne->k_num - 1; i++) {
				temp_pne->line[i - 1].child = temp_pne->line[i].child;
			}
			for (i = 1; i <= temp_pne->k_num - 1; i++) {
				temp_pne->line[i].key = temp_pne->line[i + 1].key;
			}
		}

	}

	/* n now has one more key and one more pointer;
	* the neighbor has one fewer of each.
	*/
	temp_pn->k_num++;
	temp_pne->k_num--;
	file_write_page(root, temp_pr);
	file_write_page(n, temp_pn);
	file_write_page(neighbor, temp_pne);
	file_write_page(temp_pn->link, temp_pp);
	free(temp_pr);
	free(temp_pn);
	free(temp_pne);
	free(temp_pp);
	return;
}
