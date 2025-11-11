/* /*****************************************************************************
* Exercise:    ext 2
* Date:        10/11/2025
* Developer:   Tal Hindi
* Reviewer:
* Status:
*****************************************************************************/


#include "ext2.h"

#include <ext2fs/ext2_fs.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BASE_OFFSET 1024
#define EXT2_ROOT_INODE 2

static int g_device_fd = -1;
static struct ext2_super_block g_superblock;
static struct ext2_group_desc g_group_desc;
static size_t g_block_size = 0;

static void PrintSuperBlock(void);
static void PrintGroupDesc(void);
static int ReadBlock(size_t block_num, void* buffer);
static int GetInode(size_t inode_num, struct ext2_inode* inode);
static int FindInDirectory(size_t dir_inode_num, const char* name,
                          size_t* found_inode);
static int PathToInode(const char* path, size_t* inode_num);
static void Cleanup(void);

int Open(char* device, char* filename)
{
    off_t gd_offset = 0;
    size_t inode_num = 0;

    g_device_fd = open(device, O_RDONLY);
    if (g_device_fd < 0)
    {
        perror("open device");
        return -1;
    }

    lseek(g_device_fd, BASE_OFFSET, SEEK_SET);
    read(g_device_fd, &g_superblock, sizeof(struct ext2_super_block));

    if (g_superblock.s_magic != 0xEF53)
    {
        fprintf(stderr, "Not an ext2 filesystem\n");
        close(g_device_fd);
        g_device_fd = -1;
        return -1;
    }

    g_block_size = 1024 << g_superblock.s_log_block_size;

    PrintSuperBlock();

    gd_offset = g_block_size * (g_superblock.s_first_data_block + 1);
    lseek(g_device_fd, gd_offset, SEEK_SET);
    read(g_device_fd, &g_group_desc, sizeof(struct ext2_group_desc));

    PrintGroupDesc();

    if (PathToInode(filename, &inode_num) != 0)
    {
        fprintf(stderr, "File not found: %s\n", filename);
        Cleanup();
        return -1;
    }

    return (int)inode_num;
}

int Read(int file_inode, char* buffer, size_t count)
{
    struct ext2_inode inode;
    size_t file_size = 0;
    size_t to_read = 0;
    size_t total_read = 0;
    void* block_buf = NULL;
    size_t i = 0;

    if (GetInode((size_t)file_inode, &inode) != 0)
    {
        Cleanup();
        return -1;
    }

    if (!S_ISREG(inode.i_mode))
    {
        fprintf(stderr, "Not a regular file\n");
        Cleanup();
        return -1;
    }

    file_size = inode.i_size;
    to_read = (count < file_size) ? count : file_size;

    block_buf = malloc(g_block_size);

    for (i = 0; i < EXT2_NDIR_BLOCKS && inode.i_block[i] != 0; ++i)
    {
        size_t bytes_in_block = 0;

        if (total_read >= to_read)
        {
            break;
        }

        ReadBlock(inode.i_block[i], block_buf);

        bytes_in_block = g_block_size;
        if (total_read + bytes_in_block > to_read)
        {
            bytes_in_block = to_read - total_read;
        }

        memcpy(buffer + total_read, block_buf, bytes_in_block);
        total_read += bytes_in_block;
    }

    free(block_buf);
    Cleanup();

    return (int)total_read;
}

static void PrintSuperBlock(void)
{
    printf("\n------ Printing Super Block ------\n");
    printf("Reading super-block:\n"
           "Inodes count            : %u\n"
           "Blocks count            : %u\n"
           "Reserved blocks count   : %u\n"
           "Free blocks count       : %u\n"
           "Free inodes count       : %u\n"
           "First data block        : %u\n"
           "Block size              : %zu\n"
           "Blocks per group        : %u\n"
           "Inodes per group        : %u\n"
           "Creator OS              : %u\n"
           "First non-reserved inode: %u\n"
           "Size of inode structure : %hu\n",
           g_superblock.s_inodes_count,
           g_superblock.s_blocks_count,
           g_superblock.s_r_blocks_count,
           g_superblock.s_free_blocks_count,
           g_superblock.s_free_inodes_count,
           g_superblock.s_first_data_block,
           g_block_size,
           g_superblock.s_blocks_per_group,
           g_superblock.s_inodes_per_group,
           g_superblock.s_creator_os,
           g_superblock.s_first_ino,
           g_superblock.s_inode_size);
}

static void PrintGroupDesc(void)
{
    printf("\n------ Printing Group Desc ------\n");
    printf("Reading first group-descriptor:\n"
           "Blocks bitmap block: %u\n"
           "Inodes bitmap block: %u\n"
           "Inodes table block : %u\n"
           "Free blocks count  : %u\n"
           "Free inodes count  : %u\n"
           "Directories count  : %u\n",
           g_group_desc.bg_block_bitmap,
           g_group_desc.bg_inode_bitmap,
           g_group_desc.bg_inode_table,
           g_group_desc.bg_free_blocks_count,
           g_group_desc.bg_free_inodes_count,
           g_group_desc.bg_used_dirs_count);
}

static int ReadBlock(size_t block_num, void* buffer)
{
    off_t offset = (off_t)(block_num * g_block_size);

    lseek(g_device_fd, offset, SEEK_SET);
    read(g_device_fd, buffer, g_block_size);

    return 0;
}

static int GetInode(size_t inode_num, struct ext2_inode* inode)
{
    size_t inodes_per_group = 0;
    size_t index = 0;
    size_t inode_table = 0;
    off_t offset = 0;

    inodes_per_group = g_superblock.s_inodes_per_group;
    index = (inode_num - 1) % inodes_per_group;
    inode_table = g_group_desc.bg_inode_table;

    offset = (off_t)(inode_table * g_block_size + index * g_superblock.s_inode_size);


    lseek(g_device_fd, offset, SEEK_SET);
    read(g_device_fd, inode, sizeof(struct ext2_inode));

    return 0;
}

static int FindInDirectory(size_t dir_inode_num, const char* name, size_t* found_inode)
{
    struct ext2_inode dir_inode = {0};
    struct ext2_dir_entry_2* entry = NULL;
    void* block = NULL;
    int i = 0;
    size_t offset = 0;

    *found_inode = 0;

    GetInode(dir_inode_num, &dir_inode);

    if (!S_ISDIR(dir_inode.i_mode))
    {
        fprintf(stderr, "Not a directory\n");
        return -1;
    }

    block = malloc(g_block_size);

    for (i = 0; i < EXT2_NDIR_BLOCKS && dir_inode.i_block[i] != 0; ++i)
    {
        ReadBlock(dir_inode.i_block[i], block);

        offset = 0;
        while (offset < g_block_size)
        {
            entry = (struct ext2_dir_entry_2*)((char*)block + offset);

            if (entry->rec_len == 0)
            {
                break;
            }

            if (entry->inode &&
                entry->name_len == strlen(name) &&
                memcmp(entry->name, name, entry->name_len) == 0)
            {
                *found_inode = entry->inode;
                free(block);
                return 0;
            }

            offset += entry->rec_len;
        }
    }

    free(block);
    return -1;
}

static int PathToInode(const char* path, size_t* inode_num)
{
    size_t current_inode = 0;
    size_t next_inode = 0;
    char* path_copy = NULL;
    char* token = NULL;

    if (strcmp(path, "/") == 0)
    {
        *inode_num = EXT2_ROOT_INODE;
        return 0;
    }

    if (path[0] != '/')
    {
        fprintf(stderr, "Path must start with '/'\n");
        return -1;
    }

    current_inode = EXT2_ROOT_INODE;

    path_copy = malloc(strlen(path) + 1);
    strcpy(path_copy, path);
    token = strtok(path_copy, "/");

    while (token != NULL)
    {
        printf("token: %s\n", token);

        if (FindInDirectory(current_inode, token, &next_inode) != 0)
        {
            fprintf(stderr, "Not found: %s\n", token);
            free(path_copy);
            return -1;
        }

        current_inode = next_inode;
        token = strtok(NULL, "/");
    }

    free(path_copy);
    *inode_num = current_inode;

    return 0;
}

static void Cleanup(void)
{
    if (g_device_fd >= 0)
    {
        close(g_device_fd);
        g_device_fd = -1;
    }

    g_block_size = 0;
}

