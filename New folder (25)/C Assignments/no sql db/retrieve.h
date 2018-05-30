#include<stdio.h>

char* next_token(char* str, int* p)
{
	char* token = NULL;
	int len = 0;
	while (str[*p] == ' ' || str[*p] == ','&& str[*p] != '\n')
	{
		*p = *p + 1;
	}
	while (str[*p + len] != '\0'&&str[*p + len] != ','&&str[*p + len] != '\n')
	{
		len = len + 1;
	}
	token = (char*)malloc((len + 1)*sizeof(char));
	for (int i = 0; i < len; i++)
	{
		token[i] = str[*p + i];
	}
	token[len] = '\0';
	*p = *p + len;
	return token;
}


/*
struct table* read_table_schema(char* filename)
{
	struct table* h = (struct table*)malloc(sizeof(struct table));
	FILE *f = fopen(filename, "r");
	char line[1024];
	line[0] = '\0';
	fgets(line, 1024, f);
	h->table_name = (char*)malloc((strlen(line) + 1)*sizeof(char));
	strcpy(h->table_name, line);
	fgets(line, 1024, f);
	h->num_cols = atoi(line);
	fgets(line, 1024, f);
	
	h->col_names = (char**)malloc(sizeof(char*));
	int p = 0;
	char* token = next_token(line, &p);
	while (token != NULL&& token != '\0')
	{
		for (int i = 0; i < h->num_cols; i++)
		{
			h->col_names[i] = (char*)malloc((strlen(token) + 1)*sizeof(char));
			strcpy(h->col_names[i], token);
		}
	}
}
*/
/*
struct row_node* create_partial_row(char* line,char* name)
{
	  
}
*/