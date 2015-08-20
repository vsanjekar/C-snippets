/*
 * http://www.wangafu.net/~nickm/libevent-book/01_intro.html
 * It is a trivial server that listens for TCP connections on port 40713,
 * reads data from its input one line at a time, and writes out the ROT13
 * obfuscation of line each as it arrives. It uses the Unix fork() call to
 * create a new process for each incoming connection.
 *
 * TEST this code
 * netcat/nc
 * cat input.txt | nc 127.0.0.1 40713
 */

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

char
rot13_char(char c)
{
    /* We don't want to use isalpha here; setting the locale would change
     * which characters are considered alphabetical. */
    if ((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M'))
        return c + 13;
    else if ((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z'))
        return c - 13;
    else
        return c;
}

void child(int fd) {
    char buf[1024];
    size_t buf_used = 0;
    ssize_t result;

    while (1) {
        char ch;
        result = recv(fd, &ch, 1, 0);
        if (result < 0) {
            perror("read error");
            break;
        } else if (result == 0) {
            break;
        }
        // possible overflow. Ignore.
        buf[buf_used++] = rot13_char(ch);

        if (ch == '\n') {
            send(fd, buf, buf_used, 0);
            buf_used = 0;
            continue;
        }
    }
}

void run() {
    int listener_fd;
    struct sockaddr_in sin;

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = 0;
    sin.sin_port = htons(40713);

    listener_fd = socket(AF_INET, SOCK_STREAM, 0);

#ifndef WIN32
    {
        int one;
        setsockopt(listener_fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    }
#endif

    if (bind(listener_fd, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        fprintf(stderr, "bind error \n");
        perror("bind");
        return;
    }

    if (listen(listener_fd, 16) < 0) {
        perror("listen");
        return;
    }

    while (1) {
        struct sockaddr_storage ss;
        socklen_t len = sizeof(ss);
        int fd = accept(listener_fd, (struct sockaddr*)&ss, &len);
        if (fd < 0) {
            perror("accept");
            return;
        }
        if (fork() == 0) {
            printf("Got a connection. Forking a child.\n");
            child(fd);
            exit(0);
        }
    }
}

int main(int argc, const char **argv) {
    run();
    return 0;
}
