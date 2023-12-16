#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define pointer_size 4 //in bits
#define data_size 10 //in bits 
typedef struct mydisk{
	int m_node;
	int d_node;
	int t_blocks;
	int disk_size;
	int bitmap_size;
	int main_bitblock;
	int data_bitblock;
	unsigned char* disk;
}mydisk;

int ceiling(int a, int b)
{
	int c = a/b;
	if(a%b != 0)
		c = c+1;
	return c;
}

int abs(int a)
{
	if(a<0)
		a = -1 * a;
	return a;
}

mydisk* create_disk()
{
	mydisk *m = (mydisk *)malloc(sizeof(mydisk));
	// initializing size of both the nodes 
	m->m_node = pointer_size * 2;
	m->d_node = pointer_size + data_size;

	// total possible number of blocks in whole disk
	m->t_blocks = 1<<pointer_size;
	m->t_blocks = m->t_blocks - 1;
	m->disk_size = ceiling(m->m_node * m->t_blocks,8); //in bytes
	m->bitmap_size = ceiling(m->t_blocks + pointer_size,8); //in bytes

	m->main_bitblock = ceiling(m->bitmap_size*8,m->m_node); //number of blocks for main bitmap
	m->data_bitblock = ceiling(m->bitmap_size*8,m->d_node); //number of blocks for data bitmap
	
	m->disk = (unsigned char *)malloc(sizeof(unsigned char) * m->disk_size);
	memset(m->disk,0,m->disk_size);
	return m;
}
void disk_initialize(mydisk *m)
{
	unsigned char *temp[m->bitmap_size];
	int m_point = m->t_blocks - m->data_bitblock; // represents the last available main block
	printf("%x\n",m_point);
	m_point = m_point <<(8-(pointer_size % 8));
	printf("%x\n",m_point);
	int i = ceiling(pointer_size,8) - 1; // decrement by 1 because of 0 based indexing 
	while(i>=0)
	{
		m->disk[i] = m->disk[i] | m_point;
		m_point = m_point >> 8;
		i = i - 1;
	}
	i = ceiling(pointer_size,8) - 1;
	int extra_bits = 8 - (pointer_size % 8);
	int pre_bitmap = m->main_bitblock;
	printf("%d\n",pre_bitmap);
	while(pre_bitmap >= 8)
	{
		if(extra_bits > 0)
		{
			int x = 1<<extra_bits;
			x = x - 1;
			printf("%d\n",x);
			m->disk[i] = m->disk[i] | x;
			pre_bitmap = pre_bitmap - extra_bits;
			extra_bits = 0;
		}
		else
		{
			m->disk[i] = m->disk[i] | 0xFF;
			pre_bitmap = pre_bitmap - 8;
		}
		i = i + 1;
	}
	if(pre_bitmap>0)
	{
		if(extra_bits > 0)
		{
			int x = 1<< extra_bits;
			x = x-1;
		}
		else
		{
			int x = 1<<7;
			x = x-1;
			x = x << (8-pre_bitmap);
			m->disk[i] = m->disk[i] | x;
		}
	}
	for (int j = 0;j<m->disk_size;j++)
		printf("%x ",m->disk[j]);
	printf("\n"); 
}

int main()
{
	mydisk *m = create_disk();
	printf("disk size: %d\n",m->disk_size);
	disk_initialize(m);
} 

