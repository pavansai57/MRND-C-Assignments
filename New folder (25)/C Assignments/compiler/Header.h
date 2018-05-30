
char* registernames[] = { "AX", "BX", "CX", "DX", "EX", "FX", "GX", "HX" };
char* not_opcodes[] = { "DATA", "CONST" };
char* opcodes[] = { "MOV", "MOV", "ADD", "SUB", "MUL", "JUMP", "IF", "EQ", "LT", "GT", "LTEQ", "GTEQ", "PRINT", "READ", "ENDIF","ELSE" };
char* labels[] = { "ELSE", "ENDIF" };
char* buffer;
char registers[8];

struct symbol** symbol_array;
struct constsymbol** constsymbol_array;
struct intermediate** intermediate_array;
struct label** label_array;

int num_symbols=0;
int num_constsymbols=0;
int num_intermediate_rows=0;
int num_labels=0;

struct parameter{
	char c;
	int offset;
};

struct intermediate{
	int line_num;
	int opcode;
	int parameters[4];
};


struct symbol{
	char c;
	int size = -1;
	int offset;
	int flag = 0;
};

struct constsymbol{
	char c;
	char value;
};

struct label{
	char* name;
	int address = -1;
};