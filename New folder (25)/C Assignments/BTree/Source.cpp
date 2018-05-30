#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "structs.h"

/*
struct index_page* create_index(struct data_page* pages,int i,int j)
{
	struct index_page* index = (struct index_page*)malloc(sizeof(struct index_page));
	memset(index, 0, sizeof(struct index_page));
	for ()
}*/


struct data_page* create_new_page(struct student** stdnt, int i,int j)
{
	struct data_page* dp = (struct data_page*)malloc(sizeof(struct data_page));
	memset(dp, 0, sizeof(struct data_page));
	int k = 0;
	for (; k < j - i; k++)
		dp->stdnt[k] = *(stdnt[i + k]);
	return dp;
}

struct student* create_student_from_file(FILE* f)
{
	if (!feof(f))
	{
		char line[1024];
		char* token;
		struct student* stdnt = (struct student*)malloc(sizeof(struct student));
		fgets(line, 1024, f);
		token = strtok(line, ",");
		if (token == NULL)
			return NULL;
		stdnt->id = atoi(token);
		token = strtok(NULL, ",");
		if (token == NULL)
			return NULL;
		strcpy(stdnt->name, token);
		return stdnt;
	}
	else
		return NULL;
}

struct marks* create_marks_from_file(FILE* f)
{
	char line[1024];
	if (!feof(f))
	{
		char* token;
		struct marks* marks = (struct marks*)malloc(sizeof(struct marks));
		fgets(line, 1024, f);
		token = strtok(line, ",");
		if (token == NULL)
			return NULL;
		marks->id = atoi(token);
		for (int i = 0; i < 4; i++)
		{
			token = strtok(NULL, ",");
			if (token == NULL)
				return NULL;
			marks->subjects[i] = atoi(token);
		}
		return marks;
	}
	else
		return NULL;
}


int write_all_student_records_as_pages(char src_file[], char dest_file[], int records_per_page)
{
	char line[1000];

	FILE* src = fopen(src_file, "r");
	FILE* dest = fopen(dest_file, "wb");
	fgets(line, 1000, src);
	int k = 0;
	int num_pages = 0;
	while (!feof(src))
	{
		struct dp1* page = (struct dp1*)malloc(sizeof(struct dp1));
		memset(page, 0, sizeof(struct dp1));
		page->pagetype = '1';
		page->tableid = '1';
		int enter = 0;
		for (int i = 0; i < records_per_page&&!feof(src); i++)
		{
			enter = 1;
			struct student* stdnt = create_student_from_file(src);
			page->stdnt[i] = *stdnt;
			free(stdnt);
			k++;
		}
		if (enter == 1)
		{
			fwrite(page, sizeof(struct dp1), 1, dest);
			num_pages++;
		}
		free(page);
	}
	//printf("%d\n", k);
	fclose(src);
	fclose(dest);
	return num_pages;
}

int write_all_marks_records_as_pages(char src_file[], char dest_file[],int offset, int records_per_page)
{
	char line[1000];

	FILE* src = fopen(src_file, "r");
	FILE* dest = fopen(dest_file, "ab");

	//fseek(src, offset, SEEK_SET);
	fgets(line, 1000, src);
	int k = 0;
	int num_pages = 0;
	while (!feof(src)&&num_pages<8)
	{
		struct dp2* page = (struct dp2*)malloc(sizeof(struct dp2));
		memset(page, 0, sizeof(struct dp2));
		int enter = 0;
		for (int i = 0; i < records_per_page&&!feof(src); i++)
		{
			enter = 1;
			struct marks* marks = create_marks_from_file(src);
			page->data[i] = *marks;
			free(marks);
			k++;
		}
		if (enter == 1)
		{
			fwrite(page, sizeof(struct dp2), 1, dest);
			num_pages++;
		}
		free(page);
	}
	//printf("%d\n", k);
	fclose(src);
	fclose(dest);
	return num_pages;
}

int write_level_one_student_indexes(char *filename, int offset, int num_page_ids, int num_data_pages)
{
	FILE* f1 = fopen(filename, "rb");
	FILE* f2 = fopen(filename, "ab");

	//fseek(f1, offset, SEEK_SET);
	struct dp1 initial_page;
	struct dp1 dt_page;

	struct index_page idx_page;
	memset(&idx_page, 0, sizeof(struct index_page));
	idx_page.pagetype = '1';
	idx_page.tableid = '1';

	int i = 0;
	int k = 0;
//	int offset = 0;
	int num_level_one = 0;
	while (k<num_data_pages)
	{
		if (i == 0)
		{
			fread(&initial_page, sizeof(struct dp1), 1, f1);
			idx_page.childpageid[0] = k * 128;
		}
		else
		{
			fread(&dt_page, sizeof(struct dp1), 1, f1);
			idx_page.childpageid[i] = k * 128;
			idx_page.key[i - 1] = dt_page.stdnt[0].id;//dt_page.data[0].id;//dt_page.stdnt[0].id;-->marks and students can keep  data variable same.
		}
		i++;
		if (i == num_page_ids)
		{
			i = 0;
			//fclose(f);
			//f = fopen(filename, "ab");
			fwrite(&idx_page, sizeof(struct index_page), 1, f2);
			memset(&idx_page, 0, sizeof(struct index_page));
			num_level_one++;
		}
		k++;
	}
	if (i != 0)
	{
		fwrite(&idx_page, sizeof(struct index_page), 1, f2);
		num_level_one++;
	}
	fclose(f1);
	fclose(f2);

	return num_level_one;
}

int write_level_one_marks_indexes(char* filename, int offset, int num_page_ids, int num_data_pages)
{
	FILE* f1 = fopen(filename, "rb");
	FILE* f2 = fopen(filename, "ab");

	fseek(f1, offset, SEEK_SET);
	struct dp2 initial_page;
	struct dp2 dt_page;

	struct index_page idx_page;
	memset(&idx_page, 0, sizeof(struct index_page));
	idx_page.pagetype = '2';
	idx_page.tableid = '2';

	int i = 0;
	int k = 0;
	//	int offset = 0;
	int num_level_one = 0;
	while (k<num_data_pages)
	{
		if (i == 0)
		{
			fread(&initial_page, sizeof(struct dp1), 1, f1);
			idx_page.childpageid[0] = offset+k * 128;
		}
		else
		{
			fread(&dt_page, sizeof(struct dp1), 1, f1);
			idx_page.childpageid[i] = offset+k * 128;
			idx_page.key[i - 1] = dt_page.data[0].id;//dt_page.data[0].id;//dt_page.stdnt[0].id;-->marks and students can keep  data variable same.
		}
		i++;
		if (i == num_page_ids)
		{
			i = 0;
			//fclose(f);
			//f = fopen(filename, "ab");
			fwrite(&idx_page, sizeof(struct index_page), 1, f2);
			memset(&idx_page, 0, sizeof(struct index_page));
			num_level_one++;
		}
		k++;
	}
	if (i != 0)
	{
		fwrite(&idx_page, sizeof(struct index_page), 1, f2);
		num_level_one++;
	}
	fclose(f1);
	fclose(f2);

	return num_level_one;
}


void get_student(char* filename,int offset,int id)
{
	FILE* f = fopen(filename, "rb");
	fseek(f, offset, SEEK_SET);

	struct index_page idx_page;
	int seek_offset;
	for (int j = 0; j < 1; j++)
	{
		int i;
		fread(&idx_page, sizeof(struct index_page), 1, f);
		for (i = 0; i < 15; i++)
		{
			if (idx_page.key[i] > id)
			{
				seek_offset = idx_page.childpageid[i];
				break;
			}
		}
		if (i == 15)
			seek_offset = idx_page.childpageid[i];

		fseek(f, seek_offset, SEEK_SET);
	}

	fseek(f, seek_offset, SEEK_SET);
	struct dp1 dt_page;
	fread(&dt_page, sizeof(struct dp1), 1, f);

	for (int i = 0; i < 3; i++)
	{
		if (dt_page.stdnt[i].id == id)
		{
			printf("%d %s",id, dt_page.stdnt[i].name);
			break;
		}
	}
	fclose(f);
}

void get_students(char* filename,int offset, int gt_id, int lt_id)
{
	FILE* f = fopen(filename, "rb");
	fseek(f, offset, SEEK_SET);

	struct dp1 dt_page1;
	struct dp1 dt_page2;

	struct index_page idx_page;
	int seek_offset_lt;
	for (int j = 0; j < 1; j++)
	{
		int i;
		fread(&idx_page, sizeof(struct index_page), 1, f);
		for (i = 0; i < 15; i++)
		{
			if (idx_page.key[i] > lt_id)
			{
				seek_offset_lt = idx_page.childpageid[i];
				break;
			}
		}
		if (i == 15)
			seek_offset_lt = idx_page.childpageid[i];

		fseek(f, seek_offset_lt, SEEK_SET);
	}
	fread(&dt_page1, sizeof(struct data_page), 1, f);

	fseek(f, offset, SEEK_SET);

	int seek_offset_gt;
	for (int j = 0; j < 1; j++)
	{
		int i;
		fread(&idx_page, sizeof(struct index_page), 1, f);
		for (i = 0; i < 15; i++)
		{
			if (idx_page.key[i] > gt_id)
			{
				seek_offset_gt = idx_page.childpageid[i];
				break;
			}
		}
		if (i == 15)
			seek_offset_gt = idx_page.childpageid[i-1];

		fseek(f, seek_offset_gt, SEEK_SET);
	}

	//fread(&dt_page2, sizeof(struct data_page), 1, f);
	
	while (seek_offset_gt < seek_offset_lt)
	{
		fread(&dt_page2, sizeof(struct dp1), 1, f);
		for (int i = 0; i < 3; i++)
		{
			printf("%d %s\n", dt_page2.stdnt[i].id, dt_page2.stdnt[i].name);
		}
		seek_offset_gt = seek_offset_gt + 128;
	}

	for (int i = 0; i < 3; i++)
	{
		if (dt_page1.stdnt[i].id <= lt_id)
		{
			printf("%d %s\n", dt_page1.stdnt[i].id, dt_page1.stdnt[i].name);
		}
		else
			break;
	}
	fclose(f);

	/*fseek(f, seek_offset, SEEK_SET);
	struct data_page dt_page;
	fread(&dt_page, sizeof(struct data_page), 1, f);

	for (int i = 0; i < 3; i++)
	{
		if (dt_page.stdnt[i].id == id)
		{
			printf("%d %s", id, dt_page.stdnt[i].name);
			break;
		}
	}
	fclose(f);
	*/
}

void get_marks(char* filename, int offset, int gt_id, int lt_id)
{
	FILE* f = fopen(filename, "rb");
	fseek(f, offset, SEEK_SET);

	struct dp2 dt_page1;
	struct dp2 dt_page2;

	struct index_page idx_page;
	int seek_offset_lt;
	for (int j = 0; j < 1; j++)
	{
		int i;
		fread(&idx_page, sizeof(struct index_page), 1, f);
		for (i = 0; i < 15; i++)
		{
			if (idx_page.key[i] > lt_id)
			{
				seek_offset_lt = idx_page.childpageid[i];
				break;
			}
		}
		if (i == 15)
			seek_offset_lt = idx_page.childpageid[i];

		fseek(f, seek_offset_lt, SEEK_SET);
	}
	fread(&dt_page1, sizeof(struct dp2), 1, f);

	fseek(f, offset, SEEK_SET);

	int seek_offset_gt;
	for (int j = 0; j < 1; j++)
	{
		int i;
		fread(&idx_page, sizeof(struct index_page), 1, f);
		for (i = 0; i < 15; i++)
		{
			if (idx_page.key[i] > gt_id)
			{
				seek_offset_gt = idx_page.childpageid[i];
				break;
			}
		}
		if (i == 15)
			seek_offset_gt = idx_page.childpageid[i - 1];

		fseek(f, seek_offset_gt, SEEK_SET);
	}

	//fread(&dt_page2, sizeof(struct data_page), 1, f);

	while (seek_offset_gt < seek_offset_lt)
	{
		fread(&dt_page2, sizeof(struct dp2), 1, f);
		for (int i = 0; i < 6; i++)
		{
			if (dt_page2.data[i].id >= gt_id)
			{
				printf("%d ", dt_page2.data[i].id);
				for (int j = 0; j < 4; j++)
				{
					printf("%d ", dt_page2.data[i].subjects[j]);
				}
				printf("\n");
			}
		}
		seek_offset_gt = seek_offset_gt + 128;
	}

	for (int i = 0; i < 6; i++)
	{
		if (dt_page1.data[i].id <= lt_id)
		{
			printf("%d ", dt_page1.data[i].id);
			for (int j = 0; j < 4; j++)
			{
				printf("%d ", dt_page1.data[i].subjects[j]);
			}
			//printf("%d %s\n", dt_page1.stdnt[i].id, dt_page1.stdnt[i].name);
			printf("\n");
		}
		else
			break;
	}
	fclose(f);
}



void get_students_contains(char* filename, int offset, int gt_id, int lt_id,char* contains)
{
	FILE* f = fopen(filename, "rb");
	fseek(f, offset, SEEK_SET);

	struct data_page dt_page1;
	struct data_page dt_page2;

	struct index_page idx_page;
	int seek_offset_lt;
	for (int j = 0; j < 2; j++)
	{
		int i;
		fread(&idx_page, sizeof(struct index_page), 1, f);
		for (i = 0; i < 15; i++)
		{
			if (idx_page.key[i] > lt_id)
			{
				seek_offset_lt = idx_page.childpageid[i];
				break;
			}
		}
		if (i == 15)
			seek_offset_lt = idx_page.childpageid[i];

		fseek(f, seek_offset_lt, SEEK_SET);
	}
	fread(&dt_page1, sizeof(struct data_page), 1, f);

	fseek(f, offset, SEEK_SET);

	int seek_offset_gt;
	for (int j = 0; j < 2; j++)
	{
		int i;
		fread(&idx_page, sizeof(struct index_page), 1, f);
		for (i = 0; i < 15; i++)
		{
			if (idx_page.key[i] > gt_id)
			{
				seek_offset_gt = idx_page.childpageid[i];
				break;
			}
		}
		if (i == 15)
			seek_offset_gt = idx_page.childpageid[i - 1];

		fseek(f, seek_offset_gt, SEEK_SET);
	}

	//fread(&dt_page2, sizeof(struct data_page), 1, f);

	while (seek_offset_gt < seek_offset_lt)
	{
		fread(&dt_page2, sizeof(struct data_page), 1, f);
		for (int i = 0; i < 3; i++)
		{
			if (strstr(dt_page2.stdnt[i].name, contains)!=NULL)
				printf("%d %s\n", dt_page2.stdnt[i].id, dt_page2.stdnt[i].name);
		}
		seek_offset_gt = seek_offset_gt + 128;
	}

	for (int i = 0; i < 3; i++)
	{
		if (dt_page1.stdnt[i].id <= lt_id)
		{
			if (strstr(dt_page1.stdnt[i].name, contains) != NULL)
				printf("%d %s\n", dt_page1.stdnt[i].id, dt_page1.stdnt[i].name);
		}
		else
			break;
	}

	fclose(f);
	/*fseek(f, seek_offset, SEEK_SET);
	struct data_page dt_page;
	fread(&dt_page, sizeof(struct data_page), 1, f);

	for (int i = 0; i < 3; i++)
	{
	if (dt_page.stdnt[i].id == id)
	{
	printf("%d %s", id, dt_page.stdnt[i].name);
	break;
	}
	}
	fclose(f);
	*/
}

void create_table_page(char* name)
{
	FILE* f = fopen(name, "ab");
	struct table_page tb_page;
	tb_page.pagetype = '3';
	tb_page.rootpageid[0] = 16 * 128;
	tb_page.rootpageid[1] = 3200;
	tb_page.tableid = '1';
	strcpy(tb_page.tablename[0], "student");
	strcpy(tb_page.tablename[1], "marks");
	fwrite(&tb_page, 128, 1, f);
	fclose(f);
}

int get_root_page_offset(char* name,struct table_page* table)
{
	if (strcmp(table->tablename[0],name)==0)
		return table->rootpageid[0];
	if (strcmp(table->tablename[1], name)==0)
		return table->rootpageid[1];
}

struct table_page* get_table_index(char* filename)
{
	FILE* f = fopen(filename, "rb");
	fseek(f, -128, SEEK_END);
	struct table_page* tab = (struct table_page*)malloc(sizeof(struct table_page));
	fread(tab, sizeof(struct table_page), 1, f);
	fclose(f);
	return tab;
}

void join(char* name_one,char* name_two, struct table_page* table)
{
	int offset1 = get_root_page_offset(name_one, table);
	int offset2 = get_root_page_offset(name_two, table);

	struct index_page idx_page1;
	struct index_page idx_page2;

	FILE* f1 = fopen("a.b", "rb");
	FILE* f2 = fopen("a.b", "rb");

	fseek(f1, offset1, SEEK_SET);
	fseek(f2, offset2, SEEK_SET);

	fread(&idx_page1, sizeof(struct index_page), 1, f1);
	fread(&idx_page2, sizeof(struct index_page), 1, f2);

	int offset_student = idx_page1.childpageid[0];
	int offset_marks = idx_page2.childpageid[0];

	fseek(f1, offset_student, SEEK_SET);
	fseek(f2, offset_marks, SEEK_SET);


	struct dp1 dt_page1;
	struct dp2 dt_page2;

	int i = 0;
	int j = 0;

	fread(&dt_page1, sizeof(struct dp1), 1, f1);
	fread(&dt_page2, sizeof(struct dp2), 1, f2);

	while (offset_student < offset1 && offset_marks < offset2)
	{
		if (i >= 3)
		{
			if (offset_student >= offset1)
				break;
			fread(&dt_page1, sizeof(struct dp1), 1, f1);
			offset_student = offset_student + 128;
			i = 0;
		}
		if (j >= 6)
		{
			if (offset_marks >= offset2)
				break;
			fread(&dt_page2, sizeof(struct dp2), 1, f2);
			offset_marks = offset_marks + 128;
			j = 0;
		}
		while (i < 3 && j < 6)
		{
			if (dt_page1.stdnt[i].id == dt_page2.data[j].id)
			{
				int sum = 0;
				printf("%d %s ", dt_page1.stdnt[i].id, dt_page1.stdnt[i].name);
				for (int k = 0; k < 4; k++)
				{
					sum = sum + dt_page2.data[j].subjects[k];
					printf("%d ", dt_page2.data[j].subjects[k]);
				}
				printf("%d\n",sum);
				i++;
				j++;
			}
			else if (dt_page1.stdnt[i].id < dt_page2.data[j].id)
			{
				i++;
			}
			else if (dt_page1.stdnt[i].id > dt_page2.data[j].id)
			{
				j++;
			}
		}
	}
}



int main()
{
	int num_pages_students=write_all_student_records_as_pages("student_48.txt", "a.b", 3);//randomuserdata.txt
	int num_level_one_student = write_level_one_student_indexes("a.b", 0, 16, num_pages_students);
	int num_pages_marks = write_all_marks_records_as_pages("marks.txt", "a.b",(num_pages_students+num_level_one_student)*128, 6);
	FILE* f = fopen("a.b", "rb");
	struct dp2 dt_page;
	fseek(f, (num_pages_students + num_level_one_student) * 128, SEEK_SET);
	for (int i = 0; i < 8;i++)
	{
		fread(&dt_page, 128, 1, f);
	}
	fclose(f);
	int num_level_one_marks = write_level_one_marks_indexes("a.b", (num_pages_students + num_level_one_student) * 128,16, num_pages_marks);

	create_table_page("a.b");
	//get_student("a.b", 2048, 2);
	//printf("Asd");
	//int offset = write_level_two_indexes("a.b", 16, num_pages*128,num_level_one);
	/*

	int num_pages_marks = write_all_marks_records_as_pages("marks.txt", "a.b", 6);
	FILE* f = fopen("a.b", "rb");
	struct dp2 dp;
	for (int i = 0; i < 8; i++)
	{
		fread(&dp, sizeof(struct dp2), 1, f);
	}
	*/
//	int num_level_one = write_level_one_indexes("a.b", 16, num_pages);

	//struct tablepage tb;
	//struct dp1 d1;
//	struct dp2 d2; 
	//struct indexpage itb;

	while (1)
	{
		int c=0;
		printf("\n\n1. get student id\n");
		printf("\n2. enter range\n");
		scanf("%d", &c);
		if (c == 1)
		{
			int id;
			printf("\n enter id\n");
			scanf("%d", &id);
			int root_offset = 2048;

			get_student("a.b", root_offset, id);
		}
		else if (c == 2)
		{
			int lt_id;
			int gt_id;
			printf("\nenter lt_id");
			scanf("%d", &lt_id);
			printf("\nenter gt_id");
			scanf("%d", &gt_id);
			int root_offset = 2048;
			get_students("a.b", root_offset, gt_id, lt_id);
		}
		else if (c == 3)
		{
			int lt_id;
			int gt_id;
			char contains[100];
			printf("\nenter lt_id");
			scanf("%d", &lt_id);
			printf("\nenter gt_id");
			scanf("%d", &gt_id);
			printf("\ncontains: \n");
			scanf("%s", contains);
			int root_offset = 2048;
			get_students_contains("a.b", root_offset, gt_id, lt_id,contains);
		}
		else if (c == 4)
		{
			int lt_id;
			int gt_id;
			printf("\nenter lt_id");
			scanf("%d", &lt_id);
			printf("\nenter gt_id");
			scanf("%d", &gt_id);
			int root_offset = 3200;
			get_marks("a.b", root_offset, gt_id, lt_id);
		}
		else if (c == 5)
		{
			struct table_page* tab = get_table_index("a.b");
			join("student", "marks", tab);
		}
	}




	/*
	printf("1");

	fclose(f);
	f = fopen("a.b", "wb");

	//struct data_page* pages = (struct data_page*)malloc(sizeof(struct data_page) * 4);
	for (int i = 0; i < 16; i++)
	{
		fwrite(pages[i], sizeof(struct data_page), 1, f);
	}
	f = fopen("a.b", "rb");

	struct index_page* index;



	struct data_page initial_page;
	struct data_page dt_page;

	struct index_page idx_page;
	int i = 0;
	while (!feof(f))
	{
		if (i = 0)
		{
			fread(&initial_page, sizeof(struct data_page), 1, f);
			idx_page.pageid[0] = 0;
		}
		else
		{
			fread(&dt_page, sizeof(struct data_page), 1, f);
			idx_page.pageid[i] = i;
			idx_page.key[i - 1] = dt_page.stdnt[0].id;
		}
		i++;
	}
	fclose(f);
	f = fopen("a.b", "ab");
	fwrite(&idx_page, sizeof(idx_page), 1, f);
	fclose(f);

	printf("1");

	printf("!");
	*/
}
