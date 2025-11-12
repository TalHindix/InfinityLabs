/*****************************************************************************
* Exercise:    ext 2
* Date:        11/11/2025
* Developer:   Tal Hindi
* Reviewer:    Guy Argaman
* Status:      Approved
*****************************************************************************/

#define _POSIX_C_SOURCE 200809L

#include <ext2fs/ext2_fs.h> /* ext2 structs */
#include <stdio.h>          /* printf */
#include <stdlib.h>         /* malloc */
#include <sys/types.h>      /* off_t */
#include <sys/stat.h>       /* S_ISREG */
#include <fcntl.h>          /* open */
#include <unistd.h>         /* close */
#include <string.h>         /* strlen */

#include "ext2.h" /* Open */

#define BASE_OFFSET       (1024)
#define EXT2_ROOT_INODE   (2)
#define MIN(a, b)         (((a) < (b)) ? (a) : (b))
/* ext2 block numbers are absolute; offset = block * block_size */
#define BLOCK_OFFSET(block) ((off_t)(block) * g_block_size)

/* Global variables */
static int g_device_fd = -1;
static struct ext2_super_block g_superblock;
static struct ext2_group_desc g_group_desc;
static size_t g_block_size = 0;

/* Helper funcs - forward declaration */
static int LoadSuperBlock(void);
static int LoadGroupDesc(void);
static int ReadBlock(size_t block_num, void* buffer);
static int GetInode(size_t inode_num, struct ext2_inode* inode);
static int FindInDirectory(size_t dir_inode_num, const char* name, size_t* found_inode);
static int PathToInode(const char* path, size_t* inode_num);

/* API Implementation */
int Open(char* device, char* filename)
{
    size_t inode_num = 0;

    g_device_fd = open(device, O_RDONLY);
    if (g_device_fd < 0)
    {
        perror("open device");
        return -1;
    }

    if (0 != LoadSuperBlock())
    {
        close(g_device_fd);
        g_device_fd = -1;
        return -1;
    }

    if (0 != LoadGroupDesc())
    {
        close(g_device_fd);
        g_device_fd = -1;
        return -1;
    }

    if (0 != PathToInode(filename, &inode_num))
    {
        fprintf(stderr, "File  iss not found: %s\n", filename);
        close(g_device_fd);
        g_device_fd = -1;
        return -1;
    }

    return (int)inode_num;
}

int Read(int file_inode, char* buffer, size_t count)
{
    struct ext2_inode inode = {0};
    void* block_buf = NULL;
    size_t total_read = 0;
    size_t i = 0;
    size_t bytes_to_copy = 0;

    if (g_device_fd < 0)
    {
        fprintf(stderr, "Device not open\n");
        return -1;
    }

    if (NULL == buffer)
    {
        fprintf(stderr, "Invalid buffer\n");
        close(g_device_fd);
        g_device_fd = -1;
        return -1;
    }

    if (0 != GetInode((size_t)file_inode, &inode))
    {
        close(g_device_fd);
        g_device_fd = -1;
        return -1;
    }

    if (!S_ISREG(inode.i_mode))
    {
        fprintf(stderr, "Not a regular file\n");
        close(g_device_fd);
        g_device_fd = -1;
        return -1;
    }

    count = MIN(count, inode.i_size);

    block_buf = malloc(g_block_size);
    if (NULL == block_buf)
    {
        perror("malloc");
        close(g_device_fd);
        g_device_fd = -1;
        return -1;
    }

    for (i = 0; i < EXT2_NDIR_BLOCKS && total_read < count && 0 != inode.i_block[i]; ++i)
    {
        if (0 != ReadBlock(inode.i_block[i], block_buf))
        {
            free(block_buf);
            close(g_device_fd);
            g_device_fd = -1;
            return -1;
        }

        bytes_to_copy = (total_read + g_block_size > count) ? count - total_read : g_block_size;

        memcpy(buffer + total_read, block_buf, bytes_to_copy);
        total_read += bytes_to_copy;
    }

    free(block_buf);
    close(g_device_fd);
    g_device_fd = -1;
    return (int)total_read;
}

/* Helper functions implementations */
static int ReadBlock(size_t block_num, void* buffer)
{
    ssize_t bytes_read = pread(g_device_fd, buffer, g_block_size, BLOCK_OFFSET(block_num));

    if ((ssize_t)g_block_size != bytes_read)
    {
        perror("pread block");
        return -1;
    }

    return 0;
}

static int GetInode(size_t inode_num, struct ext2_inode* inode)
{
    size_t inode_index = (inode_num - 1) % g_superblock.s_inodes_per_group;
    off_t inode_offset = BLOCK_OFFSET(g_group_desc.bg_inode_table) + (off_t)(inode_index * g_superblock.s_inode_size);

    ssize_t bytes_read = pread(g_device_fd, inode, sizeof(struct ext2_inode), inode_offset);
    if (sizeof(struct ext2_inode) != bytes_read)
    {
        perror("pread inode");
        return -1;
    }

    return 0;
}

static int FindInDirectory(size_t dir_inode_num, const char* name, size_t* found_inode)
{
    struct ext2_inode dir_inode = {0};
    struct ext2_dir_entry_2* entry = NULL;
    void* block = NULL;
    size_t i = 0;

    *found_inode = 0;

    if (0 != GetInode(dir_inode_num, &dir_inode))
    {
        return -1;
    }

    if (!S_ISDIR(dir_inode.i_mode))
    {
        fprintf(stderr, "Not a directory\n");
        return -1;
    }

    block = malloc(g_block_size);
    if (NULL == block)
    {
        perror("malloc");
        return -1;
    }

    for (i = 0; i < EXT2_NDIR_BLOCKS && 0 != dir_inode.i_block[i]; ++i)
    {
        size_t entry_offset = 0;

        if (0 != ReadBlock(dir_inode.i_block[i], block))
        {
            free(block);
            return -1;
        }

        while (entry_offset + sizeof(struct ext2_dir_entry_2) <= g_block_size)
        {
            entry = (struct ext2_dir_entry_2*)((char*)block + entry_offset);

            if (0 >= entry->rec_len)
            {
                break;
            }

            if (0 != entry->inode && strlen(name) == entry->name_len && 0 == memcmp(entry->name, name, entry->name_len))
            {
                *found_inode = entry->inode;
                free(block);
                return 0;
            }

            entry_offset += entry->rec_len;
        }
    }

    free(block);
    return -1;
}

static int PathToInode(const char* path, size_t* inode_num)
{
    char* path_copy = NULL;
    char* token = NULL;

    if (0 == strcmp(path, "/"))
    {
        *inode_num = EXT2_ROOT_INODE;
        return 0;
    }

    if ('/' != path[0])
    {
        fprintf(stderr, "Path must start with '/'\n");
        return -1;
    }

    *inode_num = EXT2_ROOT_INODE;

    path_copy = malloc(strlen(path) + 1);
    if (NULL == path_copy)
    {
        perror("malloc");
        return -1;
    }

    strcpy(path_copy, path); /* strtok change the original path , so we have to make a copy before use it */
    token = strtok(path_copy, "/");

    while (NULL != token)
    {
        size_t temp_inode = 0;

        if (0 != FindInDirectory(*inode_num, token, &temp_inode))
        {
            fprintf(stderr, "FindInDirectory:Not found: %s\n", token);
            free(path_copy);
            return -1;
        }

        *inode_num = temp_inode;
        token = strtok(NULL, "/");
    }

    free(path_copy);
    return 0;
}

static int LoadSuperBlock(void)
{
    ssize_t bytes_read = pread(g_device_fd, &g_superblock, sizeof(struct ext2_super_block), BASE_OFFSET);

    if (sizeof(struct ext2_super_block) != bytes_read)
    {
        perror("pread superblock");
        return -1;
    }

    if (EXT2_SUPER_MAGIC != g_superblock.s_magic)
    {
        fprintf(stderr, "WRONG ext2 magic number\n");
        return -1;
    }

    g_block_size = 1024 << g_superblock.s_log_block_size;

    return 0;
}

static int LoadGroupDesc(void)
{
    off_t gd_offset = (off_t)g_block_size * (g_superblock.s_first_data_block + 1);

    ssize_t bytes_read = pread(g_device_fd, &g_group_desc, sizeof(struct ext2_group_desc), gd_offset);

    if (sizeof(struct ext2_group_desc) != bytes_read)
    {
        perror("pread group descriptor");
        return -1;
    }

    return 0;
}
