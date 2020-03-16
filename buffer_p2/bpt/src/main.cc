#include <stdlib.h>
#include<stdio.h>
#include "buffer.h"
#include<cstring>
// MAIN

int main(int argc, char ** argv) {

	const char * input_file = "a.txt";
	page_t * root;
	int64_t input, range2;
	char instruction;
	char license_part;
	char * temp;
	temp = (char *)malloc(sizeof(char) * 120);
	root = NULL;
	//verbose_output = false;
	int n1;
	// if (argc > 1) {
	//input_file = (argv[1]);
	/*if (order < MIN_ORDER || order > MAX_ORDER) {
	fprintf(stderr, "Invalid order: %d .\n\n", order);
	usage_3();
	exit(EXIT_FAILURE);
	}*/
	
	// }
	

	/*if (argc > 2) {
	input_file = argv[2];

	if (fd == NULL) {
	perror("Failure  open input file.");
	exit(EXIT_FAILURE);
	}
	while (!feof(fp)) {
	fscanf(fp, "%d\n", &input);
	root = insert(root, input, input);
	}
	fclose(fp);
	print_tree(root);
	}*/
	int n2;
	printf("> ");
	int tttt;
	fflush(stdin);
	int i_table_id;
	temp = (char*)malloc(120);
	printf("push o to open table\n");
	while (scanf("%c", &instruction) != EOF) {	
		switch (instruction) {
		
		
		case 'o':
			open_table(input_file);
			printf("put number to select buffer size\n");
			scanf("%d", &tttt);
			init_db(tttt);
			break;
		case 'd':
			printf("push table_id and delete\n");
			scanf("%d", &i_table_id);
			scanf("%ld", &input);
			db_delete(i_table_id,input);
			//print_tree();
			break;
		case 'i':
			printf("push table_id and key\n");
			strcpy(temp,"a");
			scanf("%d", &i_table_id);
			scanf("%d",&n2);
			
			for(int i=1;i<=n2;i++){	
			input = i;
			
			db_insert(i_table_id,input, temp);
			}
			//print_tree();
			break;
		/*case 'f':
			scanf("%ld", &input);
			db_find(input, temp);
			break;*/
		case 'q':
			while (getchar() != (int)'\n');
			shutdown_db();
			return 0;
		case 'p':
			printf("push table_id");
			scanf("%d",&i_table_id);			
			print_tree(i_table_id);
			break;
			/* case 'r':
			scanf("%d %d", &input, &range2);
			if (input > range2) {
			int tmp = range2;
			range2 = input;
			input = tmp;
			}
			find_and_print_range(root, input, range2, instruction == 'p');
			break;
			case 'l':
			print_leaves(root);
			break;
			case 'q':
			while (getchar() != (int)'\n');
			return EXIT_SUCCESS;
			break;
			case 't':
			print_tree(root);
			break;
			case 'v':
			verbose_output = !verbose_output;
			break;
			case 'x':
			if (root)
			root = destroy_tree(root);
			print_tree(root);
			break;*/
		default:
			usage_2();
			fflush(stdin);
			break;
		}
		while (getchar() != (int)'\n');
		printf("> ");
	}
	printf("\n");
	
	return EXIT_SUCCESS;
}
