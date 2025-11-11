/*****************************************************************************
* Exercise:    ext 2
* Date:        10/11/2025
* Developer:   Tal Hindi
* Reviewer:
* Status:
*****************************************************************************/

#ifndef _ILRD_EXT2_H
#define _ILRD_EXT2_H

#include <stddef.h>  /* size_t */

ssize_t Open(char* device, char* filename);

ssize_t Read(int file_inode, char* buffer, size_t count);

#endif /* _ILRD_EXT2_H */
