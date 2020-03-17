#ifndef __LOCK_H__
#define __LOCK_H__
#include"bpt.h"
#include"buffer.h"
#include"file.h"
#include"join.h"
#include"trx.h"
#include<map>
extern map<pair<pagenum_t, int>, list<lock_t*>> hash;
struct lock_t {
	int table_id;
	Log log;
	pagenum_t pagenum;
	int lock; // 0 shared ,1 ex
	trx_t* my_trx; //
};

struct Log {
	int key;
	char * record;

};
#endif