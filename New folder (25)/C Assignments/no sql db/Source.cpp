#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "structs.h"
#include "dump.h"
#include "retrieve.h"

//struct row_node* head = NULL;

//int num_rows = 0;



struct row_node* create_new_row_node()
{
	struct row_node* node = (struct row_node*)malloc(sizeof(struct row_node));
	node->current_num = 0;
	node->commit_num = 0;
	node->next_row = NULL;
	node->col_head = NULL;
	return node;
}

struct col_node* create_new_col_node(char* name,void* value,int type)// value is already allocated
{
	struct col_node* node = (struct col_node*)malloc(sizeof(struct col_node));
	node->name = (char*)malloc((strlen(name) + 1)*sizeof(char));
	strcpy(node->name, name);
	node->next = NULL;
	node->value = value;
//	strcpy(node->value, value);
	node->version = 1;
	node->next_version = NULL;
	node->type = type;
	return node;
}

/*
struct row_node* create_copy_row(struct node* row)
{
	struct row_node* node = (struct row_node*)malloc(sizeof())
}*/


void insert_col(struct row_node* row_node,struct col_node* col_node)
{
	if (row_node->col_head == NULL)
	{
		row_node->col_head = col_node;
		col_node->version = 1;
		row_node->current_num = 1;
	}
	else
	{
		struct col_node* t = row_node->col_head;
		struct col_node* prev = t;
		while (t!=NULL&&t->next!= NULL&&strcmp(t->name,col_node->name)!=0)
		{
			prev = t;
			t = t->next;
		}
		if (strcmp(t->name, col_node->name) == 0)
		{
			if (t == row_node->col_head)
			{
				row_node->col_head=col_node;
				col_node->next = prev->next;
				prev->next = NULL;
				col_node->next_version = prev;
				row_node->col_head = col_node;
				col_node->version = row_node->current_num+1;
			}
			else
			{
				prev->next = col_node;
				col_node->next_version = t;
				col_node->next = t->next;
				t->next = NULL;
				col_node->version = row_node->current_num + 1;
			}
			/*while (t->next_version != NULL)
			{
				t = t->next_version;
			}
			t->next_version = col_node;
			*/
			//col_node->version = t->version + 1;
		}
		else if (t->next==NULL)
		{
			t->next = col_node;
			col_node->version = row_node->current_num+1;
		}
		//row_node->current_num = row_node->current_num + 1;
	}
}

struct row_node* get(struct table* tab,int id)// returns record
{
	struct row_node* t = tab->head;
	if (t == NULL)
	{
		return NULL;
	}
	int i = 0;
	while (t!=NULL&&t->next_row != NULL&&t->id != id)
	{
		t = t->next_row;
	}
	if (t->id == id)
	{
		return t;
	}
	return NULL;
}

void put(struct table* tab,int id, struct row_node* node,int version) //does update and insert
{
	struct row_node* t = get(tab,id);
	//dump_to_file_update(tab, node);
	//t->current_num = t->current_num + 1;
	if (t != NULL)
	{
		struct col_node* ct = node->col_head;
		struct col_node* temp=node->col_head;
		struct col_node* next_temp = temp;
		int enter = 0;
		while (temp!=NULL&&next_temp != NULL)
		{
			enter = 1;
			next_temp = temp->next;
			temp->next = NULL;
			temp->version = t->current_num + 1;
			insert_col(t, temp);
			//t->commit_num = t->commit_num + 1;
			//t->current_num = t->current_num + 1;
			temp = next_temp;
			/*
			insert_col(t, temp);
			temp = next_temp;
			next_temp = next_temp->next;
			t->commit_num = t->commit_num + 1;
			t->current_num = t->current_num + 1;*/
			/*
			temp = ct->next;
			insert_col(t, ct);
			t->current_num = t->current_num + 1;
			t->commit_num = t->commit_num + 1;
			ct = temp;
			ct->next = NULL;
			*/
		}
		if (enter == 1)
		{
			t->commit_num = t->commit_num + 1;
			t->current_num = t->current_num + 1;
		}
	}
	else
	{
		if (tab->head == NULL)
		{
			tab->head = node;
			node->commit_num = 1;
			node->current_num = 1;///removed return here
		}
		else
		{
			struct row_node* temp = tab->head;
			while (temp->next_row != NULL)
			{
				temp = temp->next_row;
			}
			temp->next_row = node;
			node->commit_num = 1;
			node->current_num = 1;
			//insert_col(t, node->col_head);
		}
		/*t->next_row = node;
		node->commit_num = 1;
		node->current_num = 1;*/
	}
}

void add_new_row(struct table* tab, struct row_node* row)
{
	struct row_node* t = tab->head;
	if (t == NULL)
	{
		tab->head = row;
	}
	else
	{
		while (t->next_row != NULL)
		{
			t = t->next_row;
		}
		t->next_row = row;
	}
}

void delete_row(struct row_node* row_node)
{
	if (row_node == NULL)
		return;
	struct col_node * col_head = row_node->col_head;
	while (col_head != NULL)
	{
		struct col_node* col = col_head;
		col_head = col_head->next;
		while (col != NULL)
		{
			struct col_node* temp = col;
			col = col->next_version;
			free(temp->name);
			free(temp->value);
			free(temp);
		}
	}
	///free(row_node);
}

void Delete(struct table* tab,int id)
{
	int i = 0;
	struct row_node* t = tab->head;

	if (t == NULL)
	{
		return;
	}
	else if (t->id == id)
	{
		struct row_node* node = tab->head;
		tab->head = tab->head->next_row;
		delete_row(node);
		//free(t);
	}
	else
	{
		struct row_node* prev = tab->head;
		while (t!= NULL&&t->id!=id)
		{
			prev = t;
			t = t->next_row;
			i++;
		}
		if (t->id == id)
		{
			prev->next_row = t->next_row;
			t->next_row = NULL;
			delete_row(t->next_row);
		}
		/*else if (t== NULL)
		{
			struct row_node* row = t->next_row;
			t->next_row = t->next_row->next_row;
			free(row);
		}
		*/
	}
}


/*
void update_row(struct table* tab, int row_num, struct row_node* partial_row)
{
	int i = 0;
	struct row_node* t = tab->head;
	while (t->next_row != NULL&&i<row_num)
	{
		t = t->next_row;
		i++;
	}
	if (t != NULL)
	{
		struct col_node* col = partial_row->col_head;
		struct col_node* t=
		while (col->next != NULL)
		{
			if (strcmp)
		}
	}
}*/

void print_row_node(struct table* tab,struct row_node* node)
{
	if (tab != NULL&&node != NULL)
	{
		struct col_node* col = node->col_head;
		while (col != NULL)
		{
			struct col_node* col_version = col->next_version;
			int nextbool = 0;
			if (col->type == 0)
				printf("\n%s %d %d\n", col->name, *(int*)(col->value), col->version);
			else if (col->type == 1)
				printf("\n%s %s %d\n", col->name, (char*)col->value, col->version);
			while (col_version != NULL)
			{
				nextbool = 1;
				if (col_version->type == 0)
				{
					printf("\n%s %d %d\n", col_version->name, *(int*)col_version->value, col_version->version);
				}
				else if (col_version->type == 1)
				{
					printf("\n%s %s %d\n", col_version->name, (char*)col_version->value, col_version->version);
				}
				col_version = col_version->next_version;
			}
			col = col->next;
		}
	}
	/*
	if (tab != NULL&& node != NULL)
	{
		for (int i = 0; i < tab->num_cols; i++)
		{
			struct col_node* col = node->col_head;
			while (col != NULL)
			{
				struct col_node* col_version = col;
				while (col_version->next_version != NULL)
				{
					printf("%s %s %d\n", col_version->name, col_version->value, col_version);
				}
				printf("\n\n");
				col = col->next;
			}
			node = node->next_row;
		}
	}
	*/
}

void print_table(struct table* tab)
{
	struct row_node* node = tab->head;
	while (node!= NULL)
	{
		struct col_node* col = node->col_head;
		while (col != NULL)
		{
			struct col_node* col_version = col->next_version;
			int nextbool = 0;
			if (col->type == 0)
				printf("\n%s %d %d\n", col->name, *(int*)(col->value), col->version);
			else if (col->type == 1)
				printf("\n%s %s %d\n", col->name, (char*)col->value, col->version);
			while (col_version!= NULL)
			{
				nextbool = 1;
				if (col_version->type == 0)
				{
					printf("\n%s %d %d\n", col_version->name, *(int*)col_version->value, col_version->version);
				}
				else if (col_version->type == 1)
				{
					printf("\n%s %s %d\n", col_version->name, (char*)col_version->value, col_version->version);
				}
				col_version = col_version->next_version;
			}
			col = col->next;
		}
		node = node->next_row;
	}
}

struct row_node* create_new_node_with_cols(struct table* tab,int id)
{
	struct row_node* row = create_new_row_node();
	row->id = id;
	struct col_node* col_head = NULL;
	struct col_node* end = NULL;
	for (int i = 0; i < tab->num_cols; i++)
	{
		void* col_value = NULL;
		printf("enter value for %s", tab->col_names[i]);
		if (tab->types[i] == 0)
		{
			col_value = malloc(sizeof(int));
			scanf("%d", (int*)col_value);
		}
		else if (tab->types[i] == 1)
		{
			col_value = malloc(30 * sizeof(char*));
			scanf("%s", (char*)col_value);
		}
		if (col_head == NULL)
		{
			col_head = create_new_col_node(tab->col_names[i], col_value,tab->types[i]);
			end = col_head;
		}
		else
		{
			struct col_node* new_node = create_new_col_node(tab->col_names[i], col_value,tab->types[i]);
			end->next = new_node;
			end = end->next;
		}
	}
	row->col_head=col_head;
	row->commit_num = 1;
	row->current_num = 1;
	return row;
}

struct table* create_table(struct table* tab)
{
	char table_name[1024];
	tab= (struct table*)malloc(sizeof(struct table));
	tab->head = NULL;
	printf("\n enter table name\n");
	scanf("%s", table_name);
	tab->table_name = (char*)malloc((strlen(table_name) + 1)*sizeof(char));
	strcpy(tab->table_name, table_name);
	/*
	int num_columns;
	printf("\nenter number of columns\n");
	scanf("%d", &num_columns);
	int num_types = 0;
	printf("\n enter number of types\n");
	scanf("%d", &num_types);
	tab->num_types = num_types;
	tab->types = (int*)malloc(num_types*sizeof(int));
	printf("\n enter each column types<0,1>\n");
	for (int i=0; i < num_types; i++)
	{
		printf("\nfor column %d\n",i+1);
		scanf("%d",&tab->types[i]);
	}

	tab->num_cols = num_columns;
	tab->col_names = (char**)malloc(num_columns*sizeof(num_columns));
	for (int i = 0; i < num_columns; i++)
	{
		int temp_type = -1;
		char temp[100];
		printf("\n enter column name:\n");
		scanf("%s", temp);
		tab->col_names[i] = (char*)malloc((strlen(temp) + 1)*sizeof(char));
		strcpy(tab->col_names[i], temp);
	}
	*/
	tab->next = NULL;
	return tab;
}

struct table* create_multiple_table(struct table* tab,int num)
{
	struct table* temp = NULL;
	for (int i = 0; i < num; i++)
	{
		if (tab == NULL)
		{
			tab = create_table(tab);
			temp = tab;
		}
		else
		{
			temp->next = create_table(tab);
			temp = temp->next;
		}
	}
	return  tab;
}

struct table* get_table(struct table* h,char* name)
{
	struct table* t = h;
	while (t != NULL)
	{
		if (strcmp(t->table_name, name) == 0)
			return t;
		t = t->next;
	}
	return NULL;
}

struct row_node* create_partial_row()
{
	char name[1000];
	int type = 0;
	struct row_node* node = create_new_row_node();
	struct col_node* col = NULL;
	int num_cols;
	printf("\n enter num of cols\n");
	scanf("%d", &num_cols);
	for (int i = 0; i < num_cols; i++)
	{
		if (node->col_head == NULL)
		{
			node->col_head = (struct col_node*)malloc(sizeof(struct col_node));
			col= node->col_head;
			col->next_version = NULL;
			col->next = NULL;
			col->version = 1;
		}
		else
		{
			col->next = (struct col_node*)malloc(sizeof(struct col_node));
			col = col->next;
			col->next = NULL;
			col->next_version = NULL;
			col->version = 1;
		}
		printf("\n enter col name\n");
		scanf("%s", name);
		col->name = (char*)malloc((strlen(name) + 1)*sizeof(char));
		strcpy(col->name, name);
		printf("\n enter type 0 for int and 1 for string");
		scanf("%d", &type);
		col->type = type;
		printf("\nenter value\n");
		if (type == 0)
		{
			col->value = malloc(sizeof(int));
			scanf("%d", (int*)col->value);
		}
		else if (type == 1)
		{
			char value[1000];
			scanf("%s", value);
			col->value = malloc((strlen(value) + 1)*sizeof(char));
			strcpy((char*)col->value, value);
		}
	}
	return node;
}



int main()
{
	struct table* tab =NULL;
	int num_tables = 0;
	printf("\nenter num of tables\n");
	scanf("%d", &num_tables);
	tab = create_multiple_table(tab, num_tables);
	/*tab->head =
	printf("enter number of columns");
	int num_columns;
	scanf("%d", &tab->num_cols);
	char name[1024];
	tab->col_names = (char**)malloc(tab->num_cols*sizeof(char*));
	for (int i = 0; i < tab->num_cols; i++)
	{
		printf("\nenter column names\n");
		scanf("%s", &name);
		tab->col_names[i] = (char*)malloc((strlen(name) + 1)*sizeof(char));
		strcpy(tab->col_names[i], name);
	}*/
	struct table* current_table = tab;
	while (1)
	{
		printf("\ncurrent table %s",current_table->table_name);
		printf("\n1. get");
		printf("\n2. put");
		printf("\n3. print table");
		printf("\n4. delete");
		printf("\n5. choose table");
		int c;
		scanf("%d", &c);
		if (c == 1)
		{
			int id;
			printf("\nenter id\n");
			scanf("%d", &id);
			struct row_node* node = get(current_table,id);
			print_row_node(current_table,node);
			
			/*struct row_node* row = create_new_row_node();
			for (int i = 0; i < tab->num_cols; i++)
			{
				char value[1024];
				printf("\n enter col_name FOR\n");
				scanf("%s", name);
				printf("\n enter col value \n");
				scanf("%s", value);
				struct col_node* col = create_new_col_node(name, value);
				insert_col(row, col);
			}
			add_new_row(tab, row);*/
		}
		else if (c == 2)
		{
			int id;
			printf("\nenter id: ");
			scanf("%d", &id);
			struct row_node* row_node = create_partial_row();//create_new_node_with_cols(current_table,id);
			int version;
			printf("\nenter version\n");
			scanf("%d", &version);
			row_node->id = id;
			put(current_table, id, row_node,version);
		}
		else if (c == 3)
		{
			print_table(current_table);
		}
		else if (c == 4)
		{
			int id;
			printf("\nenter id\n");
			scanf("%d", &id);
			Delete(current_table, id);
		}
		else if (c == 5)
		{
			char table_name[100];
			scanf("%s", table_name);
			current_table = get_table(tab, table_name);
		}
		else if (c == 6)
		{
			//dump_table_schema(current_table);
		}
	}
}