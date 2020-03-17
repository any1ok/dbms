#include<stdint.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include"buffer.h"
#include"file.h"
#include "bpt.h"

#include<stdio.h>



int init_db(int num_buf) {
	buf_m.size = num_buf;
	for (int i = 0; i < num_buf;i++) {
		buffer* input = (buffer*)malloc(sizeof(buffer));
		input->table_id = 0;
		input->is_dirty = 0;
		input->is_pinned = 0;
		input->page_num = 0;
		buf_m.buf.push_back(input);
	}
	return buf_m.buf.size();
}
int close_table(int table_id) {
	
	file_write_page(table_id, (page_t*)buf_m.header[table_id]);
	for (int i = 0; i < buf_m.size; i++) {
		if (buf_m.buf[i]->table_id == table_id) {
			file_write_page(buf_m.buf[i]->page_num, buf_m.buf[i]->frame);
			free(buf_m.buf[i]->frame);			
			free(buf_m.buf[i]);
			buf_m.buf.erase(buf_m.buf.begin() + i);
			buffer* input = (buffer*)malloc(sizeof(buffer));
			input->table_id = 0;
			input->is_dirty = 0;
			input->is_pinned = 0;
			input->page_num = 0;
			buf_m.buf.push_back(input);
		}
	}
	return table_id;

}
int shutdown_db() {
	for (int i = 1; i <= 10; i++) {
		file_write_page(i, (page_t*)buf_m.header[i]);
	
	}
	for (int i = 0; i < buf_m.size; i++) {
		file_write_page(buf_m.buf[i]->page_num, buf_m.buf[i]->frame);

	}
}

int find_space() {
	int i;
	for (i = 0; i < buf_m.size; i++) {
		if (buf_m.buf[i]->table_id == 0) {
			free(buf_m.buf[i]->frame);			
			free(buf_m.buf[i]);			
			buf_m.buf.erase(buf_m.buf.begin() + i);
			
			return i;
			
		}
		
	}

	for (i = 0; i < buf_m.size; i++) {
		if (buf_m.buf[i]->is_dirty == 0 && buf_m.buf[i]->is_pinned == 0) {
			file_write_page(buf_m.buf[i]->page_num, buf_m.buf[i]->frame);
			free(buf_m.buf[i]->frame);			
			free(buf_m.buf[i]);			
			buf_m.buf.erase(buf_m.buf.begin() + i);
			
			return i;
		}
		
	}
	for (i = 0; i < buf_m.size; i++) {
		if (buf_m.buf[i]->is_pinned == 0) {
			file_write_page(buf_m.buf[i]->page_num, buf_m.buf[i]->frame);
			free(buf_m.buf[i]->frame);			
			free(buf_m.buf[i]);			
			buf_m.buf.erase(buf_m.buf.begin() + i);
			
			return i;
		}

	}
	file_write_page(buf_m.buf[0]->page_num, buf_m.buf[0]->frame);
	free(buf_m.buf[0]->frame);			
	free(buf_m.buf[0]);
	buf_m.buf.erase(buf_m.buf.begin());
	
}

int buffer_read_page(int table_id,pagenum_t n, page_t * dest) {
	if(n ==0)
		return 0;
	buffer * in = (buffer*)malloc(sizeof(buffer));
	page_t* in_frame = (page_t*)malloc(sizeof(page_t));
	
	for (int i = 0; i < buf_m.size; i++) {
		if (buf_m.buf[i]->page_num == n) {
			memcpy(dest,buf_m.buf[i]->frame,sizeof(page_t));
			memcpy(in_frame,dest,sizeof(page_t));
			in->frame = in_frame;
			in->is_pinned =1;
			in->table_id = table_id;
			in->page_num = n;
			
			buf_m.buf.erase(buf_m.buf.begin() + i);
			buf_m.buf.push_back(in);
			return 0;
		}
	}
	int space = find_space();
	
	file_read_page(n, dest);
	
	memcpy(in_frame,dest,sizeof(page_t));
	in->frame = in_frame;
	in->is_dirty = 0;
	in->is_pinned = 1;
	in->table_id = table_id;
	in->page_num = n;
	
	buf_m.buf.push_back(in);
	

	return 0;

}

void buffer_write_page(int table_id,pagenum_t n, page_t * dest) {
	if(n==0)
	return ;
	buffer * in=(buffer*)malloc(sizeof(buffer));;
	page_t* in_frame = (page_t*)malloc(sizeof(page_t));
	for (int i = 0; i < buf_m.size; i++) {
		if (buf_m.buf[i]->page_num == n) {
			memcpy(in_frame,dest,sizeof(page_t));
			in->frame = in_frame;
			in->is_dirty =1;
			in->is_pinned =0;
			in->table_id = table_id;
			in->page_num = n;
			buf_m.buf.erase(buf_m.buf.begin() + i);
			buf_m.buf.push_back(in);
			
			return ;
		}
	}
	int space = find_space();
	
	
	memcpy(in_frame,dest,sizeof(page_t));
	in->frame = in_frame;
	in->is_dirty = 1;
	in->is_pinned = 0;
	in->table_id = table_id;
	in->page_num = n;
	buf_m.buf.push_back(in);
	return ;
}



