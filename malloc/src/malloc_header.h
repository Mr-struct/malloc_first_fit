/*
 ** ETNA PROJECT, 31 janv. 2019
 ** malloc_header
 ** File description
 */

#ifndef MALLOC_HEADER_H_
#define MALLOC_HEADER_H_
#define align4(x) (((((x) - 1) >> 2) << 2) + 4)
#define BLOCK_SIZE sizeof(struct s_block)
typedef struct s_block *t_block;

struct s_block {
	// the size of the data
	size_t size;
	// the next block
	t_block next;
	// a flag to know if the current block is free or not
	int free;
	char data[1];
};



#endif /* MALLOC_HEADER_H_ */
