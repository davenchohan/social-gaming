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

  // Instantiate game list
  std::vector<std::string> fakeServerGameList = {"Rock,Paper,Scissors"};
  std::map<std::string, std::string> fakeGameRules = {{"Rock,Paper,Scissors", "Rules:None"}};
  std::map<std::string, GameSessionHandler> sessionHandlerDB;
  std::map<std::string, std::string> demoSessionHandlerDB = {{"Hi","Rock,Paper,Scissors"}};

  RequestInfo someInfo;

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
      serverRequest request = demoParseReq(log);
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
        // Instantiate host
        Player host("dummy_host", 0);
        // Instantiate Game
        Game newGame(fakeGenerate(), host);
        // Set Game variables
        // TODO: Implement a way to parse game variables from server request
        // May need loop to add all variables into the game, for now just a single statement
        // TODO: replace "Rock" with game variable
        // Game variables should be defined in serverGameList
        auto variable = request.gameVariables.find("Rock");
        std::string varName = variable->first;
        std::string varVal = variable->second;
        GameVariable someVar(varName, varVal);
        newGame.AddVariable(varName, someVar);

        // Add Game to session handler
        GameSessionHandler sessionHandler(newGame.GetGameId(), newGame);
        // Add session handler to DB
        sessionHandlerDB.insert(std::pair<std::string, GameSessionHandler> {std::to_string(newGame.GetGameId()), sessionHandler} );
        // Construct response
        std::string server_status = "ReqCreateGameFilled Successful" + '\n' + newGame.GetGameName() + " created, GameID: " + std::to_string(newGame.GetGameId());
        server_response = server_status;
        

      }else if(request.request == "ReqJoinGame"){
        std::cout << "ReqJoinGame" << std::endl;
        // TODO: Replace find 
        // Search gameSessionDB for the gameId given by request
        std::string id = request.gameId;
        if (auto sessionIt = sessionHandlerDB.find(id); sessionIt != sessionHandlerDB.end()){
          Player player("dummy_player", 1);
          auto handler = sessionIt->second;
          handler.AddPlayer(player.GetName(), player);
          //Construct response
          std::string server_status = "ReqJoinGame Successful" + '\n' + player.GetName() + " added into " + std::to_string(handler.GetGame().GetGameId());
          server_response = server_status; 
        }else{
          throw UnknownGameException("Game not found: " + request.gameName);
        }
      }else if(request.request == "ReqViewGame"){
        std::string id = request.gameId;
        if (auto sessionIt = sessionHandlerDB.find(id); sessionIt != sessionHandlerDB.end()){
          AudienceMember member("dummy_viewer", 0);
          auto  handler = sessionIt->second;
          handler.AddAudienceMember(member.GetName(), member);

          auto status = "ReqViewGame Successful" + '\n' + member.GetName() + " added as an audience member for " + std::to_string(handler.GetGame().GetGameId());
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
          GameVariable someVar(varName, varVal);
          // Set updated variable
          sessionIt->second.GetGame().AddVariable(varName, someVar);

          // Construct Response
          server_response = "ReqUpdateGame Successful" + '\n' + varName + " was updated with value: " + varVal + ", in game: " + sessionIt->second.GetGame().GetGameName();
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
        // TODO: Remove once communication format is implemented
        std::cout << "Got: DemoReqGetGamesList" << std::endl;
        std::string list_str = "";
        // Stringify vector, bad implementation
        std::for_each(fakeServerGameList.begin(), fakeServerGameList.end(), [&list_str, &fakeServerGameList](std::string &item){
          std::string builder = "'" + item + "'";
          list_str = list_str + builder;
          if (item == fakeServerGameList.back()){
            return;
          }
          list_str = list_str + ",";
        });
        std::string final_response = "Req DemoReqGetGamesList Successful\n";
        server_response = final_response + "jsonObject={'gamesList':'[" + list_str + "]'}";
        std::cout << "Server Response: " + server_response << std::endl;
      }else if(request.request == "DemoReqGetGame"){
        // TODO: Remove once communication format is implemented
        std::cout << "Got: DemoReqGetGame" << std::endl;
        auto it = demoSessionHandlerDB.find(request.gameId);
        if( it!= demoSessionHandlerDB.end()){
          std::string final_response = "DemoReqGetGame Success\n";
          server_response = final_response + "jsonObject={'game':" + "'" + it->second + "'" + "}";
        }else{
          server_response = "DemoReqGetGame Failure: No such game\n";
        }
        std::cout << server_response << std::endl;
      }else{
        std::cout << "Bad Request: " + request.request << std::endl;
        throw UnknownRequestException("Unknown Request: " + request.request);
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

