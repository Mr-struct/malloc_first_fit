/*
 ** ETNA PROJECT, 31 janv. 2019
 ** malloc_header
 ** File description
 */

#ifndef MALLOC_HEADER_H_
#define MALLOC_HEADER_H_
typedef struct s_block *t_block;

struct s_block {
	// the size of the data
	size_t size;
	//the previous block
	t_block prev;
	// the next block
	t_block next;
	// a flag to know if the current block is free or not
	int free;
	// the pointer of the array indicate the end of meta data
	char data[1];
};

#define align4(x) (((((x) - 1) >> 2) << 2) + 4)
#define BLOCK_SIZE 12

#endif /* MALLOC_HEADER_H_ */
