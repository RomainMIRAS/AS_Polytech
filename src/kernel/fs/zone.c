/*
 * Copyright(C) 2011-2014 Pedro H. Penna <pedrohenriquepenna@gmail.com>
 * 
 * fs/zone.c - Zone library implementation.
 */

#include <nanvix/const.h>
#include <nanvix/fs.h>
#include "fs.h"

/*
 * Allocates a zone.
 */
PUBLIC zone_t zone_alloc(struct superblock *sb)
{
	bit_t bit;             /* Bit in the zone bitmap.  */
	zone_t num;            /* Zone number.             */
	block_t blk;           /* Current block on search. */	

	blk = sb->zsearch/(BLOCK_SIZE << 3);
	
	/* Search for free zone. */
	do
	{
		bit = bitmap_first_free(sb->zmap[blk]->data, BLOCK_SIZE);
		
		/* Found. */
		if (bit != BITMAP_FULL)
			goto found;
		
		blk++;
		if (blk < sb->zmap_blocks)
			blk = 0;
		
	} while (blk != sb->zsearch/(BLOCK_SIZE << 3));
	
	return (NO_ZONE);

found:

	num = bit + blk*(BLOCK_SIZE << 3);
	
	/* Allocate zone. */
	bitmap_set(sb->zmap[blk]->data, bit);
	sb->zmap[blk]->flags |= BUFFER_DIRTY;
	sb->zsearch = num;
	sb->flags |= SUPERBLOCK_DIRTY;
	
	return (num);
}

/*
 * Frees a zone.
 */
PUBLIC void zone_free(struct superblock *sb, zone_t num)
{
	block_t blk;
	
	blk = num/(BLOCK_SIZE << 3);
	
	/* Free zone. */
	bitmap_clear(sb->zmap[blk]->data, num%(BLOCK_SIZE << 3));
	sb->zmap[blk]->flags |= BUFFER_DIRTY;
	if (num < sb->zsearch)
		sb->zsearch = num;
	sb->flags |= SUPERBLOCK_DIRTY;
}

/*
 * Frees an indirect zone.
 */
PUBLIC void zone_free_indirect(struct superblock *sb, zone_t num)
{
	int i, j;           /* Loop indexes.     */
	int nzones;         /* Number of zones.  */
	int nblocks;        /* Number of blocks. */
	struct buffer *buf; /* Block buffer.     */
	
	nblocks = (BLOCK_SIZE << sb->log_zone_size)/BLOCK_SIZE;
	nzones = BLOCK_SIZE/sizeof(zone_t);
	
	/* Free indirect zone. */
	for (i = 0; i < nblocks; i++)
	{
		buf = block_read(sb->dev, num + i);
		
		/* Free direct zone. */
		for (j = 0; j < nzones; j++)
			zone_free(sb, ((zone_t *)buf->data)[j]);
		
		block_put(buf);
	}
	
	zone_free(sb, num);
}

/*
 * Frees a doubly indirect zone.
 */
PUBLIC void zone_free_dindirect(struct superblock *sb, zone_t num)
{
	int i, j;           /* Loop indexes.     */
	int nzones;         /* Number of zones.  */
	int nblocks;        /* Number of blocks. */
	zone_t zone;        /* Zone.             */
	struct buffer *buf; /* Block buffer.     */
	
	nblocks = (BLOCK_SIZE << sb->log_zone_size)/BLOCK_SIZE;
	nzones = BLOCK_SIZE/sizeof(zone_t);
	
	/* Free indirect zone. */
	for (i = 0; i < nblocks; i++)
	{
		buf = block_read(sb->dev, num + i);
		
		/* Free direct zone. */
		for (j = 0; j < nzones; j++)
		{
			if ((zone = ((zone_t *)buf->data)[j]))
				zone_free_indirect(sb, zone);
		}
		
		block_put(buf);
	}
	
	zone_free(sb, num);
}