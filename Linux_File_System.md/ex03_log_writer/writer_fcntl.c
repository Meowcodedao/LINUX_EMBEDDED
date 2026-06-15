/*
 * writer_fcntl.c — log writer using fcntl() F_SETLKW for exclusive locking
 *
 * Comparison: flock vs fcntl
 * +--------------------------+---------------------------+---------------------------+
 * | Property                 | flock                     | fcntl                     |
 * +--------------------------+---------------------------+---------------------------+
 * | Lock granularity         | Whole file only           | Byte range supported      |
 * | Works over NFS           | No                        | Yes                       |
 * | Inherited across fork    | Yes (child gets a copy)   | No (per open-file-desc)   |
 * | Released on crash        | Yes                       | Yes                       |
 * | Best used when           | Simple local file locking | Network FS / byte-range   |
 * +--------------------------+---------------------------+---------------------------+
 */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LOG_FILE "system.log"
#define BUF_SIZE 256

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s \"message\"\n", argv[0]);
        return 1;
    }

    int fd = open(LOG_FILE, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    struct flock fl = {
        .l_type   = F_WRLCK,
        .l_whence = SEEK_SET,
        .l_start  = 0,
        .l_len    = 0,
    };
    fcntl(fd, F_SETLKW, &fl);

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

    char line[BUF_SIZE];
    int len = snprintf(line, sizeof(line),
                       "[PID:%d] [%s] [INFO] %s\n",
                       (int)getpid(), timestamp, argv[1]);

    write(fd, line, (size_t)len);

    fl.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &fl);

    close(fd);
    return 0;
}
