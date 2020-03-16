#include "bpt.h"
#include "file.h"
#include <stdlib.h>
#include<stdio.h>

// MAIN

int main(int argc, char ** argv) {

	char * input_file = "a.txt";
	page_t * root;
	int64_t input, range2;
	char instruction;
	char license_part;
	char * temp;
	temp = malloc(sizeof(char) * 120);
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
	open_table(input_file);
	// }
	temp_h = (header_page_t*)malloc(sizeof(header_page_t));
	file_read_page(0,temp_h);
	

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
	fflush(stdin);
	temp = (char*)malloc(120);
	while (scanf("%c", &instruction) != EOF) {
		switch (instruction) {
		case 'd':
			scanf("%ld", &input);
			db_delete(input);
			//print_tree();
			break;
		case 'i':
			strcpy(temp,"a");
			scanf("%d",&n2);
			for(int i=1;i<=n2;i++){	
			input = i;
			
			//scanf("%ld", &input);
			//scanf("%s", temp);
			db_insert(input, temp);
			}
			//print_tree();
			break;
		case 'f':
			scanf("%ld", &input);
			db_find(input, temp);
			break;
		case 'q':
			while (getchar() != (int)'\n');
			return 0;
		case 'p':
			print_tree();
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
	file_write_page(0,temp_h);
	return EXIT_SUCCESS;
}
