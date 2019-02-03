/*
 ** ETNA PROJECT, 31 janv. 2019
 ** implementation_malloc
 ** File description
 */
#include <sys/types.h>
#include <unistd.h>
#include "malloc_header.h"
void *base = NULL;

t_block get_first_fit_block(t_block *last, size_t size)
{
	t_block b = base;
	while (b && !(b->free && b->size >= size )) {
		*last = b;
		b = b->next;
	}
	return (b);
}

t_block extend_heap (t_block last , size_t s)
{
	t_block b;
	b = sbrk (0);
	if (sbrk( BLOCK_SIZE + s) == (void*) -1)
		return (NULL );
	b->size = s;
	b->next = NULL;
	if (last)

		last ->next = b;
	b->free = 0;
	return (b);
}

void split_block(t_block b, size_t s)
{
	t_block new_block;
	new_block = b->data + s;
	new_block->size = b->size - s - BLOCK_SIZE;
	new_block->next = b->next;
	new_block->free = 1;
	b->size = s;
	b->next = new_block;
}
void *malloc(size_t size)
{
	t_block new_block, last;
	size_t s;
	s = align4(size);
	if (base) {
		/* First find a block */
		last = base;
		new_block = get_first_fit_block(&last, s);
		if (new_block) {
			/* can we split */
			if ((new_block->size - s) >= ( BLOCK_SIZE + 4)) {
				split_block(new_block, s);
			}
			new_block->free = 0;
		} else {
			/* No fitting block , extend the heap */
			new_block = extend_heap(last, s);
			if (!new_block)
				return (NULL);
		}
	} else {
		/* first time */
		new_block = extend_heap(NULL, s);
		if (!new_block)
			return (NULL);
		base = new_block;
	}
	return (new_block->data);
}
