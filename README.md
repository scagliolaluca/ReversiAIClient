# ReversiClientBasicCPP

## Requirements
- GNU Compiler
- make

## Getting Started
Basic client for Reversi. To build the client run <br>
```sh
make release
# or  compile with debug flag set
make debug
```
at the root folder of the project. The compiled client will be available in `bin/client`.
You can change the name of the compiled client by editing the `TARGET_NAME` and/or `DEBUG_NAME` in the `makefile`.

You can run the client with the following parameters:
```sh 
./bin/client # this will use the default parameters for the ip address (127.0.0.1) and port (7777) of the server
./bin/client --ip 192.168.178.1 --port 5555 # this will overwrite the default parameters
./bin/client --help # displays help information
```

## Doxygen
You can generate a doxygen documentation at the root of the project with the following command:
```sh
doxygen
```
The output can be found in the `documentaion` directory.<br>
Doxygen "how to install" can be found here: https://www.doxygen.nl/manual/install.html

## Interesting Files
Quick introduction to the general file structure of the basic client.<br>
All header files for the given .cpp files in `src/` can be found in `include/`<br>
**Keep in mind that we have not used classes. We have used namespaces to structure the code!**
<hr>

### main.cpp
1. parses arguments for server connection
2. starts networking to connect to the server and start receiving messages
3. starts the client by calling `Client::runClient();`
<hr>

### client.h/client.cpp
Sends the given groupnumber to the server with `Network::sendMsg(1, 1, msg);`. Where `msg` is just a `uint8_t`.
Then it starts a while loop and reacts to the different message types received from the server.
```cpp
while (running) {
    Network::recvMsg(&type, &len, &msg);
    switch (type) { 
        # react to different messages from the server
    }
}
```
Interesting messages might be `SEND_MOVE_TO_SERVER`. Once the client detects this message, it will call the `Moves::getRandomMove(x, y, CurrentState::boardArr, GameDetails::playerNumber);` function to get a random valid move to send it to the server. This code will represent how the message is constructed and sent:
```cpp
msg = new uint8_t[5];
msg[0] = 0;
msg[1] = x;
msg[2] = 0;
msg[3] = y;
msg[4] = 0;  // not needed in basic client

Network::sendMsg(5, 5, msg);
```
<hr>

### moves.h/moves.cpp
Within this namespace, functions are responsible to calculate valid moves. Therefore functions those functions are used: `populateValidMoves`, `isValidMove`, `validateDirection`.
The other functions are for basic reversi actions like making a move on the board, recolor once a stone is placed or just select a random move.
<hr>

### currentstate.h/currentstate.cpp
Then namespace contains global variables which hold the current state of the game. It also provides the initialization functions, populate functions and delete functions.<br>
**Hint: The neighbors array contains the neighbors of each baord tile**

## Nice to know
- You can turn on specific loggins in the `include/logging.h`. By commenting in or out.
- In the settings you can modify your group number: `include/settings.h`.
- In `gamedetails.h/gamedetails.cpp` are general informations about the game (boardwidth and -height, playercount, etc.).
- `map.h/map.cpp` contains functions to parse the map received from the server.

## No need to modify
You dont need to modifiy the `network.h/network.cpp`, `arrayops.h`

## Authors
- Johannes Treske
- Philipp Thaler