# Socket-Programming-in-C
A Client-Server program that creates a socket on its end of the communication and attempts to connect that socket to a server. When the connection is made, the server creates a socket object on its end of the communication. The client and the server can now communicate by writing to and reading from the socket.

## How to Clone

```bash
git clone https://github.com/yourusername/Socket-Programming-in-C.git
cd Socket-Programming-in-C
```

## How to Compile

### On Linux/macOS
```bash
gcc -Wall TCP_Server.c -lm -o server
gcc -Wall TCP_Client.c -lm -o client
```

### On Windows
```bash
gcc -o server TCP_Server.c -lws2_32
gcc -o client TCP_Client.c -lws2_32
```

<br>
<img src="Socket.gif">

<br>

## How To Use in Codeblocks

Goto Settings > Compiler > Linker Settings > add ws2_32 > done.

<img src="Codeblocks.gif">
