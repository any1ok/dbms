#ifndef __FILE_H__
#define __FILE_H__

#include<stdint.h>

typedef uint64_t pagenum_t;
typedef struct {
	int64_t key;
	char value[120];
}k_v;

typedef struct {
	int64_t key;
	pagenum_t child;
}k_c;
typedef struct Node* PtrToNode;
typedef PtrToNode Queue;
struct Node {
	pagenum_t id;
	Queue next;
};
typedef struct page_t {
	pagenum_t link;
	int leaf;
	int k_num;
	int reser[26];
	pagenum_t right;
	union {
		k_v pack[31];
		k_c line[248];
	};
}page_t;

typedef struct head {
	pagenum_t free;
	pagenum_t Root;
	pagenum_t num;
	char reserved[4072];
}header_page_t;
int open_table(const char* pathname);


pagenum_t file_alloc_page();


void file_free_page(pagenum_t pagenum);
void file_read_page(pagenum_t pagenum, page_t * dest);
void file_write_page(pagenum_t pagenum, const page_t* src);
#endif
