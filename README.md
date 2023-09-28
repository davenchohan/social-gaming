
# Single Threaded Web Socket Networking

This repository contains The Hot Root Soup game server 

In addition, a simple chat server is provided as well as multiple clients using
[FTXUI](https://github.com/ArthurSonzogni/FTXUI) with C++ or WebAssembly, and



## Dependencies

This project requires:

1. C++17 or newer
2. Boost >= 1.72
3. CMake >= 3.24
4. FTXUI >= 5.0 



## Building with CMake

1. Clone the repository.


2. Create a new directory for building.

        mkdir sgbuild

3. Change into the new directory.

        cd sgbuild

4. Run CMake with the path to the source.

        cmake ../social-gaming/

5. Run make inside the build directory:

        make

This produces chat server and client tools called `bin/chatserver` and
`bin/client-ftxui` respectively. The library for single threaded clients and
servers is built in `lib/`.




## Running the Social-Gaming Server

First run the chat server on an unused port of the server machine. The server
also takes an HTML file that it will server to standard http requests for
`index.html`.

    bin/chatserver 8000 ../web-socket-networking/webchat.html


This will connect to the given port (8000 in this case) of the local machine.
Connecting to a remote machine can be done by explicitly using the remote
machine's IP address instead of `localhost`. Inside the chat client, you can
enter commands or chat with other clients by typing text and hitting the
ENTER key. You can disconnect from the server by typing `quit`. You can shut
down the server and disconnect all clients by typing `shutdown`. Typing
anything else will send a chat message to other clients.

A browser based interface can be accessed by opening the URL
`http://localhost:8000/index.html`. The server will respond with the
specified web page above. By clicking `Connect`, the page gains access to
chat on the server via web sockets in browsers that support web sockets.





## Building and Running the Social-Gaming Web Client

If you have emscripten installed, then you can configure and build a C++
client that runs in the browser with WebAssembly. The GUI is driven by
FTXUI, and the browser client should behave the same as the native client.

Building is similar to a normal CMake process:

1. Create a new directory specifically for building with emscripten.

        mkdir embuild

2. Change into the new directory.

        cd embuild

3. Run the emscripten CMake wrapper with the path to the source.

        emcmake cmake ../social-gaming/

4. Run the emscripten make wrapper inside the build directory:

        emmake make

Running the client requires starting a simple web server.

        cd bin/
        ./run_webassembly.py

