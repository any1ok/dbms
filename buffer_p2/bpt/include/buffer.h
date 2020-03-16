#ifndef __BUFFER_H__
#define __BUFFER_H__
#include "file.h"
#include "bpt.h"

#include<vector>
using namespace std;
typedef struct buffer {
	//union {
	page_t* frame;
		//header_page_t * head_frame;
	//};
	int table_id;
	int page_num;
	int is_dirty;
	int is_pinned;

}buffer;

typedef struct buf_frame {
	vector <buffer*> buf; 
	header_page_t * header[11];
	int size;
	int now;
	int page_num;
	int head_num;
	pagenum_t free;
}buf_frame;

extern buf_frame buf_m;
int init_db(int num_buf);
int close_table(int table_id);
int shutdown_db();
extern uint16_t buffer_size;
int buffer_read_page(int table_id, pagenum_t n, page_t * dest);
void buffer_write_page(int table_id, pagenum_t n, page_t * dest);
#endif 
