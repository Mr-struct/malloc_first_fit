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
t_block get_best_fit_block_algo(t_block *last, size_t size)
{
	t_block base_block = base;
	t_block best_block = base;
	while (base_block && !(base_block->is_free && base_block->size >= size)) {
		if (best_block->size <= base_block->size) {
			*last = best_block;
		} else {
			*last = base_block;
		}
		best_block = base_block;
		base_block = base_block->next_block;
	}
	return (base_block);
}

/**
 * rise the size of the heap
 * @param last_block
 * @param size_to_expend
 * @return
 */
t_block heap_lenght_plus_plus(t_block last_block, size_t size)
{
	t_block block = NULL;
	block = sbrk(0);

	if (sbrk(BLOCK_SIZE + size) == (void*)-1) {
		return (NULL );
	}

	block->size = size;
	block->next_block = NULL;
	if (last_block) {
		last_block->next_block = block;
	}

	block->is_free = 0;
	return (block);
}

/**
 * Divide a block of memorie block_to_split
 * to the size of the geven parameter size_of_new_block
 * @param block_to_split
 * @param size_of_new_lock
 */
void diviading_block(t_block block_to_divid, size_t size_of_new_block)
{
	t_block new_block = NULL;
	new_block = (t_block)(block_to_divid->data + size_of_new_block);
	new_block->size = block_to_divid->size - size_of_new_block - BLOCK_SIZE;
	new_block->next_block = block_to_divid->next_block;
	new_block->is_free = 1;
	block_to_divid->size = size_of_new_block;
	block_to_divid->next_block = new_block;
}

t_block divid_or_expen(t_block new_block, t_block last, size_t size_aligned)
{
	if (new_block) {
		/* can we split */
		if ((new_block->size - size_aligned)
				>= ( BLOCK_SIZE + 4)) {
			diviading_block(new_block, size_aligned);
		}
		new_block->is_free = 0;
	} else {
		/* No fitting block , extend the heap */
		new_block = heap_lenght_plus_plus(last, size_aligned);
		if (!new_block) {
			return (NULL);
		}
	}
	return new_block;
}
/**
 * malloc implementation
 * @param size
 */
void *malloc(size_t size)
{
	t_block new_block = NULL;
	t_block last = NULL;
	size_t size_aligned;
	size_aligned = align4(size);
	if (base) {
		/* First find a block */
		last = base;
		new_block = get_best_fit_block_algo(&last, size_aligned);
		new_block = divid_or_expen(new_block, last, size_aligned);
	} else {
		/* first time */
		new_block = heap_lenght_plus_plus(NULL, size_aligned);
		if (!new_block) {
			return (NULL);
		}
		base = new_block;
	}
	return (new_block->data);
}
