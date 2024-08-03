#include "vfs.h"
#include <mem/heap.h>
#include <panic/panic.h>

vfs_node_t* initialize_vfs_node(uint32_t inode_num) {
    vfs_node_t *node = (struct vfs_node_t *)malloc(sizeof(struct vfs_node_t));
    if (node == NULL) {
        Panic("VFS_NODE_IS_NULL", nullptr);
        while (true);
        return NULL;
    }

    // Initialize fields
    node->inode_num = inode_num;
    // Other initializations

    return node;
}