/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#include "Server.h"
#include "CustomExceptions.h"
#include "Player.h"
#include "Game.h"
#include "GameSessionHandler.h"
#include "GameVariable.h"
#include "AudienceMember.h"
#include "GameList.h"
#include "ParserLibrary.h"
#include "GameSessionList.h"
#include "RandomIdGenerator.h"

#include <fstream>
#include <sstream>
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
using Json = nlohmann::json;


std::vector<Connection> clients;

// Empty struct for hold server request items
// This is a bad fake, to be used temporarily
// TODO: Replace this struct with a better one that will be created by parseRequest() function
struct serverRequest{
  std::string request;
  std::string gameName;
  std::string data;
  std::map<std::string, std::string> gameInfo;
  std::string gameId;
  std::map<std::string, std::string> gameVariables;
};


//TODO: Implement this function
// Can be implemented here or in server.cpp
serverRequest parseRequest(const std::string &log){
  serverRequest temp;
  temp.request = "";
  temp.data = "";
  temp.gameInfo = {{"Rule1",""}, {"Rule2",""}};
  temp.gameName = "";
  temp.gameId = "1234556";
  temp.gameVariables = {{"Rock","Beats Scissors"}, {"Paper", "Beats Rock"}, {"Scissors", "Beats Paper"}};
  return temp;
}

serverRequest demoParseReq(const std::string log){
  serverRequest temp;
  auto log_cp = log;
  int pos = log_cp.find(',');
  if(log_cp == " " || log_cp == ""){
    temp.request = "";
    temp.data ="";
    temp.gameInfo = {{"",""}};
    temp.gameName ="";
    temp.gameId = "";
    temp.gameVariables = {{"",""}};
    return temp;
  }
  if (pos != log_cp.npos){
    std::string req = log_cp.substr(0, pos);
    temp.request = req;
    temp.data = "";
    temp.gameInfo = {{"Rule1",""}, {"Rule2",""}};
    temp.gameName = "";
    temp.gameId = log_cp.substr(pos + 1);
    temp.gameVariables = {{"Rock","Beats Scissors"}, {"Paper", "Beats Rock"}, {"Scissors", "Beats Paper"}};
    return temp;
  }else{
    throw UnknownRequestException("Bad Request, could not parse");
  }
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

/*
bool evaluateFilledGame(std::map<std::string, std::string> &gameSpec, std::map<std::string, std:string> &receivedItems) {
  if (gameSpec.size() != receivedItems.size()) {
    return false;
  }
  for (const auto item : gameSpec) {
    if (receivedItems.find(item.first) == receivedItems.end()) {
      return false;
    }
  }
  return true;
}
*/

//function to instantiate game from server request
Game instantiateGame(serverRequest gameRequest, Player& gameHost) {
  Game newGame(stoi(gameRequest.gameId));
  for (auto variable: gameRequest.gameVariables) {
    GameVariable newVariable(variable.first, RandomIdGenerator::generateUniqueId(), variable.second); 
    newGame.AddVariable(variable.first, newVariable);
  }
  return newGame;
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
      result << message.text;
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

std::string handleRequest(const std::string& request,
                          GameList& serverGameList,
                          GameSessionList& sessionHandlerDB,
                          const std::map<std::string, std::string>& demoSessionHandlerDB) {
    std::istringstream iss(request);
    std::string requestType;
    iss >> requestType;

    if (requestType == "ReqCreateGame") {
        // Handle ReqCreateGame logic
        // Example: Assume the request format is "ReqCreateGame GameName"
        std::string gameName;
        iss >> gameName;

      
        // Create the game and add it to the serverGameList
        Game newGame(RandomIdGenerator::generateUniqueId());
        newGame.SetGameName(gameName);
        serverGameList.AddGame(newGame);

            // Return a success response
        return "Game created: " + gameName + ", GameID: " + std::to_string(newGame.GetGameId());

    } else if (requestType == "ReqJoinGame") {
        // Handle ReqJoinGame logic
        // Example: Assume the request format is "ReqJoinGame GameID PlayerName"
        int gameId;
        std::string playerName;
        iss >> gameId >> playerName;

        // Check if the game session exists
            // Add player to the game session
        GameSessionHandler handler = sessionHandlerDB.GetGameSessionHandler(std::to_string(gameId));
        Player player(playerName, RandomIdGenerator::generateUniqueId()); // Assuming Player constructor takes name and ID
        handler.AddPlayer(player.GetName(), player);

        // Return a success response
        return "Player " + playerName + " joined GameID: " + std::to_string(gameId);
    }
    // Add more cases for other request types as needed

    // If the request type is not recognized, return an error response
    return "Unknown request type: " + requestType;
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

  GameList serverGameList;
  GameSessionList sessionHandlerDB = GameSessionList();

  Game rockPaperScissors = Game(RandomIdGenerator::generateUniqueId());
  rockPaperScissors.SetGameName("Rock, Paper, Scissors");
  serverGameList.AddGame(rockPaperScissors);

  std::map<std::string, std::string> demoSessionHandlerDB = {{"Hi", "Rock,Paper,Scissors"}};

  while (true) {
    bool errorUpdating = false;
    try {
      server.update();
    }catch (std::exception& e) {
      std::cerr << "Exception from Server update:\n"
                << " " << e.what() << "\n\n";
      errorUpdating = true;
    }

    const auto incoming = server.receive();
    const auto [log, shouldQuit, returnAll] = processMessages(server, incoming);
    std::string server_response;
    allMessages.push_back(log+"\n");

    if (returnAll) {
      for (std::string str : allMessages) {
      auto outgoing = buildOutgoing(str);
      server.send(outgoing);
      }
    } else {      
      server_response = handleRequest(log, serverGameList, sessionHandlerDB, demoSessionHandlerDB);
      auto outgoing = buildOutgoing(server_response);
      server.send(outgoing);
    }

    if (shouldQuit || errorUpdating) {
      break;
    }

    sleep(1);
  }
  
  return 0;
}

