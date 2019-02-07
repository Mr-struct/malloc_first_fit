/*
 ** ETNA PROJECT, 31 janv. 2019
 ** implementation_malloc
 ** File description :
 ** 	malloc implementation
 */
#include <sys/types.h>
#include <unistd.h>
#include "malloc_free_header.h"

/**
 *
 * return the first fit block in memorie
 *
 */
t_meta_data get_best_fit_block_algo(t_meta_data *last, size_t size)
{
	t_meta_data base_block = base;
	t_meta_data best_block = base;

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
 */
t_meta_data heap_lenght_plus_plus(t_meta_data last_block, size_t size)
{
	t_meta_data block = NULL;

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
 */
void diviading_block(t_meta_data block_to_divid, size_t size_of_new_block)
{
	t_meta_data new_block = NULL;
	new_block = (t_meta_data)(block_to_divid->data + size_of_new_block);
	new_block->size = block_to_divid->size - size_of_new_block - BLOCK_SIZE;
	new_block->next_block = block_to_divid->next_block;
	new_block->is_free = 1;
	block_to_divid->size = size_of_new_block;
	block_to_divid->next_block = new_block;
}

t_meta_data divid_or_expen(t_meta_data new_block, t_meta_data last, size_t size_aligned)
{
	if (new_block) {
		if ((new_block->size - size_aligned) >= ( BLOCK_SIZE + 4)) {
			diviading_block(new_block, size_aligned);
		}
		new_block->is_free = 0;
	} else {
		new_block = heap_lenght_plus_plus(last, size_aligned);
		if (!new_block) {
			return (NULL);
		}
	}
	return new_block;
}
/**
 * malloc implementation
 */
void *malloc(size_t size)
{
	t_meta_data new_block = NULL;
	t_meta_data last = NULL;
	size_t size_aligned = 0;
	size_aligned = align(size);

	if (base) {
		last = base;
		new_block = get_best_fit_block_algo(&last, size_aligned);
		new_block = divid_or_expen(new_block, last, size_aligned);
	} else {
		new_block = heap_lenght_plus_plus(NULL, size_aligned);
		if (!new_block) {
			return (NULL);
		}
		base = new_block;
	}
	return (new_block->data);
}

