/*****************************************************************************
* Exercise:    ext 2
* Date:        10/11/2025
* Developer:   Tal Hindi
* Reviewer:
* Status:
*****************************************************************************/

#include <stdio.h> /* printf */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "ext2.h" /* Ext2OpenFilesystem */

int main(void)
{
    char buf[64] = {0};
    int fd = Open("/dev/ram0","/myfile.txt");
    int n = Read(fd, buf, sizeof(buf)-1);
    printf("read %d bytes: \"%s\"\n", n, buf);

    return 0;
}
