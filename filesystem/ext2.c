#include "ext2.h"

ext2_diskCacheEntry_t*	ext2_cache = NULL;
ext2_superblock_t*		ext2_superblock = NULL;
ext2_bgdescriptor_t*	ext2_rootBlock = NULL;
fs_node_t*				ext2_root_fsnode = NULL;