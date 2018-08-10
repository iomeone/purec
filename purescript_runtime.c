#include <gc/gc.h>
#include "Block.h"
#include "purescript_runtime.h"

// -----------------------------------------------------------------------------
// managed blocks: automatically finalized Blocks
// -----------------------------------------------------------------------------

void managed_block_release (managed_block_t * block) {
	Block_release(block->block);
}

managed_block_t * managed_block_new (void * block) {
	managed_block_t * managed_block = GC_NEW(managed_block_t);
	managed_block->block = block;
	GC_register_finalizer(
		managed_block,
		(GC_finalization_proc) managed_block_release,
		0, 0, 0);
	return managed_block;
}

// -----------------------------------------------------------------------------
// any: dynamically typed values
// -----------------------------------------------------------------------------

inline void * purs_any_get (purs_any_tag_t tag, purs_any_t * x) {
	if (tag == x->tag) {
		switch (tag) {
			case ABS_BLOCK:
				return x->value.block;
			case ABS:
				return x->value.fn;
			case CONS:
				return x->value.cons;
			default:
				return NULL;
		}
	} else {
		return NULL;
	}
}

purs_cons_t * purs_any_get_cons (purs_any_t * x) {
	return purs_any_get(CONS, x);
}

abs_t purs_any_get_abs (purs_any_t * x) {
	return (abs_t) purs_any_get(ABS, x);
}

int purs_any_get_int (purs_any_t * x) {
	if (x->tag == INT) {
		return x->value.num_int;
	} else {
		return -1;
	}
}

managed_block_t * purs_any_get_abs_block (purs_any_t * x) {
	return (managed_block_t *) purs_any_get(ABS_BLOCK, x);
}

#define PURS_ANY_SET_IMPL(_name, _type, _tag, _key) \
	purs_any_t * _name (purs_any_t * any, _type val) { \
		any->tag = _tag; \
		any->value._key = val; \
		return any; \
	}

PURS_ANY_SET_IMPL(purs_any_set_abs, abs_t *, ABS, fn)
PURS_ANY_SET_IMPL(purs_any_set_abs_block, managed_block_t *, ABS_BLOCK, block)
PURS_ANY_SET_IMPL(purs_any_set_float, float, FLOAT, num_float)
PURS_ANY_SET_IMPL(purs_any_set_int, int, INT, num_int)
PURS_ANY_SET_IMPL(purs_any_set_cons, purs_cons_t *, CONS, cons)
PURS_ANY_SET_IMPL(purs_any_set_c_string, char *, C_STRING, c_string)

purs_cons_t * purs_cons_set(
	purs_cons_t * cons,
	int tag,
	purs_any_t * value) {
	cons->tag = tag;
	cons->value = value;
	return cons;
}
