struct symbol* get_symbol(int id)
{
	return symbol_array[id];
}

int get_symbol_position(char c)
{
	for (int i = 0; i < num_symbols; i++)
	{
		if (symbol_array[i]->c == c)
			return i;
	}
	return -1;
}

int evaluate_if_condition(char c1,char c2,int op)
{
	if (op == 8)
	{
		if (c1 == c2)
			return 1;
	}
	else if (op == 9)
	{
		if (c1 < c2)
			return 1;
	}
	else if (op == 10)
	{
		if (c1 > c2)
			return 1;
	}
	else if (op == 11)
	{
		if (c1 <= c2)
			return 1;
	}
	else if (op == 12)
	{
		if (c1 >= c2)
			return 1;
	}
	else
		return 0;
	return 0;
}

void evaluate_intermediate_table()
{
	int pc = 0;
	while (pc < num_intermediate_rows)
	{
		if (intermediate_array[pc]->opcode == 1)
		{
			buffer[intermediate_array[pc]->parameters[1]] = registers[intermediate_array[pc]->parameters[0]];
			pc++;
		}
		else if (intermediate_array[pc]->opcode == 2)
		{
			struct symbol * symb= get_symbol(intermediate_array[pc]->parameters[1]);
			if (symb->flag == 1)
			{
				registers[intermediate_array[pc]->parameters[1]] = (char)symb->offset;
				///printf("%d", symb->offset);
			}
			else
				registers[intermediate_array[pc]->parameters[1]] = buffer[intermediate_array[pc]->parameters[0]];
			pc++;
		}
		else if (intermediate_array[pc]->opcode == 3)
		{
			int param1 = intermediate_array[pc]->parameters[0];
			int param2 = intermediate_array[pc]->parameters[1];
			int param3 = intermediate_array[pc]->parameters[2];
			registers[param1] = registers[param2] + registers[param3];
			pc++;
		}
		else if (intermediate_array[pc]->opcode == 4)
		{
			int param1 = intermediate_array[pc]->parameters[0];
			int param2 = intermediate_array[pc]->parameters[1];
			int param3 = intermediate_array[pc]->parameters[2];
			registers[param1] = registers[param2] - registers[param3];
			pc++;
		}
		else if (intermediate_array[pc]->opcode == 5)
		{
			int param1 = intermediate_array[pc]->parameters[0];
			int param2 = intermediate_array[pc]->parameters[1];
			int param3 = intermediate_array[pc]->parameters[2];
			registers[param1] = registers[param2] * registers[param3];
			pc++;
		}
		else if (intermediate_array[pc]->opcode == 6)
		{
			pc = intermediate_array[pc]->parameters[0]-1;
		}
		else if (intermediate_array[pc]->opcode == 7)
		{
			int truth=evaluate_if_condition(registers[intermediate_array[pc]->parameters[0]], registers[intermediate_array[pc]->parameters[1]], intermediate_array[pc]->parameters[2]);
			if (truth == 1)
			{
				pc = pc + 1;
			}
			else
			{
				pc = intermediate_array[pc]->parameters[3] - 1;
			}
		}
		else if (intermediate_array[pc]->opcode ==14)
		{
			int reg_num=intermediate_array[pc]->parameters[0];
			printf("enter value for register %s: ",registernames[reg_num]);
			int v;
			scanf("%d", &v);
			registers[reg_num] = (char)v;
			pc++;
		}
		else if (intermediate_array[pc]->opcode == 13)
		{

			int reg_num = intermediate_array[pc]->parameters[0];
			if (reg_num < 8)
			{
				printf("value of %s is %d\n", registernames[reg_num], registers[reg_num]);
			}
			else
			{
				if (intermediate_array[pc]->parameters[1] == 1)
				{
					struct symbol* symb = get_symbol(intermediate_array[pc]->parameters[0] - 8);
					printf("%d \n", symb->offset);
				}
				else
				{
					struct symbol* symb = get_symbol(reg_num - 8);
					if (symb->flag != 1)
						printf("%d \n", buffer[reg_num - 8]);
					else
						printf("%d \n", symb->offset);
				}
			}
			pc++;
		}
		else if (intermediate_array[pc]->opcode == 15)
		{
			pc = pc + 1;
		}
		else
		{
			pc = pc + 1;
		}
	}
}



void write_to_file(char* name)
{
	FILE* f = fopen(name, "w");
	fprintf(f, "INTERMEDIATE\n");
	for (int i = 0; i < num_intermediate_rows; i++)
	{
		fprintf(f, "%d %d ", intermediate_array[i]->line_num, intermediate_array[i]->opcode);
		for (int j = 0; j < 4; j++)
		{
			fprintf(f, "%d ", intermediate_array[i]->parameters[j]);
		}
		fprintf(f, "\n");
	}
	fprintf(f, "LABEL\n");
	for (int i = 0; i < num_labels; i++)
	{
		fprintf(f, "%s %d\n", label_array[i]->name, label_array[i]->address);
	}
	fprintf(f, "SYMBOLTABLE\n");
	for (int i = 0; i < num_symbols; i++)
	{
		fprintf(f, "%c %d %d %d\n", symbol_array[i]->c, symbol_array[i]->flag, symbol_array[i]->offset, symbol_array[i]->size);
	}
	fclose(f);
}

void read_from_file(char* filename)
{
	FILE* f = fopen(filename, "r");
	char line[100];
	fgets(line, 100, f);
	num_intermediate_rows = 0;
	num_symbols = 0;
	num_labels = 0;
	char *token = NULL;

	while (!feof(f))
	{
		fgets(line,100,f);
		if (strcmp(line, "LABEL\n") == 0)
		{
			break;
		}
		else{
			token = strtok(line, " ");
			intermediate_array[num_intermediate_rows]->line_num = atoi(token);
			token=strtok(NULL, " ");
			intermediate_array[num_intermediate_rows]->opcode = atoi(token);
			
			for (int i = 0; i < 4; i++)
			{
				token = strtok(NULL, " ");
				intermediate_array[num_intermediate_rows]->parameters[i] = atoi(token);
			}
			num_intermediate_rows++;
		}
	}
	line[0] = '\0';

	if (!feof(f))
	{
		fgets(line, 100, f);
		while (strcmp(line, "SYMBOLTABLE\n") != 0)
		{
			token = strtok(line, "  ");
			strcpy(label_array[num_labels]->name, token);
			token = strtok(NULL, "  ");
			label_array[num_labels]->address = atoi(token);
			//token = strtok(NULL, " ");
			//label_array[num_labels]->address = atoi(token);
			num_labels++;
			fgets(line, 100, f);
		}
	}
	line[0] = '\0';
	while (!feof(f))
	{
		fgets(line, 100, f);
		if (line[0] == '\n')
			break;
		token = strtok(line, " ");
		if (token == NULL)
			break;
		symbol_array[num_symbols]->c = token[0];
		token = strtok(NULL, " ");
		if (token == NULL)
			break;
		symbol_array[num_symbols]->flag = atoi(token);
		token = strtok(NULL, " ");
		if (token == NULL)
			break;
		symbol_array[num_symbols]->offset = atoi(token);
		token = strtok(NULL, " ");
		if (token == NULL)
			break;
		symbol_array[num_symbols]->size = atoi(token);
		num_symbols++;
	}
}