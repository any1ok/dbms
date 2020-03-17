#include"bpt.h"
#include"buffer.h"
#include"file.h"
#include"join.h"
#include"trx.h"
int begin_trx() {//시작해서 할당하고 그번호 초기화
	if (trx_m.state[0] == 0) {
		trx_m.state[0] = 1;
		trx_m.trx = (trx_t**)malloc(sizeof(trx_t*) * 11);
	}
	int id=1;
	while (trx_m.state[id]) {
		trx_m.trx[id] = (trx_t*)malloc(sizeof(trx_t));
		trx_m.trx[id]->state = 0;
		trx_m.trx[id]->trx_id = id;
		trx_m.trx[id]->wait_lock = NULL;
		id++;
	}
	
	return id;
}

int end_trx(int tid) {//끝났으면 이제 써주고 끝내야지
	trx_m.state[tid] == 0;
	while (!trx_m.trx[tid]->trx_locks.empty()) {
		pagenum_t pt = trx_m.trx[tid]->trx_locks.front()->pagenum;
		int key =trx_m.trx[tid]->trx_locks.front()->log.key;
		int table_id = trx_m.trx[tid]->trx_locks.front()->table_id;
		//레코드 저장
		char * record = (char*)malloc(sizeof(char) * 120);
		memcpy(record, trx_m.trx[tid]->trx_locks.front()->log.record, sizeof(char) * 120);
		if (pt < 11) {

		}
		else {
			page_t * temp_p = (page_t*)malloc(sizeof(page_t));
			buffer_read_page(table_id, pt, temp_p);

		}

		trx_m.trx[tid]->trx_locks.erase(trx_m.trx[tid]->trx_locks.begin());
	}
}


int db_find(int table_id, int64_t key, char *dest,int trx_id) {
	int i = 0;
	page_t *temp_p1 = (page_t*)malloc(sizeof(page_t));
	lock_t* lock;
	lock->lock = 0;
	lock->my_trx = trx_m.trx[trx_id];
	pagenum_t c = find_leaf(table_id, key);
	if (c == 0) return 1;
	buffer_read_page(table_id, c, temp_p1);
	for (i = 0; i < temp_p1->k_num; i++) {
		if (temp_p1->pack[i].key == key) {
			
			strcpy(dest, temp_p1->pack[i].value);
			return 0;
		}
	}
	free(temp_p1);
	return 1;
}

int db_update(int table_id, int64_t key, char *dest, int trx_id) {
	int i = 0;
	page_t *temp_p1 = (page_t*)malloc(sizeof(page_t));
	pagenum_t c = find_leaf(table_id, key);
	if (c == 0) return 1;
	buffer_read_page(table_id, c, temp_p1);
	for (i = 0; i < temp_p1->k_num; i++) {
		if (temp_p1->pack[i].key == key) {
			////
			strcpy(dest, temp_p1->pack[i].value);
			return 0;
		}
	}
	free(temp_p1);
	return 1;
}
