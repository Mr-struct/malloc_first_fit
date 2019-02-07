/*
 ** ETNA PROJECT, 31 janv. 2019
 ** malloc_header
 ** File description :
 ** 	malloc and free header
 */
#ifndef MALLOC_FREE_HEADER_H_
#define MALLOC_FREE_HEADER_H_
#define align(x) (((((x) - 1) >> 2) << 2) + 4)
#define BLOCK_SIZE 12
static void *base = NULL;
typedef struct s_meta_data *t_meta_data;
struct s_meta_data {
	size_t size;
	t_meta_data previous_block;
	t_meta_data next_block;
	int is_free;
	void *pointer_addr_block;
	char data[1];
};
t_meta_data get_best_fit_block_algo(t_meta_data *last, size_t size);
t_meta_data heap_lenght_plus_plus(t_meta_data last_block, size_t size);
void diviading_block(t_meta_data block_to_divid, size_t size_of_new_block);
t_meta_data divid_or_expen(t_meta_data new_block, t_meta_data last, size_t size_aligned);
void *malloc(size_t size);
t_meta_data fusion_with(t_meta_data block);
t_meta_data get_block_from_addr(void *addr);
int is_malloced_addr(void *addr);
void free(void *addr);
#endif /* MALLOC_FREE_HEADER_H_ */
