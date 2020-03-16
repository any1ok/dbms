#include<stdint.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include "file.h"
#include "buffer.h"
#include "bpt.h"
#include<stdio.h>

header_page_t* temp_h;
int db;
int open_table(const char* pathname) {
	

	db = open(pathname, O_CREAT | O_SYNC | O_RDWR, 0777);
	
	for(int i=1;i<=10;i++){
	header_page_t* alloc_h = (header_page_t*)malloc(sizeof(header_page_t));
	pread(db, alloc_h, 4096, 4096 * i);
	buf_m.header[i] = alloc_h;
	}		
		
	
	if (buf_m.head_num > 10 || buf_m.head_num<0) {
		printf("lol");
		return 0;
	}
	

	
	int table_id = buf_m.head_num;
	return buf_m.head_num;
}
pagenum_t file_alloc_page() {

	if (buf_m.free==0) {
		buf_m.page_num++;
		pagenum_t asdf = buf_m.page_num+10;
		return asdf;
	}
	else {
		pagenum_t temp = buf_m.free;
		page_t* temp_p = (page_t*)malloc(sizeof(page_t));
		pread(db, temp_p, 4096, 4096 * buf_m.free);
		buf_m.free = temp_p->link;
		free(temp_p);
		return temp;
	}
}
//free \C7ϱ\E2
void file_free_page(pagenum_t pagenum) {
	page_t* temp_p1 = (page_t*)malloc(sizeof(page_t));
	page_t* temp_p2 = (page_t*)malloc(sizeof(page_t));
	pread(db, temp_p1, 4096, 4096 * pagenum);
	temp_p1->link = buf_m.free;
	buf_m.free = pagenum;
	pwrite(db, temp_p1, 4096, 4096 * pagenum);
	free(temp_p1);
	//\C3߰\A1
}
void file_read_page(pagenum_t pagenum, page_t * dest) {
	pread(db, dest, 4096, 4096 * pagenum);

}
void file_write_page(pagenum_t pagenum, const page_t* src) {
	pwrite(db, src, 4096, 4096 * pagenum);

}
