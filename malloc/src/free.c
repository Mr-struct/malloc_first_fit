/*
 ** ETNA PROJECT, 7 févr. 2019
 ** free_implementation
 ** File description :
 ** 	free implementation
 **
 */
#include <sys/types.h>
#include <unistd.h>

#include "malloc_free_header.h"
/**
 * fusoin the current block with the next block
 */
t_meta_data fusion_with(t_meta_data block)
{
	if (block->next_block && block->next_block->is_free) {
		block->size += BLOCK_SIZE + block->next_block->size;
		block->next_block = block->next_block->next_block;
		if (block->next_block) {
			block->next_block->previous_block = block;
		}
	}
	return (block);
}

/**
 *  Get block from and addr
 */
t_meta_data get_block_from_addr(void *addr)
{
	char *temp = NULL;
	temp = addr;
	temp -= BLOCK_SIZE;
	addr = temp;
	return (addr);
}

/**
 *  test if the given adress is an old malloced address
 */
int is_malloced_addr(void *addr)
{
	if (base) {
		if (addr > base && addr < sbrk(0)) {
			return (addr
					== (get_block_from_addr(addr))->pointer_addr_block);
		}
	}
	return (0);
}

/**
 * free implementation
 */
void free(void *addr)
{
	t_meta_data block_to_free = NULL;

	if (is_malloced_addr(addr)) {
		block_to_free = get_block_from_addr(addr);
		block_to_free->is_free = 1;
		if (block_to_free->previous_block
						&& block_to_free->previous_block->is_free) {
			block_to_free = fusion_with(
					block_to_free->previous_block);
		}
		if (block_to_free->next_block) {
			fusion_with(block_to_free);
		} else {
			if (block_to_free->previous_block) {
				block_to_free->previous_block->next_block = NULL;
			} else {
				base = NULL;
			}
		}
	}
}
