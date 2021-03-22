# Very Simple Web Server
A Very Simple Web Server implemented in C

The server has been implemented using TCP sockets. The server when receives a GET request, checks for what page was requested and sends a welcome message when the homepage is accessed. Currently, the server sends 404 Error Not Found for all requests except the homepage or the file `mypage.html`.

Server sends a response using TCP with a header and then the content according to the HTTP protocol.

## Prerequisites
For running this program, you need the following
- `gcc` - The GNU Compiler Collection
- `make` - The Make utility
- A Linux system (should work on any UNIX based systems but is tested on Linux)

## Usage
First, clone the repository and change working directory to the cloned repository.
```
$ git clone github.com/aneeshsharma/VSWS_2_C
$ cd VSWS_2_C
```

To compile the server program, either use `gcc` or `make`.

```
$ make
OR
$ gcc server.c -o server.o -lpthread
```

To start the server, run `server.o` as

```
$ ./server.o
```

The server should start on the `PORT` (default 8080).

Access the homepage from a web browser by opening the URL [http://localhost:8080](http://localhost:8080)

Access the html page included using the url [http://localhost:8080/mypage.html](http://localhost:8080/mypage.html)
