/*
 * Very Simple Web Server
 * A simple HTTP server implemented in C
 * Author:  Anish Sharma
 * Date:    14-03-2021
 */

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>

#define PORT 5000

#define BUFFER_SIZE 1024
#define MAX_REQUEST_SIZE 200

int get_reqest_page(char *request, char *page)
{
    char *start = "GET /";
    int i = 0;
    for (i = 0; i < strlen(start); i++)
    {
        if (start[i] != request[i])
            return 0;
    }
    int j = 0;
    for (; i < strlen(request); i++)
    {
        if (request[i] == ' ')
            break;
        page[j] = request[i];
        j++;
    }

    if (i == strlen(request))
        return 0;
    return 1;
}

void *handle_client(void *arg)
{
    int sock_fd = *((int *)arg);

    int count = 0;
    char *message = "Welcome to Networks Lab!";

    char buffer[BUFFER_SIZE];

    int recv_len = recv(sock_fd, buffer, BUFFER_SIZE, 0);

    int x = 0;
    char request[MAX_REQUEST_SIZE];
    char page[MAX_REQUEST_SIZE];

    // Get the line of request
    for (x = 0; buffer[x] != '\n' && x < MAX_REQUEST_SIZE - 1; x++)
    {
        request[x] = buffer[x];
    }
    request[x] = '\0';

    printf("- Received request: %s\n", request);

    // Send not found for all requests except homepage
    if (!get_reqest_page(request, page) || strlen(page) != 0)
    {
        printf("- Page %s not found!\n", page);
        count = 0;
        count += sprintf(buffer + count, "HTTP/1.0 404 File not found\n");
        count += sprintf(buffer + count, "Server: VSWSInC/0.0\n");
        count += sprintf(buffer + count, "Date: \n");
        count += sprintf(buffer + count, "Connection: close\n");
        count += sprintf(buffer + count, "Content-type: text/plain\n");
        count += sprintf(buffer + count, "Content-Length: 9\n");
        count += sprintf(buffer + count, "Last-Modified: \n");
        count += sprintf(buffer + count, "\nRequested page Not Found\n");
        send(sock_fd, buffer, count, 0);
        close(sock_fd);
        return NULL;
    }

    // Send HTTP 200 OK response for homepage with content inside message

    count = 0;
    count += sprintf(buffer + count, "HTTP/1.0 200 OK\n");
    count += sprintf(buffer + count, "Server: VSWSInC/0.0\n");
    count += sprintf(buffer + count, "Date: \n");
    count += sprintf(buffer + count, "Content-type: text/plain\n");
    count += sprintf(buffer + count, "Content-Length: %d\n", strlen(message));
    count += sprintf(buffer + count, "Last-Modified: \n");

    count += sprintf(buffer + count, "\n%s\n", message);

    send(sock_fd, buffer, count, 0);

    close(sock_fd);
}

int main()
{
    int server_fd, conn_socket, recv_len;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    printf("Starting Very Simple Web Server....\n");
    printf("A simple server that responsds to HTTP requests\n");

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\nError creating socket\n");
        return -1;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        printf("\nCouldn't set socket options\n");
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        printf("\nError binding port\n");
        return -1;
    }

    printf("Serving at http://0.0.0.0:%d\n", PORT);

    if (listen(server_fd, 10) < 0)
    {
        printf("\nError listening to socket\n");
        return -1;
    }

    while (1)
    {
        if ((conn_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            printf("Error accepting connection!!!\n");
            return -1;
        }

        printf("- New connection!\n");
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *)&conn_socket);
    }

    return 0;
}