#pragma once

#include <stdint.h>
#include <drivers/device/device.h>
#include "vfs.h"

// FileSystem constants and definitions
#define FS_SYS_CLEAN 1
#define FS_SYS_ERRORS 2

#define EXT2_DIRECT_BLOCKS 12

#define EXT2_FT_UNKNOWN  0
#define EXT2_FT_REG_FILE 1
#define EXT2_FT_DIR      2
#define EXT2_FT_CHRDEV   3
#define EXT2_FT_BLKDEV   4
#define EXT2_FT_FIFO     5
#define EXT2_FT_SOCK     6
#define EXT2_FT_SYMLINK  7

#define SUPERBLOCK_BLOCK_NUM 1

// FileSystem actions
#define FS_ACT_IGNORE 1
#define FS_ACT_REMOUNT_FS_RO 2
#define FS_ACT_HKP 3

// OS IDs
#define OSID_LINUX 0 
#define OSID_GNUHURD 1
#define OSID_MASIX 2
#define OSID_FreeBSD 3
#define OSID_OTHER 4

// Optional feature flags
#define OPT_FEAT_PREALLOCATE 0x0001
#define OPT_FEAT_AFS 0x0002
#define OPT_FEAT_FS_JOURNAL 0x0020
#define OPT_FEAT_INODES_EXT 0x0004
#define OPT_FEAT_FS_RESIZE 0x0008
#define OPT_FEAT_DIRS_HASH 0x0010

// Required feature flags
#define REQ_FEAT_COMPRESSION 0x0001
#define REQ_FEAT_DIRS_TYPE_FIELD 0x0002
#define REQ_FEAT_FS_REPLAY_JOURNAL 0x0004
#define REQ_FEAT_FS_JOURNAL_DEVICE 0x0008

// Read-only feature flags
#define RO_FEAT_SPARSE 0x0001
#define RO_FEAT_FS_LONG_FILE_SIZE 0x0002
#define RO_FEAT_DIR_STORED_BINARY_TREE 0x0004

struct fs_base_superblock {
    uint8_t inodes;
    uint8_t blocks;
    uint8_t reserved_blocks;
    uint8_t unallocated_blocks;
    uint8_t unallocated_inodes;
    uint8_t log2_block_size;
    uint8_t log2_frag_size;
    uint8_t blocks_in_each_group;
    uint8_t fragments_in_each_group;
    uint8_t inodes_in_each_group;
    uint8_t last_mount_time;            // in posix time 
    uint8_t last_written_time;          // in posix time 
    uint8_t mounts_count;
    uint8_t mounts_allowed_count;
    uint8_t signature; // ext2 magic 0xef53
    uint8_t fs_state;
    uint8_t callback_error;
    uint8_t portion;
    uint8_t interval;
    uint8_t os_id;
    uint8_t major_portion;
    uint8_t user_id;
    uint8_t group_id;

    // extended only
    uint8_t first_unreserved_inode;
    uint8_t size_inode;
    uint8_t block_group;
    uint8_t optional_feature;
    uint8_t required_feature;
    uint8_t no_supported_feature;
    char fs_id[16];
    char volume_name[16];
    char last_mounted_path_volume[64];
    uint8_t used_compression_algorithms;
    uint8_t preallocate_files_number_blocks;
    uint8_t preallocate_dirs_number_blocks;
    uint16_t unused1;
    char journal_id[16];
    uint8_t journal_inode;
    uint8_t journal_device;
    uint8_t inode_list;

    char unused2[1024-236];
} __attribute__ ((packed));

struct bgd_t {
    uint32_t block_bitmap;
    uint32_t inode_bitmap;
    uint32_t inode_table;
    uint32_t free_blocks;
    uint32_t free_inodes;
    uint32_t num_dirs;
    uint32_t unused1;
    uint32_t unused2;
} __attribute__ ((packed));

struct ext2_dir_entry_t {
    uint32_t inode;       
    uint16_t rec_len;     
    uint8_t  name_len;    
    uint8_t  file_type;   
    char     name[];      
} __attribute__ ((packed));

struct inode_t {
    uint16_t permission;
    uint16_t userid;
    uint32_t size;
    uint32_t atime;
    uint32_t ctime;
    uint32_t mtime;
    uint32_t dtime;
    uint16_t gid;
    uint16_t hard_links;
    uint32_t num_sectors;
    uint32_t flags;
    uint32_t os_specific1;
    uint32_t blocks[EXT2_DIRECT_BLOCKS + 3];
    uint32_t generation;
    uint32_t file_acl;
    union {
        uint32_t dir_acl;
        uint32_t size_high;
    };
    uint32_t f_block_addr;
    char os_specific2[12];
}__attribute__ ((packed));

struct ext2_cache_t {
    uint32_t block;
    uint32_t times;
    uint8_t dirty;
    char * block_data;
};

struct ext2_fs_t {
    vfs_node_t * disk_device;
    fs_base_superblock * sb;
    bgd_t * bgds;
    uint32_t block_size;
    uint32_t blocks_per_group;
    uint32_t inodes_per_group;
    uint32_t total_groups;
    uint32_t bgd_blocks;
};

class Ext2 {
public:
    Ext2(Device* device);
    void Init();
    void ReadBlock(uint64_t blockNum, void* buffer);
    void WriteBlock(uint64_t blockNum, void* buffer);
    void read_superblock();
    void read_block_group_descriptors();
    uint32_t get_block_from_offset(struct inode_t *inode, uint32_t offset);
    struct inode_t *get_inode(uint32_t inode_num);
    uint32_t find_free_inode();
    uint32_t find_free_block();
    void make_dir(struct vfs_node_t *parent_node, const char *name, uint16_t permission);
    uint32_t read(vfs_node_t *node, uint32_t offset, uint32_t size, char *buffer);

private:
    Device* device;
    ext2_fs_t fs;
};
