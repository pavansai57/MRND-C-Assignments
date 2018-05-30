void display_symbol_table()
{
	for (int i = 0; i < num_symbols; i++)
	{
		printf("%c %d %d\n", symbol_array[i]->c, symbol_array[i]->size, symbol_array[i]->offset);
	}
}

void display_const_table()
{
	for (int i = 0; i < num_constsymbols; i++)
	{
		printf("%c %d\n", constsymbol_array[i]->c, constsymbol_array[i]->value);
	}
}

void display_intermediate_table()
{
	for (int i = 0; i < num_intermediate_rows; i++)
	{
		printf("%d %d", intermediate_array[i]->line_num, intermediate_array[i]->opcode);
		for (int j = 0; j < 4; j++)
		{
			if (intermediate_array[i]->parameters[j] >= 0)
				printf(" %d", intermediate_array[i]->parameters[j]);
		}
		printf("\n");
	}
}

void display_label_table()
{
	for (int i = 0; i < num_labels; i++)
	{
		printf("%s %d\n", label_array[i]->name, label_array[i]->address);
	}
}