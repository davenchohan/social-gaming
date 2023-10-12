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
};

struct FakeGameVariable{
  std::string variableName;
  std::string variableValue;

  FakeGameVariable(std::string &name, std::string &value)
    : variableName{name}, variableValue{value}
      {}
  std::string GetVariableValue() const {
    return variableValue;
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
  std::vector<FakeGameVariable> variables;
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
  void SetVariable(const std::string &variableName, const FakeGameVariable &variable){
    variables.push_back(variable);
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
    void addAudienceMember(std::string name, FakeAudienceMember &member){

    }
};

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


// Demo function to "fake" parsing a request
// Note: This function uses strtok, which should be changed for the real implementation of parseRequest
serverRequest demoParseRequest(std::string log){
  serverRequest temp;
  int pos = log.find(',');
  if(pos != log.npos){
    string req = log.substr(0, pos);
    temp.request = req;
    temp.data = "";
    temp.gameInfo = {{"Rule1",""}, {"Rule2",""}};
    temp.gameName = "";
    temp.gameId = "1234556";
    temp.gameVariables = {{"Rock","Beats Scissors"}, {"Paper", "Beats Rock"}, {"Scissors", "Beats Paper"}};
    return temp;
  }else if(log == " " || log == " "){
    temp.request = "blank request";
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
  std::vector<std::string> fakeServerGameList = {"Rock,Paper,Scissors"};
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
      serverRequest demoRequest = demoParseRequest(log);
      // Begin logic implementation
      if (request.request == " " || request.request == ""){
        std::cout << "No message from client" << std::endl;
        server_response = log;
      }else if (request.request == "ReqCreateGame"){
        std::cout << "ReqCreateGame" << std::endl;
        // TODO: Replace find games list with GameList.GetGameSpec(gameName)
        if (std::find(fakeServerGameList.begin(), fakeServerGameList.end(), request.gameName) != fakeServerGameList.end()){
          auto iterator = fakeGameRules.find(request.gameName);
          const std::string rules_template = iterator->second;
          // TODO: Parse game rules + game variables into a format for client
          server_response = rules_template;
        }else{
          throw UnknownGameException("Game not found: " + request.gameName);
        }
      }else if (request.request == "ReqCreateGameFilled"){
        std::cout << "ReqCreateGameFilled" << std::endl;
        std::map<std::string, std::string> fakeGameSpec = {{"players","3-10"}, {"Rounds","3"}};
        evaluateFilledGame(fakeGameSpec, request.gameInfo);
        // Instantiate Game class
        auto fakeGenerate = [](){
          return 404;
        };
        // Instantiate Game
        FakeGame fakeGame(fakeGenerate(), host);
        // Set Game variables
        // TODO: Implement a way to parse game variables from server request
        // May need loop to add all variables into the game, for now just a single statement
        // TODO: replace "Rock" with game variable
        // Game variables should be defined in serverGameList
        auto variable = request.gameVariables.find("Rock");
        std::string varName = variable->first;
        std::string varVal = variable->second;
        FakeGameVariable someVar(varName, varVal);
        fakeGame.SetVariable(varName, someVar);

        // Add Game to session handler
        FakeGameSessionHandler sessionHandler(fakeGame.gameId, fakeGame);
        // Add session handler to DB
        sessionHandlerDB.insert(sessionHandlerDB.end(), std::pair<std::string, FakeGameSessionHandler> {std::to_string(fakeGame.gameId), sessionHandler} );
        // Construct response
        std::string server_status = "ReqCreateGameFilled Successful" + '\n' + fakeGame.gameName + " created, GameID: " + std::to_string(fakeGame.gameId);
        server_response = server_status;
        

      }else if(request.request == "ReqJoinGame"){
        std::cout << "ReqJoinGame" << std::endl;
        // TODO: Replace find 
        // Search gameSessionDB for the gameId given by request
        std::string id = request.gameId;
        if (auto sessionIt = sessionHandlerDB.find(id); sessionIt != sessionHandlerDB.end()){
          FakePlayer player("dummy_player");
          auto handler = sessionIt->second;
          handler.addPlayer(player.name, player);
          //Construct response
          std::string server_status = "ReqJoinGame Successful" + '\n' + player.name + " added into " + std::to_string(handler.game.gameId);
          server_response = server_status; 
        }else{
          throw UnknownGameException("Game not found: " + request.gameName);
        }
      }else if(request.request == "ReqViewGame"){
        std::string id = request.gameId;
        if (auto sessionIt = sessionHandlerDB.find(id); sessionIt != sessionHandlerDB.end()){
          FakeAudienceMember member("dummy_viewer");
          auto  handler = sessionIt->second;
          handler.addAudienceMember(member.name, member);

          auto status = "ReqViewGame Successful" + '\n' + member.name + " added as an audience member for " + std::to_string(handler.game.gameId);
          // TODO: Implement support for sending over list of audience members
          server_response = status;
        }else{
          throw UnknownGameException("Game not found: " + request.gameName);
        }
      }else if(request.request == "ReqUpdateGame"){
        std::cout<< "ReqUpdateGame" << std::endl;
        std::string id = request.gameId;
        if (auto sessionIt = sessionHandlerDB.find(id); sessionIt != sessionHandlerDB.end()){
          // TODO: Implement a cleaner way to update a game variable
          // Get variable from request
          auto variableName = request.gameVariables.find("Paper");
          auto variable = request.gameVariables.find("Paper");
          std::string varName = variable->first;
          std::string varVal = variable->second;
          // Create new variable
          FakeGameVariable someVar(varName, varVal);
          // Set updated variable
          sessionIt->second.game.SetVariable(varName, someVar);

          // Construct Response
          server_response = "ReqUpdateGame Successful" + '\n' + varName + " was updated with value: " + varVal + ", in game: " + sessionIt->second.game.gameName;
        }else{
          throw UnknownGameException("Game not found: " + request.gameName);
        }
      }else if (request.request == "ReqUpdatePlayer"){
        std::cout << "ReqUpdatePlayer" << std::endl;
        auto id = request.gameId;
        if (auto sessionIt = sessionHandlerDB.find(id); sessionIt != sessionHandlerDB.end()){
          // TODO: Evaluate if we need to update the player state
        }else{
          throw UnknownGameException("Game not found: " + request.gameName);
        }
      }else if(request.request == "DemoReqGetGamesList"){
      }else{
        throw UnknownRequestException("Unknown Request: " + log);
      }
      auto outgoing = buildOutgoing(server_response);
      server.send(outgoing);
    }
    

    if (shouldQuit || errorWhileUpdating) {
      break;
    }

    sleep(1);
  }

  return 0;
}

