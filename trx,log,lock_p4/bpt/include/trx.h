#ifndef __TRX_H__
#define __TRX_H__
#include<list>
#include"bpt.h"
#include"buffer.h"
#include"file.h"
#include"join.h"
#include"lock.h"
using namespace std;


struct trx_t {
	int trx_id;
	int state; //IDLE==0,RUNNING==1,WATTING==2;
	list<lock_t*> trx_locks;
	lock_t* wait_lock;
};

struct trx_manage {
	trx_t** trx;
	int state[11];

};
extern trx_manage trx_m;

int db_find(int table_id, int64_t key, char *dest, int trx_id);
int db_update(int table_id, int64_t key, char *dest, int trx_id);
#endif