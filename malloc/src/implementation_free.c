/*
** ETNA PROJECT, 7 févr. 2019 by belmeh_s
** implementation_free.c
** File description:
**
*/
#include <sys/types.h>
#include <unistd.h>
#include "malloc_header.h"
t_block fusion_with(t_block block)
{
	if (block->next_block && block->next_block->is_free) {
		block->size += BLOCK_SIZE + block->next_block->size;
		block->next_block = block->next_block->next_block;
		if (block->next_block)
			block->next_block->previous_block = block;
	}
	return (block);
}

/* Get the block from and addr */
t_block get_block_from_addr(void *addr)
{
	char *tmp;
	tmp = addr;
	return (addr = tmp -= BLOCK_SIZE);
}

/* Valid addr for free */
int is_malloced_addr(void *addr)
{
	if (base) {
		if (addr > base && addr < sbrk(0)) {
			return (addr == (get_block_from_addr(addr))->ptr);
		}
	}
	return (0);
}

/* The free */
/* See free(3) */
void free(void *addr)
{
	t_block block_to_free = NULL;

	if (is_malloced_addr(addr)) {
		block_to_free = get_block_from_addr(addr);
		block_to_free->is_free = 1;
		/* fusion with previous if possible */
		if (block_to_free->previous_block
				&& block_to_free->previous_block->is_free) {
			block_to_free = fusion_with(
					block_to_free->previous_block);
		}
		/* then fusion with next */
		if (block_to_free->next_block) {
			fusion_with(block_to_free);
		}
		else {
			/* free the end of the heap */
			if (block_to_free->previous_block) {
				block_to_free->previous_block->next_block =
						NULL;
			}
			else {
				base = NULL;
			}
		}
	}
}


