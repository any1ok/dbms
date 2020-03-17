
#include "bpt.h"
#include "buffer.h"
#include "file.h"
#include "join.h"
#include <inttypes.h>
int join_table(int table_id_1, int table_id_2) {

	pagenum_t pn1 = buf_m.header[table_id_1]->Root;
	pagenum_t pn2 = buf_m.header[table_id_2]->Root;
	page_t* p1 = (page_t*)malloc(sizeof(page_t));
	page_t* p2 = (page_t*)malloc(sizeof(page_t));
	int h1 = 0;
	int h2 = 0;
	buffer_read_page(table_id_1, pn1, p1);
	buffer_read_page(table_id_2, pn2, p2);
	while (p1->leaf == 0) {

		pn1 = p1->right;
		buffer_read_page(table_id_1, pn1, p1);
	}
	while (p2->leaf == 0) {

		pn2 = p2->right;
		buffer_read_page(table_id_1, pn2, p2);
	}

	int i = 0;
	int j = 0;

	while (1) {
		if (p1->pack[i].key == p2->pack[j].key) {
			printf("%"PRId64" ", p1->pack[i].key);
			i++;
			j++;
			if (p1->k_num == i) {
				if (p1->right == 0)
					return 0;
				buffer_read_page(table_id_1, p1->right, p1);
				i = 0;
			}
			if (p2->k_num == j) {
				if (p2->right == 0)
					return 0;
				buffer_read_page(table_id_2, p2->right, p2);
				j = 0;
			}

		}
		else if (p1->pack[i].key > p2->pack[j].key) {
			j++;
			if (p2->k_num == j) {
				if (p2->right == 0)
					return 0;
				buffer_read_page(table_id_2, p2->right, p2);
				j = 0;
			}

		}
		else {
			if (p1->pack[i].key < p2->pack[j].key) {
				i++;
				if (p1->k_num == i) {
					if (p1->right == 0)
						return 0;
					buffer_read_page(table_id_1, p1->right, p1);
					i = 0;
				}

			}

		}


	}
}
