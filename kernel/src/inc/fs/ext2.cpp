#include "ext2.h"
#include <mem/heap.h>
#include <mem/memory.h>
#include <drivers/ata/ata.h>
#include <liba/string.h>
#include <log.h>
#include <panic/panic.h>

uint32_t block_size = 4096;

Ext2::Ext2(Device* device) : device(device) {}

void Ext2::ReadBlock(uint64_t blockNum, void* buffer) {
    device->Read(blockNum, buffer, block_size);
}

void Ext2::WriteBlock(uint64_t blockNum, void* buffer) {
    device->Write(blockNum, buffer, block_size);
}

void Ext2::Init() {
    log->print("Initializing fields...");
    memset(&fs, 0, sizeof(ext2_fs_t));
    fs.block_size = block_size;

    log->print("Reading superblock...");
    read_superblock();

    log->print("Allocating memory for the block group descriptors...");
    fs.bgds = (struct bgd_t *)malloc(sizeof(struct bgd_t) * fs.total_groups);
    if (fs.bgds == NULL) {
        Panic("EXT2_BGDS_MEMORY_ALLOCATION_FAILURE", nullptr);
        while (true);
    }

    log->print("Reading block group descriptors...");
    read_block_group_descriptors();
}

void Ext2::read_superblock() {
    fs.sb = (struct fs_base_superblock *)malloc(sizeof(struct fs_base_superblock));
    if (fs.sb == NULL) {
        Panic("EXT2_SUPERBLOCK_MEMORY_ALLOCATION_FAILURE", nullptr);
        while (true);
    }

    ReadBlock(SUPERBLOCK_BLOCK_NUM, fs.sb);

    if (fs.sb->log2_block_size < 0 || fs.sb->log2_block_size > 12) {
        Panic("EXT2_INVALID_LOG2_BLOCK_SIZE", nullptr);
        while (true);
    }

    fs.block_size = 1024 << fs.sb->log2_block_size;

    if (fs.block_size == 0) {
        Panic("EXT2_INVALID_BLOCK_SIZE", nullptr);
        while (true);
    }

    fs.blocks_per_group = fs.sb->blocks_in_each_group;
    fs.inodes_per_group = fs.sb->inodes_in_each_group;

    if (fs.blocks_per_group == 0) {
        Panic("EXT2_BLOCKS_PER_GROUP_ZERO", nullptr);
        while (true);
    }

    fs.total_groups = (fs.sb->blocks + fs.blocks_per_group - 1) / fs.blocks_per_group;

    if (fs.total_groups == 0) {
        Panic("EXT2_INVALID_TOTAL_GROUPS", nullptr);
        while (true);
    }
}

void Ext2::read_block_group_descriptors() {
    uint32_t block_size = 1024 << fs.sb->log2_block_size;
    uint32_t bgd_block = block_size == 1024 ? 2 : 1;
    uint32_t bgd_size = fs.total_groups * sizeof(bgd_t);
    uint32_t blocks_needed = (bgd_size + block_size - 1) / block_size;

    for (uint32_t i = 0; i < blocks_needed; ++i) {
        ReadBlock(bgd_block + i, ((char *)fs.bgds) + i * block_size);
    }
}

uint32_t Ext2::get_block_from_offset(struct inode_t *inode, uint32_t offset) {
    uint32_t block = offset / fs.block_size;
    if (block < EXT2_DIRECT_BLOCKS) {
        return inode->blocks[block];
    } else {
        block -= EXT2_DIRECT_BLOCKS;
        uint32_t block_size = fs.block_size / sizeof(uint32_t);

        if (block < block_size) {
            uint32_t *indirect_block = (uint32_t *)malloc(fs.block_size);
            ReadBlock(inode->blocks[EXT2_DIRECT_BLOCKS], indirect_block);
            uint32_t block_num = indirect_block[block];
            free(indirect_block);
            return block_num;
        }

        block -= block_size;
        if (block < block_size * block_size) {
            uint32_t *double_indirect_block = (uint32_t *)malloc(fs.block_size);
            ReadBlock(inode->blocks[EXT2_DIRECT_BLOCKS + 1], double_indirect_block);
            uint32_t indirect_block_index = block / block_size;
            uint32_t *indirect_block = (uint32_t *)malloc(fs.block_size);
            ReadBlock(double_indirect_block[indirect_block_index], indirect_block);
            uint32_t block_num = indirect_block[block % block_size];
            free(indirect_block);
            free(double_indirect_block);
            return block_num;
        }

        block -= block_size * block_size;
        uint32_t *triple_indirect_block = (uint32_t *)malloc(fs.block_size);
        ReadBlock(inode->blocks[EXT2_DIRECT_BLOCKS + 2], triple_indirect_block);
        uint32_t double_indirect_block_index = block / (block_size * block_size);
        uint32_t *double_indirect_block = (uint32_t *)malloc(fs.block_size);
        ReadBlock(triple_indirect_block[double_indirect_block_index], double_indirect_block);
        uint32_t indirect_block_index = (block / block_size) % block_size;
        uint32_t *indirect_block = (uint32_t *)malloc(fs.block_size);
        ReadBlock(double_indirect_block[indirect_block_index], indirect_block);
        uint32_t block_num = indirect_block[block % block_size];
        free(indirect_block);
        free(double_indirect_block);
        free(triple_indirect_block);
        return block_num;
    }
    return 0;
}

struct inode_t *Ext2::get_inode(uint32_t inode_num) {
    uint32_t block_group = (inode_num - 1) / fs.inodes_per_group;
    uint32_t index = (inode_num - 1) % fs.inodes_per_group;
    struct bgd_t *bgd = &fs.bgds[block_group];
    uint32_t inode_table_block = bgd->inode_table + (index * sizeof(struct inode_t)) / fs.block_size;
    uint32_t inode_table_offset = (index * sizeof(struct inode_t)) % fs.block_size;
    char *buffer = (char *)malloc(fs.block_size);
    ReadBlock(inode_table_block, buffer);
    struct inode_t *inode = (struct inode_t *)malloc(sizeof(struct inode_t));
    memcpy(inode, buffer + inode_table_offset, sizeof(struct inode_t));
    free(buffer);
    return inode;
}

uint32_t Ext2::find_free_inode() {
    for (uint32_t group = 0; group < fs.total_groups; group++) {
        struct bgd_t *bgd = &fs.bgds[group];
        char *bitmap = (char *)malloc(fs.block_size);
        ReadBlock(bgd->inode_bitmap, bitmap);

        for (uint32_t i = 0; i < fs.inodes_per_group / 8; i++) {
            for (uint8_t bit = 0; bit < 8; bit++) {
                if (!(bitmap[i] & (1 << bit))) {
                    bitmap[i] |= (1 << bit);
                    WriteBlock(bgd->inode_bitmap, bitmap);
                    free(bitmap);
                    return group * fs.inodes_per_group + i * 8 + bit + 1;
                }
            }
        }
        free(bitmap);
    }
    return 0;
}

uint32_t Ext2::find_free_block() {
    for (uint32_t group = 0; group < fs.total_groups; group++) {
        struct bgd_t *bgd = &fs.bgds[group];
        char *bitmap = (char *)malloc(fs.block_size);
        ReadBlock(bgd->block_bitmap, bitmap);

        for (uint32_t i = 0; i < fs.block_size; i++) {
            for (uint8_t bit = 0; bit < 8; bit++) {
                if (!(bitmap[i] & (1 << bit))) {
                    bitmap[i] |= (1 << bit);
                    WriteBlock(bgd->block_bitmap, bitmap);
                    free(bitmap);
                    return group * fs.blocks_per_group + i * 8 + bit;
                }
            }
        }
        free(bitmap);
    }
    return 0;
}

void Ext2::make_dir(struct vfs_node_t *parent_node, const char *name, uint16_t permission) {
    uint32_t inode_num = find_free_inode();
    uint32_t block_num = find_free_block();
    if (inode_num == 0 || block_num == 0) {
        log->print("Failed to allocate inode or block for new directory.");
        return;
    }

    // Initialize the new inode
    struct inode_t *inode = (inode_t *)malloc(sizeof(inode_t));
    memset(inode, 0, sizeof(inode_t));
    inode->size = fs.block_size;
    inode->blocks[0] = block_num;
    inode->permission = permission | FS_DIRECTORY;

    // Write the inode to the inode table
    uint32_t block_group = (inode_num - 1) / fs.inodes_per_group;
    uint32_t index = (inode_num - 1) % fs.inodes_per_group;
    struct bgd_t *bgd = &fs.bgds[block_group];
    uint32_t inode_table_block = bgd->inode_table + (index * sizeof(struct inode_t)) / fs.block_size;
    uint32_t inode_table_offset = (index * sizeof(struct inode_t)) % fs.block_size;
    char *buffer = (char *)malloc(fs.block_size);
    ReadBlock(inode_table_block, buffer);
    memcpy(buffer + inode_table_offset, inode, sizeof(struct inode_t));
    WriteBlock(inode_table_block, buffer);
    free(buffer);
    free(inode);

    // Initialize the new vfs_node_t
    struct vfs_node_t *node = (struct vfs_node_t *)malloc(sizeof(struct vfs_node_t));
    memset(node, 0, sizeof(struct vfs_node_t));
    strcpy(node->name, name);
    node->inode_num = inode_num;
    node->size = 0;
    node->device = parent_node->device;
    node->mask = permission;
    node->uid = parent_node->uid;
    node->gid = parent_node->gid;
    node->flags = FS_DIRECTORY;
    node->fs_type = VFS_EXT2_MAGIC;
    node->open_flags = 0;
    node->create_time = node->access_time = node->modified_time = 0; // Set appropriate times
    node->offset = 0;
    node->nlink = 2; // '.' and '..'
    node->refcount = 0;
    node->read = nullptr;
    node->write = nullptr;
    node->open = nullptr;
    node->close = nullptr;
    node->readdir = nullptr;
    node->finddir = nullptr;
    node->create = nullptr;
    node->unlink = nullptr;
    node->mkdir = nullptr;
    node->ioctl = nullptr;
    node->get_size = nullptr;
    node->chmod = nullptr;
    node->get_file_size = nullptr;
    node->listdir = nullptr;

    parent_node->size++;

    // Read the parent directory's block
    struct dirent *dir_entries = (struct dirent *)malloc(fs.block_size);
    ReadBlock(parent_node->inode_num, dir_entries);
    uint32_t offset = 0;

    // Add the new directory entry
    while (offset < parent_node->size * sizeof(struct dirent)) {
        if (dir_entries[offset / sizeof(struct dirent)].inode_num == 0) {
            dir_entries[offset / sizeof(struct dirent)].inode_num = inode_num;
            strcpy(dir_entries[offset / sizeof(struct dirent)].name, name);
            break;
        }
        offset += sizeof(struct dirent);
    }

    // Write the updated directory block back to disk
    WriteBlock(parent_node->inode_num, dir_entries);
    free(dir_entries);

    // Log the creation of the directory
    log->print("Directory created successfully.");
}
