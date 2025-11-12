/*****************************************************************************
* Exercise:    ext 2
* Date:        11/11/2025
* Developer:   Tal Hindi
* Reviewer:    Guy Argaman
* Status:      Approved
*****************************************************************************/

#include <stdio.h> /* printf */

#include "ext2.h" /* Open */

int main(int argc, char** argv)
{
    char buf[64] = {0};
    int fd = Open(argv[1],argv[2]);
    int n = Read(fd, buf, sizeof(buf)-1);
    printf("read %d bytes: \"%s\"\n", n, buf);

    return 0;
}
