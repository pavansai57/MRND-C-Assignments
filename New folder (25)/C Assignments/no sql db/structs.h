struct table{
	char* table_name = NULL;
	int num_cols = 0;
	char** col_names = NULL;
	struct row_node* head = NULL;
	int num_types = 0;
	int* types;//0 int
			   //1 string
	struct table* next = NULL;
};

struct row_node{
	int current_num = 0;
	int commit_num = 0;
	struct row_node* next_row = NULL;
	struct col_node* col_head = NULL;
	int id = 0;
	int type = 0;
};

struct col_node{
	char* name;
	void* value;
	int version = 0;
	struct col_node* next = NULL;
	struct col_node* next_version = NULL;
	//void* key;
	int size = 0;
	int type = -1;
};
