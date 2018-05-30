#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Header.h"
#include "Header1.h"
#include "Header2.h"

int get_opcode_number(char* opcode);

int stk[1000];
int top = -1;

void push(int val)
{
	top++;
	stk[top] = val;
}

int pop()
{
	int v = stk[top];
	top--;
	return v;
}

int get_top()
{
	return stk[top];
}



void add_label(char* name,int address)
{
	label_array[num_labels]->name = (char*)malloc(sizeof(char)*(strlen(name) + 1));
	int i = 0;
	for (i = 0; name[i] != ':'; i++)
	{
		label_array[num_labels]->name[i] = name[i];
	}
	label_array[num_labels]->name[i] = '\0';
	label_array[num_labels]->address = address;
	num_labels++;
}


bool number_present(char* name)
{
	if (name[2] >= 48 && name[2] <= 57)
	{
		return true;
	}
	else
		return false;
}

struct symbol* search_symbol_table(char symbol)
{
	for (int i = 0; i < num_symbols; i++)
	{
		if (symbol_array[i]->c == symbol)
			return symbol_array[i];
	}
	return NULL;
}


int is_register_name(char* name)
{
	for (int i = 0; i < 8; i++)
	{
		if (strcmp(name, registernames[i]) == 0)
			return i;
	}
	return -1;
}


struct label* find_label(char* name)
{
	for (int i = 0; i < num_labels; i++)
	{
		if (strcmp(label_array[i]->name, name) == 0)
			return label_array[i];
	}
	return NULL;
}


int is_label(char* name)
{
	for (int i = 0; name[i] != '\0'; i++)
	{
		if (name[i] == ':')
			return 1;
	}
	return -1;
}


void read_instructions(char** lines,int num_lines)
{
	int i = 0;
	while (strcmp(lines[i], "START:")!=0)
	{
		i++;
	}

	add_label(lines[i], 1);

	int line = 1;
	if (i != 0)
	{
		i++;
		for (int j = i; j < num_lines; j++)
		{
			char temp[1024];
			strcpy(temp, lines[j]);
			char* token = strtok(temp, " ,");
			if (is_label(token) == 1)
			{
				add_label(token, num_intermediate_rows + 1);
			}
			int opcode_num = get_opcode_number(token);
			if (opcode_num == 1)
			{
					token = strtok(NULL, " ,");
					int reg_num = is_register_name(token);
					if (reg_num>= 0)
					{
						int index_array = 0;
						token = strtok(NULL, " ,");
						struct symbol* symb =search_symbol_table(token[0]);
						if (number_present(token))
						{
							index_array = atoi(&token[2]);
						}
						intermediate_array[num_intermediate_rows]->line_num = num_intermediate_rows + 1;
						intermediate_array[num_intermediate_rows]->opcode = 2;
						if (symb->flag == 1)
						{
							intermediate_array[num_intermediate_rows]->parameters[1] = get_symbol_position(symb->c);
							intermediate_array[num_intermediate_rows]->parameters[0] = reg_num;
							registers[intermediate_array[num_intermediate_rows]->parameters[0]] = (char)symb->offset;

						}
						else
						{
							intermediate_array[num_intermediate_rows]->parameters[1] = symb->offset + index_array;
							intermediate_array[num_intermediate_rows]->parameters[0] = reg_num;
//							printf("\nmoving value from memory %c to register %s\n", symb->c, registernames[reg_num]);
							registers[intermediate_array[num_intermediate_rows]->parameters[0]] = buffer[intermediate_array[num_intermediate_rows]->parameters[1]];
						}
						num_intermediate_rows++;
					}
					else
					{
						int index_array = 0;
						//token = strtok(NULL, " ,");
						struct symbol* symb = search_symbol_table(token[0]);
						if (number_present(token) == true)
						{
							index_array = atoi(&token[2]);
						}
						token = strtok(NULL, " ,");
						int reg_num=is_register_name(token);
						intermediate_array[num_intermediate_rows]->line_num = num_intermediate_rows + 1;
						intermediate_array[num_intermediate_rows]->opcode = 1;
						intermediate_array[num_intermediate_rows]->parameters[0] = symb->offset+index_array;
						intermediate_array[num_intermediate_rows]->parameters[1] = reg_num;
//						printf("\nmoving value from register %s to memory %c\n", registernames[reg_num], symb->c);
						buffer[intermediate_array[num_intermediate_rows]->parameters[0]] = registers[intermediate_array[num_intermediate_rows]->parameters[1]];
						num_intermediate_rows++;
					}
			}
			else if (opcode_num == 3)
			{
				int param1 = -1;
				int param2 = -1;
				int param3 = -1;
				token = strtok(NULL, " ,");
				param1 = is_register_name(token);
				token = strtok(NULL, " ,");
				param2 = is_register_name(token);
				token = strtok(NULL, " ,");
				param3 = is_register_name(token);
				intermediate_array[num_intermediate_rows]->line_num = num_intermediate_rows + 1;
				intermediate_array[num_intermediate_rows]->opcode = opcode_num;
				intermediate_array[num_intermediate_rows]->parameters[0] = param1;
				intermediate_array[num_intermediate_rows]->parameters[1] = param2;
				intermediate_array[num_intermediate_rows]->parameters[2] = param3;
				registers[param1] = registers[param2] + registers[param3];
//				printf("\naddition of registers %s and %s is %d which is stored in %s\n", registernames[param2], registernames[param3], registers[param1], registernames[param1]);
				num_intermediate_rows = num_intermediate_rows + 1;
			}
			else if (opcode_num == 4)
			{
				int param1 = -1;
				int param2 = -1;
				int param3 = -1;
				token = strtok(NULL, " ,");
				param1 = is_register_name(token);
				token = strtok(NULL, " ,");
				param2 = is_register_name(token);
				token = strtok(NULL, " ,");
				param3 = is_register_name(token);
				intermediate_array[num_intermediate_rows]->line_num = num_intermediate_rows + 1;
				intermediate_array[num_intermediate_rows]->opcode = opcode_num;
				intermediate_array[num_intermediate_rows]->parameters[0] = param1;
				intermediate_array[num_intermediate_rows]->parameters[1] = param2;
				intermediate_array[num_intermediate_rows]->parameters[2] = param3;
				registers[param1] = registers[param2] - registers[param3];
//				printf("\nsubtraction of registers %s and %s is %d which is stored in %s\n", registernames[param2], registernames[param3], registers[param1], registernames[param1]);
				num_intermediate_rows = num_intermediate_rows + 1;
			}
			else if (opcode_num == 5)
			{
				int param1 = -1;
				int param2 = -1;
				int param3 = -1;
				token = strtok(NULL, " ,");
				param1 = is_register_name(token);
				token = strtok(NULL, " ,");
				param2 = is_register_name(token);
				token = strtok(NULL, " ,");
				param3 = is_register_name(token);
				intermediate_array[num_intermediate_rows]->line_num = num_intermediate_rows + 1;
				intermediate_array[num_intermediate_rows]->opcode = opcode_num;
				intermediate_array[num_intermediate_rows]->parameters[0] = param1;
				intermediate_array[num_intermediate_rows]->parameters[1] = param2;
				intermediate_array[num_intermediate_rows]->parameters[2] = param3;
				registers[param1] = registers[param2] * registers[param3];
//				printf("\nmultiplication of registers %s and %s is %d which is stored in %s\n", registernames[param2], registernames[param3], registers[param1], registernames[param1]);
				num_intermediate_rows = num_intermediate_rows + 1;
			}
			else if (opcode_num == 6)
			{
				token = strtok(NULL, " ,");
				struct label* lbl=find_label(token);
				intermediate_array[num_intermediate_rows]->line_num = num_intermediate_rows + 1;
				intermediate_array[num_intermediate_rows]->opcode = opcode_num;
				intermediate_array[num_intermediate_rows]->parameters[0] = lbl->address;
				num_intermediate_rows = num_intermediate_rows + 1;
			}
			else if (opcode_num == 7)
			{
				token = strtok(NULL, " ,");
				int reg1=is_register_name(token);
				token = strtok(NULL, " ,");
				int op1 = get_opcode_number(token);
				token = strtok(NULL, " ,");
				int reg2 = is_register_name(token);
				token = strtok(NULL, " ,");
				intermediate_array[num_intermediate_rows]->line_num = num_intermediate_rows + 1;
				intermediate_array[num_intermediate_rows]->opcode = opcode_num;
				intermediate_array[num_intermediate_rows]->parameters[0] = reg1;
				intermediate_array[num_intermediate_rows]->parameters[1] = reg2;
				intermediate_array[num_intermediate_rows]->parameters[2] = op1;
				intermediate_array[num_intermediate_rows]->parameters[3] = -1;
				push(intermediate_array[num_intermediate_rows]->line_num);
				num_intermediate_rows = num_intermediate_rows + 1;
			}
			else if (opcode_num == 14)//read
			{
				token = strtok(NULL, " ");
				int reg_num = is_register_name(token);
				intermediate_array[num_intermediate_rows]->line_num = num_intermediate_rows+1;
				intermediate_array[num_intermediate_rows]->opcode = opcode_num;
				intermediate_array[num_intermediate_rows]->parameters[0] = reg_num;
//				printf("enter value: ");
//				scanf("%d", &registers[reg_num]);
				num_intermediate_rows++;
			}
			else if (opcode_num == 13)//print
			{
				token = strtok(NULL, " [");
				int reg_num = is_register_name(token);
				if (reg_num != -1)
				{
//					printf("value of %s is %d\n", registernames[reg_num], registers[reg_num]);
					intermediate_array[num_intermediate_rows]->line_num = num_intermediate_rows + 1;
					intermediate_array[num_intermediate_rows]->opcode = opcode_num;
					intermediate_array[num_intermediate_rows]->parameters[0] = reg_num;
				}
				else
				{
					char c = token[0];
					token = strtok(NULL, " [");

					if (token != NULL)
					{
						int val = atoi(token);
						int offset = get_symbol_position(c);
						struct symbol* symb = get_symbol(offset);
						intermediate_array[num_intermediate_rows]->line_num = num_intermediate_rows + 1;
						intermediate_array[num_intermediate_rows]->opcode = opcode_num;
						intermediate_array[num_intermediate_rows]->parameters[0] = 8 + symb->offset + val;
						intermediate_array[num_intermediate_rows]->parameters[1] = -2;
					}
					else
					{
						//int val = atoi(token);
						int offset = get_symbol_position(c);
						struct symbol* symb = get_symbol(offset);
						intermediate_array[num_intermediate_rows]->line_num = num_intermediate_rows + 1;
						intermediate_array[num_intermediate_rows]->opcode = opcode_num;
						intermediate_array[num_intermediate_rows]->parameters[0] = 8 + offset;
						intermediate_array[num_intermediate_rows]->parameters[1] = 1;
					}
					
				}
				num_intermediate_rows++;
			}
			else if (opcode_num == 16)
			{
				intermediate_array[num_intermediate_rows]->line_num = num_intermediate_rows + 1;
				intermediate_array[num_intermediate_rows]->opcode = 6;
				intermediate_array[num_intermediate_rows]->parameters[0] = -1;
				int line = get_top();
				pop();
				push(intermediate_array[num_intermediate_rows]->line_num);
				intermediate_array[line - 1]->parameters[3] = num_intermediate_rows + 1+1;
				num_intermediate_rows++;
			}
			else if (opcode_num == 15)
			{
				int line = get_top();
				pop();
				intermediate_array[num_intermediate_rows]->line_num = num_intermediate_rows + 1;
				intermediate_array[num_intermediate_rows]->opcode = 0;
				/*if (intermediate_array[line-1]->opcode==7)
					intermediate_array[line - 1]->parameters[3] = num_intermediate_rows + 1 + 1;
				else*/
					intermediate_array[line - 1]->parameters[0] = intermediate_array[num_intermediate_rows]->line_num+1;//
				num_intermediate_rows++;
			}
		}

	}
}

char* next_token(char* str, int* p)
{
	char* token = NULL;
	int len = 0;
	while (str[*p] == ' ' || str[*p] == ','&& str[*p] != '\n')
	{
		*p = *p + 1;
	}
	while (str[*p + len] != '\0'&&str[*p + len] != ','&&str[*p+len]!=' '&&str[*p+len]!='='&&str[*p + len] != '\n')
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


int get_opcode_number(char* opcode)
{
	for (int i = 0; i < 16; i++)
	{
		if (strcmp(opcodes[i], opcode) == 0)
		{
			return i + 1;
		}
	}
	return -1;
}


int read_data(char** lines, int num_lines)
{
	char temp_buff[100];
	int p = 0;
	char* token = NULL;
	int i = 0;
	int opcode_num = -1;
	int offset = 0;
	for (i = 0; i < num_lines &&strcmp(lines[i],"START:")!=0; i++)
	{
		p = 0;
		char temp[100];
		strcpy(temp, lines[i]);
		
		token = strtok(temp, " ,");
		while (token != NULL)
		{
			if (strcmp(token, "DATA") == 0)
			{
				token = strtok(NULL, " ,[");
				//read_data(token,symbol_array,num_symbols);
				int j = 0;
				symbol_array[num_symbols]->c = token[0];
				//symbol_array[num_symbols]->c = token[0];
				token = strtok(NULL, " ,[");
				int size = 0;
				if (token != NULL)
				{
					size = atoi(token);
					if (size == 0)
					{
						symbol_array[num_symbols]->size = 1;
						symbol_array[num_symbols]->offset = offset;
						symbol_array[num_symbols]->flag = 0;
						offset = offset + 1;
					}
					else{
						symbol_array[num_symbols]->size = size;
						symbol_array[num_symbols]->offset = offset;
						symbol_array[num_symbols]->flag = 0;
						offset = offset + size;
					}
					num_symbols++;
				}
				else
				{
					symbol_array[num_symbols]->size = 1;
					symbol_array[num_symbols]->offset = offset;
					offset = offset + 1;
					symbol_array[num_symbols]->flag = 0;
					num_symbols++;
				}
			}
				else if (strcmp(token, "CONST") == 0)
				{
					token = strtok(NULL, " ,=");
					//read_data(token,symbol_array,num_symbols);
					int j = 0;
					symbol_array[num_symbols]->c = token[0];
					token = strtok(NULL, " ,=");
					symbol_array[num_symbols]->offset = atoi(token);
					symbol_array[num_symbols]->flag = 1;
					num_symbols++;
				}
				token = strtok(NULL, " ,");
			}
	}
	return 1;
}


void strip_spaces(char* str)
{
	int prefix = 0;
	int len = strlen(str) - 1;
	int suffix = len;
	if (str[len] == '\n')
		suffix = len - 1;

	while (str[prefix] == ' ' || str[prefix]=='\t'|| str[prefix] == '\n'&&prefix <= len)
	{
		prefix = prefix + 1;
	}
	while (str[suffix] == ' ' || str[suffix] == '\n'&&suffix >= 0)
	{
		suffix = suffix - 1;
	}

	int i = 0;
	while (prefix <= suffix)
	{
		str[i] = str[prefix];
		prefix++;
		i++;
	}
	str[i] = '\0';
	str[suffix + 1] = '\0';
}

int read_file(FILE* f,char** lines)
{
	char temp_buff[1024];
	buffer = (char*)malloc(500 * sizeof(char));
	int num_lines = 0;
	while (!feof(f))
	{
		temp_buff[0] = '\0';
		fgets(temp_buff, 1024, f);
		strip_spaces(temp_buff);
		if (temp_buff[0] != '\0')
		{
			lines[num_lines] = (char*)malloc(sizeof(char)*strlen(temp_buff) + 1);
			strcpy(lines[num_lines], temp_buff);
			num_lines++;
		}
	}
	return num_lines;
}

int main()
{
	FILE* f = fopen("abc3.txt","r");
	char** lines = (char**)malloc(100*sizeof(char*));
	symbol_array = (struct symbol**)malloc(20*sizeof(struct symbol*));
	constsymbol_array = (struct constsymbol**)malloc(20 * sizeof(struct constsymbol*));
	intermediate_array = (struct intermediate**)malloc(20 * sizeof(struct intermediate*));
	label_array = (struct label**)malloc(20 * sizeof(struct label*));

	for (int i = 0; i < 20; i++)
	{
		symbol_array[i] = (struct symbol*)malloc(1 * sizeof(struct symbol));
	}
	for (int i = 0; i < 20; i++)
	{
		constsymbol_array[i] = (struct constsymbol*)malloc(1 * sizeof(struct constsymbol));
	}

	for (int i = 0; i < 20; i++)
	{
		intermediate_array[i] = (struct intermediate*)malloc(sizeof(struct intermediate));
	}

	for (int i = 0; i < 20; i++)
	{
		label_array[i] = (struct label*)malloc(1 * sizeof(struct label));
	}
	int end_position = 0;
	
	/*for (int i = 0; i < 100; i++)
	{
		lines[i] = (char*)malloc(100 * sizeof(char));
	}*/
	int num_lines=read_file(f,lines);
	read_data(lines, num_lines);
	int i = 0;

	read_instructions(lines,num_lines);

	while (1)
	{
		printf("\n1. display symbol table\n");
		printf("\n2. display const table\n");// removed this feature.
		printf("\n3. display intermediate table\n");
		printf("\n4. display label table\n");
		printf("\n5. evaluate intermediate table\n");
		printf("\n6. save to file\n");
		printf("\n7. read from file\n");
		printf("\n8. evaluate\n");
		scanf("%d", &i);
		if (i == 1)
			display_symbol_table();
		else if (i == 2)
			display_const_table();
		else if (i == 3)
			display_intermediate_table();
		else if (i == 4)
			display_label_table();
		else if (i == 5)
		{
			while (top != -1)
				pop();
			evaluate_intermediate_table();
		}
		else if (i == 6)
		{
			char name[100];
			printf("\nenter file name: ");
			scanf("%s", name);
			write_to_file(name);
		}
		else if (i == 7)
		{
			char name[100];
			printf("\n enter file name\n");
			scanf("%s", name);
			read_from_file(name);
		}
		else if (i == 8)
		{
			char name[100];
			printf("\n enter name: ");
			scanf("%s", name);
			fclose(f);
			f = fopen(name, "r");
			num_intermediate_rows = 0;
			num_symbols = 0;
			num_constsymbols = 0;
			num_labels = 0;
			num_lines = read_file(f, lines);
			read_data(lines, num_lines);
			int i = 0;
			read_instructions(lines, num_lines);
		}
	}
}