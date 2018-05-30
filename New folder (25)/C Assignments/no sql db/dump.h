#include<stdio.h>
#include<stdlib.h>

void dump_to_file(struct table* table)
{
	struct row_node* node = table->head;
	while (node != NULL)
	{
		struct col_node* col = node->col_head;
		while (col != NULL)
		{
			struct col_node* temp_col = col;
			char filename[100];
			sprintf(filename, "%s_%s.txt", table->table_name, col->name);
			FILE* fp = fopen(filename, "a");
			while (temp_col != NULL)
			{
				if (col->type == 0)
					fprintf(fp, "%d,%d,%d,%s,%d,%d,%d\n", node->id, node->commit_num, node->current_num, col->name, col->type, col->version, *(int*)col->value);
				else if (col->type == 1)
					fprintf(fp, "%d,%d,%d,%s,%d,%d,%s\n", node->id, node->commit_num, node->current_num, col->name, col->type, col->version, (char*)col->value);
				temp_col = temp_col->next_version;
			}
			fclose(fp);
		}
		node = node->next_row;
	}
}

void dump_to_file_update(struct table* tab, struct row_node* node)
{
	//struct row_node* node = tab->head;
		struct col_node* col = node->col_head;
		while (col != NULL)
		{
			char filename[100];
			sprintf(filename, "%s_%s.txt", tab->table_name,col->name);
			FILE* fp = fopen(filename, "a");
			if (col->type == 0)
				fprintf(fp, "%d,%d,%s,%d,%d,%d\n", node->id,node->current_num,node->commit_num,col->name, col->version,col->type, *(int*)col->value);
			else if (col->type == 1)
				fprintf(fp, "%d,%d,%s,%d,%d,%s\n", node->id,node->current_num,node->commit_num, col->name, col->version,col->type, (char*)col->value);
			col = col->next;
			fclose(fp);
		}
}

/*
void dump_table_schema(struct table* tab)
{
	char filename[1000];
	sprintf(filename, "table_%s.txt", tab->table_name);
	FILE* f = fopen(filename, "w");
	fprintf(f, "%s\n", tab->table_name);

	fprintf(f, "%d\n", tab->num_cols);
	int i = 0;
	for (i = 0; i < tab->num_cols-1; i++)
	{
		fprintf(f, "%s,", tab->col_names[i]);
	}
	fprintf(f, "%s\n", tab->col_names[i]);
	for (i = 0; i < tab->num_types-1; i++)
	{
		fprintf(f, "%d,", tab->types[i]);
	}
	fprintf(f, "%d\n", tab->types[i]);
	fclose(f);
}*/