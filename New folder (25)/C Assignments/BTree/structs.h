struct student{
	int id;
	char name[32];
};

struct data_page{
	struct student stdnt[3];
	char unused[20];
};

/*
struct index_page{
	int pageid[16];
	int key[15];
	char unused[4];
};
*/




struct table_page{

	char pagetype;
	char tableid;
	char unused[6];
	int rootpageid[10];
	char tablename[10][8];
};

struct marks{
	int id;
	int subjects[4];
};

struct dp1{
	char pagetype;
	char tableid;
	char unused[18];
	struct student stdnt[3];
};

struct dp2{
	char pagetype;
	char tableid;
	char unused[6];
	struct marks data[6];
};

struct index_page{
	char pagetype;
	char tableid;
	char unused[2];
	int childpageid[16];
	int key[15];
};




