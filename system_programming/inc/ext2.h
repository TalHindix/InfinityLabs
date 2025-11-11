/*****************************************************************************
* Exercise:    ext 2
* Date:        11/11/2025
* Developer:   Tal Hindi
* Reviewer:    Guy Argaman
* Status:
*****************************************************************************/

#ifndef _ILRD_EXT2_H
#define _ILRD_EXT2_H

#include <stddef.h>  /* size_t */

int Open(char* device, char* filename);

int Read(int file_inode, char* buffer, size_t count);

#endif /* _ILRD_EXT2_H */
