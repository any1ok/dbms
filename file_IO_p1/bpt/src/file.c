#include<stdint.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include "file.h"
#include<stdio.h>

header_page_t* temp_h;
int db;
int open_table(char* pathname) {
	db = open(pathname, O_CREAT | O_SYNC | O_RDWR, 0777);
	temp_h = (header_page_t*)malloc(sizeof(header_page_t));

	memset(temp_h, 0, 4096);
	pread(db, temp_h, 4096, 0);
	if (temp_h->num == 0) {
		temp_h->num = 1;
		file_write_page(0, (page_t*)temp_h);
		//
	}

	return 0;
}
pagenum_t file_alloc_page() {
	temp_h = (header_page_t*)malloc(sizeof(header_page_t));

	file_read_page(0, temp_h); 
	if (temp_h->free == 0) {
		pagenum_t asdf = temp_h->num++;
		file_write_page(0, (page_t*)temp_h);
		printf("%d\n", asdf);
		return asdf;
	}
	else {
		pagenum_t temp = temp_h->free;
		page_t* temp_p = (page_t*)malloc(sizeof(page_t));
		pread(db, temp_p, 4096, 4096 * temp_h->free);
		temp_h->free = temp_p->link;
		pwrite(db, temp_h, 4096, 0);
		free(temp_p);
		return temp;
	}
}
//free 하기
void file_free_page(pagenum_t pagenum) {
	page_t* temp_p1 = (page_t*)malloc(sizeof(page_t));
	page_t* temp_p2 = (page_t*)malloc(sizeof(page_t));
	pread(db, temp_p1, 4096, 4096 * pagenum);
	pread(db, temp_p2, 4096, 4096 * temp_h->free);
	temp_p1->link = temp_h->free;
	temp_h->free = pagenum;
	pwrite(db, temp_p1, 4096, 4096 * pagenum);
	pwrite(db, temp_p2, 4096, 4096 * temp_h->free);
	pwrite(db, temp_h, 4096, 0);
	free(temp_p1);
	free(temp_p2); 
	//추가
}
void file_read_page(pagenum_t pagenum, page_t * dest) {
	pread(db, dest, 4096, 4096 * pagenum);

}
void file_write_page(pagenum_t pagenum, const page_t* src) {
	pwrite(db, src, 4096, 4096 * pagenum);

}
