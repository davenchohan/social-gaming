/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#include "Server.h"
#include "CustomExceptions.h"


#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <map>


using networking::Server;
using networking::Connection;
using networking::Message;


std::vector<Connection> clients;

struct FakePlayer{
  std::string name;
  FakePlayer(std::string name): name{name}
    {}

  bool operator==(const FakePlayer &player) const{
    return name == player.name;
  }
  bool operator<(const FakePlayer &player) const {
    return name < player.name;
  }
};

struct FakeGame{
  std::string gameName;
  FakePlayer host;
  int minPlayers;
  int maxPlayers;
  bool audienceEnabled;
  int numRounds;
  int gameProgress;
  int gameId;
  FakeGame(int newGameId, FakePlayer &gameHost): gameId{newGameId}, host{gameHost}
  {
    gameName = "fake";
    minPlayers = 0;
    maxPlayers = 0;
    audienceEnabled = false;
    numRounds = 0;
    gameProgress = 0;
  }
  void setID(int id){
    gameId = id;
  }
};

struct FakeAudienceMember{
  std::string name;
  FakeAudienceMember(std::string name) : name{name}
    {}
};

struct FakeGameSessionHandler{
    std::map<std::string, FakePlayer> players;
    std::map<std::string, FakeAudienceMember> audienceMembers;
    int currentRound;
    int sessionId;
    FakeGame game;
    FakeGameSessionHandler(int id, FakeGame&gameObj)
      : sessionId{id}, game{gameObj}
        {}
    void addPlayer(std::string name, FakePlayer &player){

    }
};

// Empty struct for hold server request items
// This is a bad fake, to be used temporarily
// TODO: Replace this struct with a better one
struct serverRequest{
  std::string request;
  std::string gameName;
  std::string data;
  std::map<std::string, std::string> gameInfo;
  std::string gameId;
};


//TODO: Replace this function with an actual request parser
// Can be implemented here or in server.cpp
serverRequest parseRequest(const std::string &log){
  serverRequest temp;
  temp.request = "";
  temp.data = "";
  temp.gameInfo = {{"Rule1",""}, {"Rule2",""}};
  temp.gameName = "";
  temp.gameId = "1234556";
  return temp;
}

// TODO: Replace this function wtih better implementation that verifies all aspects of Game are filled
// Possible inputs: Game game, serverRequest request
void evaluateFilledGame(std::map<std::string,std::string> &gameSpec, std::map<std::string, std::string> &receivedItems){
  if (gameSpec.size() != receivedItems.size()){
    throw IncompleteGameException("Error, incomplete game");
  }else{
    // Checking for missing game spec items from client
    for (auto item : gameSpec){
      if(receivedItems.find(item.first) == receivedItems.end()){
        throw IncompleteGameException("Error, recived game spec does not have: " + item.first);
      }
    }
    return;
  }
  return;
}

void
onConnect(Connection c) {
  std::cout << "New connection found: " << c.id << "\n";
  clients.push_back(c);
}


void
onDisconnect(Connection c) {
  std::cout << "Connection lost: " << c.id << "\n";
  auto eraseBegin = std::remove(std::begin(clients), std::end(clients), c);
  clients.erase(eraseBegin, std::end(clients));
}


struct MessageResult {
  std::string result;
  bool shouldShutdown;
  bool returnAll;
};


MessageResult
processMessages(Server& server, const std::deque<Message>& incoming) {
  std::ostringstream result;
  bool quit = false;
  bool returnAll = false;
  
  for (const auto& message : incoming) {
    std::cout << "New message found: " << message.text << "\n";
    if (message.text == "quit") {
      server.disconnect(message.connection);
    } else if (message.text == "shutdown") {
      std::cout << "Shutting down.\n";
      quit = true;
    }else if(message.text =="getall"){
      returnAll = true;

    } else {
      result << message.connection.id << "> " << message.text << "\n";
    }
  }
  return MessageResult{result.str(), quit, returnAll};
}


std::deque<Message>
buildOutgoing(const std::string& log) {
  std::deque<Message> outgoing;
  for (auto client : clients) {
    outgoing.push_back({client, log});
  }
  return outgoing;
}


std::string
getHTTPMessage(const char* htmlLocation) {
  if (access(htmlLocation, R_OK ) != -1) {
    std::ifstream infile{htmlLocation};
    return std::string{std::istreambuf_iterator<char>(infile),
                       std::istreambuf_iterator<char>()};

  }

  std::cerr << "Unable to open HTML index file:\n"
            << htmlLocation << "\n";
  std::exit(-1);
}


int
main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "Usage:\n  " << argv[0] << " <port> <html response>\n"
              << "  e.g. " << argv[0] << " 4002 ./webchat.html\n";
    return 1;
  }
  std::vector<std::string> allMessages;
  const unsigned short port = std::stoi(argv[1]);
  Server server{port, getHTTPMessage(argv[2]), onConnect, onDisconnect};

  // Instantiate host
  FakePlayer host("dummy");

  // Instantiate game list
  std::vector<std::string> fakeGameList = {"Rock,Paper,Scissors"};
  std::map<std::string, std::string> fakeGameRules = {{"Rock,Paper,Scissors", "Rules:None"}};
  std::map<std::string, FakeGameSessionHandler> sessionHandlerDB;

  while (true) {
    bool errorWhileUpdating = false;
    try {
      server.update();
    } catch (std::exception& e) {
      std::cerr << "Exception from Server update:\n"
                << " " << e.what() << "\n\n";
      errorWhileUpdating = true;
    }
    

    const auto incoming = server.receive();
    const auto [log, shouldQuit, returnAll] = processMessages(server, incoming);
    std::string server_response;
    allMessages.push_back(log+"\n");
    if(returnAll){
      for(std::string str : allMessages){
        auto outgoing = buildOutgoing(str);
        server.send(outgoing);
      }
    }else {
      // Space to parse log file into specific sections: request, gameInfo, data, etc
      serverRequest request = parseRequest(log);
      // Begin logic implementation
      if (request.request == " " || request.request == ""){
        std::cout << "No message from client" << std::endl;
      }else if (request.request == "ReqCreateGame"){
        // TODO: Replace find games list with GameList.GetGameSpec(gameName)
        if (std::find(fakeGameList.begin(), fakeGameList.end(), request.gameName) != fakeGameList.end()){
          auto iterator = fakeGameRules.find(request.gameName);
          const std::string rules_template = iterator->second;
          server_response = rules_template;
        }else{
          throw UnknownGameException("Game not found: " + request.gameName);
        }
      }else if (request.request == "ReqCreateGameFilled"){
        
        std::map<std::string, std::string> fakeGameSpec = {{"players","3-10"}, {"Rounds","3"}};
        evaluateFilledGame(fakeGameSpec, request.gameInfo);
        // Instantiate Game class
        auto fakeGenerate = [](){
          return 404;
        };
        // Instantiate Game
        FakeGame fakeGame(fakeGenerate(), host);
        // Add Game to session handler
        FakeGameSessionHandler sessionHandler(fakeGame.gameId, fakeGame);
        // Add session handler to DB
        sessionHandlerDB.insert(sessionHandlerDB.end(), std::pair<std::string, FakeGameSessionHandler> {std::to_string(fakeGame.gameId), sessionHandler} );
        // Construct response
        std::string server_status = "ReqCreateGameFilled Successful" + '\n' + fakeGame.gameName + " created, GameID: " + std::to_string(fakeGame.gameId);
        server_response = server_status;
        

      }else if(request.request == "RequestJoinGame"){
        std::cout << "ReqJoinGame" << std::endl;
        
        // TODO: Replace find 
        // Search gameSessionDB for the gameId given by request
        std::string id = request.gameId;
        
        if (auto sessionIt = sessionHandlerDB.find(id); sessionIt != sessionHandlerDB.end()){
          FakePlayer player("dummy_player");
          FakeGameSessionHandler handler = sessionIt->second;
          handler.addPlayer(player.name, player);
          //Construct response
          std::string server_status = "ReqJoinGame Successful" + '\n' + player.name + " added into " + std::to_string(handler.game.gameId);
          server_response = server_status;
          
        }else{
          throw UnknownGameException("Game not found: " + request.gameName);
        }
        
        
        
      }else{
        throw UnknownRequestException("Unknown Request: " + log);
      }
      auto outgoing = buildOutgoing(log);
      server.send(outgoing);
      
    }
    

    if (shouldQuit || errorWhileUpdating) {
      break;
    }

    sleep(1);
  }

  return 0;
}

