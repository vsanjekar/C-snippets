/*
 * http://www.wangafu.net/~nickm/libevent-book/01_intro.html
 * Here’s an example of a really simple client using blocking network calls.
 * It opens a connection to www.google.com, sends it a simple HTTP request,
 * and prints the response to stdout.
 */

#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, const char **argv) {
    const char hostname[] = "www.google.com";
    const char query[] =
        "GET / HTTP/1.0\r\n"
        "Host: www.google.com\r\n"
        "\r\n";


    // Lookup the IP address for hostname. Not threadsafe.
    // Blocking
    struct hostent *h = gethostbyname(hostname);
    if (!h) {
        fprintf(stderr, "Error retrieving the host information for %s. %s\n", hostname, hstrerror(h_errno));
        return -1;
    }

    // Skip IPv6
    if (h->h_addrtype != AF_INET) {
        fprintf(stderr, "IPv6 not supported.\n");
        return -1;
    }

    fprintf(stdout, "Official host name %s\n", h->h_name);

    // create a socket and connect
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(80);
    sin.sin_addr = *(struct in_addr *)h->h_addr;
    // Blocking
    if (connect(fd, (struct sockaddr *)&sin, sizeof(sin)) == -1) {
        fprintf(stderr, "Error connecting to the host %s. %s\n", hostname, strerror(errno));
        close(fd);
        return -1;
    }

    // Write to the socket
    int remaining = strlen(query);
    char const *copy = query;
    while (remaining) {
        // Blocking
        int n_written = send(fd, copy, remaining, 0);
        if (n_written < 0) {
            perror("send");
            close(fd);
            return -1;
        }
        remaining -= n_written;
        copy += n_written;
    }

    // Read the response
    char buf[2048] = "";
    while(1) {
        // Blocking
        ssize_t result = recv(fd, buf, sizeof(buf), 0);
        if (result == 0) {
            break;
        } else if (result < 0) {
            fprintf(stderr, "Error reading from the host %s. %s\n", hostname, strerror(errno));
            close(fd);
            return -1;
        }
        fwrite(buf, 1, result, stdout);
    }

    fprintf(stdout, "Official host name %s\n", buf);
    close(fd);
    return 0;
}
