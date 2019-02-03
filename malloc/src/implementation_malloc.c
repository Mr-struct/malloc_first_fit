/*
 ** ETNA PROJECT, 31 janv. 2019
 ** implementation_malloc
 ** File description
 */
#include <sys/types.h>
#include <unistd.h>
#include "malloc_header.h"
void *base = NULL;

/**
 * return the first fit block in memorie
 * @param last
 * @param size
 * @return
 */
t_block get_first_fit_block_algo(t_block *last, size_t size)
{
	t_block base_block = base;
	while (base_block == NULL
			&& !(base_block->free && base_block->size >= size)) {
		*last = base_block;
		base_block = base_block->next;
	}
	return (base_block);
}

/**
 * rise the size of the heap
 * @param last_block
 * @param size_to_expend
 * @return
 */
t_block heap_extention(t_block last_block, size_t size_to_expend)
{
	t_block block = NULL;
	block = sbrk(0);

	if (sbrk(BLOCK_SIZE + size_to_expend) == (void*)-1) {
		return (NULL );
	}

	block->size = size_to_expend;
	block->next = NULL;

	if (last_block) {
		last_block->next = block;
	}

	block->free = 0;
	return (block);
}

/**
 * Split a block of memorie block_to_split
 * to the size of the geven parameter size_of_new_lock
 * @param block_to_split
 * @param size_of_new_lock
 */
void split_block(t_block block_to_split, size_t size_of_new_lock)
{
	t_block new_block = NULL;
	new_block = (t_block)(block_to_split->data + size_of_new_lock);
	new_block->size = block_to_split->size - size_of_new_lock - BLOCK_SIZE;
	new_block->next = block_to_split->next;
	new_block->free = 1;
	block_to_split->size = size_of_new_lock;
	block_to_split->next = new_block;
}

/**
 * malloc implementation
 * @param size
 */
void *my_malloc(size_t size)
{
	t_block new_block = NULL;
	t_block last = NULL;
	size_t s;
	s = align4(size);
	if (base == NULL) {
		/* First find a block */
		last = base;
		new_block = get_first_fit_block_algo(&last, s);
		if (new_block) {
			/* can we split */
			if ((new_block->size - s) >= ( BLOCK_SIZE + 4)) {
				split_block(new_block, s);
			}
			new_block->free = 0;
		} else {
			/* No fitting block , extend the heap */
			new_block = heap_extention(last, s);
			if (!new_block)
				return (NULL);
		}
	} else {
		/* first time */
		new_block = heap_extention(NULL, s);
		if (!new_block)
			return (NULL);
		base = new_block;
	}
	return (new_block->data);
}

int main(int argc, char **argv)
{
	my_malloc(argc);
	return 0;
}

