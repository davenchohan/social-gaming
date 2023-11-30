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
#include "parser_test.h"
#include "rule_interpreter.h"


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


class ServerNetworkManager {
    public:
    static void send(Server& server, const std::string& message, std::vector<Connection>& receivers) {
      std::deque<Message> outgoing;
      for (auto receiver : receivers) {
          outgoing.push_back({receiver, message});
      }
      server.send(outgoing);
    }
};


std::vector<Connection> clients;
std::vector<Player> players;
std::unordered_map<std::string, std::string> games;

void loadSupportedGames() {
  games["Rock,Paper,Scissors"] = "./lib/gameSpecs/rock_paper_scissors.txt";
}
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
  try
  {
    if (pos != log_cp.npos){
      JsonConverter converter;
      size_t equalsPos = log_cp.find('=');
      // Extract the string after the equals sign
      std::string afterEquals = (equalsPos != std::string::npos) ? log_cp.substr(equalsPos + 1) : "";
      std::string newGameName = "";
      if (!afterEquals.empty())
      {
        Json json_object = converter.GetJsonItem(afterEquals);
        if(json_object.find("GameName") != json_object.end()){
            newGameName = json_object["GameName"].dump();
            newGameName.erase(std::remove(newGameName.begin(), newGameName.end(), '\"'), newGameName.end());
        }
      }
      
      std::string req = log_cp.substr(0, pos);
      temp.request = req;
      temp.data = "";
      temp.gameInfo = {{"Rule1",""}, {"Rule2",""}};
      temp.gameName = newGameName;
      temp.gameId = "";
      temp.gameVariables = {{"Rock","Beats Scissors"}, {"Paper", "Beats Rock"}, {"Scissors", "Beats Paper"}};
      return temp;
    }else{
      std::cout << "Error with request" << std::endl;
      throw UnknownRequestException("Bad Request, could not parse");
    }
  }
  catch (const UnknownRequestException& e) {
    std::cerr << "UnknownRequestException caught" << std::endl;
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

void registerNewPlayer(int id){
  std::string name = "player";
  name.append(std::to_string(id));
  players.push_back(Player(name, id));
}

void
onConnect(Connection c) {
  std::cout << "New connection found: " << c.id << "\n";
  clients.push_back(c);
  int newId = c.id;
  registerNewPlayer(newId);
}


void
onDisconnect(Connection c) {
  std::cout << "Connection lost: " << c.id << "\n";
  auto eraseBegin = std::remove(std::begin(clients), std::end(clients), c);
  clients.erase(eraseBegin, std::end(clients));
  int playerIdToRemove = c.id;
  players.erase(std::remove_if(players.begin(), players.end(),
                  [playerIdToRemove](const Player& player) {
                      return player.GetUserId() == playerIdToRemove;
                  }),
                  players.end());
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

std::string createNewGame(const std::string& gameName,
                          serverRequest& request,
                          GameList& serverGameList){
  std::cout << "ReqCreateGame" << std::endl;
  // Handle ReqCreateGame logic
  // Example: Assume the request format is "ReqCreateGame GameName"

  // Create the game and add it to the serverGameList
  Game newGame(RandomIdGenerator::generateUniqueId());
  newGame.SetGameName(gameName);
  serverGameList.AddGame(newGame);

      // Return a success response
  return "Game created: " + gameName + ", GameID: " + std::to_string(newGame.GetGameId());
}

std::string createFilledGame(const std::string& gameName,
                          serverRequest& request,
                          GameSessionList& sessionHandlerDB){
    std::cout << "ReqCreateGameFilled" << std::endl;
    std::map<std::string, std::string> fakeGameSpec = {{"players","3-10"}, {"Rounds","3"}};
    evaluateFilledGame(fakeGameSpec, request.gameInfo);
    std::string server_response = "";
    // Instantiate Game class
    auto fakeGenerate = [](){
      return 404;
    };
    // Instantiate host
    Player host("dummy_host", 0);
    // Instantiate Game
    Game newGame(fakeGenerate());
    // Set Game variables
    // TODO: Implement a way to parse game variables from server request
    // May need loop to add all variables into the game, for now just a single statement
    // TODO: replace "Rock" with game variable
    // Game variables should be defined in serverGameList
    auto variable = request.gameVariables.find("Rock");
    std::string varName = variable->first;
    std::string varVal = variable->second;
    GameVariable someVar(varName, RandomIdGenerator::generateUniqueId(), varVal);
    newGame.AddVariable(varName, someVar);

    // Add Game to session handler
    GameSessionHandler sessionHandler(newGame.GetGameId(), newGame, host);
    // Add session handler to DB
    sessionHandlerDB.AddGameSessionHandler(std::to_string(newGame.GetGameId()), sessionHandler);
    // Construct response
    std::string server_status = "ReqCreateGameFilled Successful" + '\n' + newGame.GetGameName() + " created, GameID: " + std::to_string(newGame.GetGameId());
    server_response = server_status;
    return server_response;
}

std::string joinGame(const std::string& id,
                      serverRequest& request,
                      GameSessionList& sessionHandlerDB){
  // Handle ReqJoinGame logic
  if (sessionHandlerDB.DoesSessionExist(id)){
    auto handler = sessionHandlerDB.GetGameSessionHandler(id);
    Player player("dummy_player", 1);
    handler.AddPlayer(player.GetName(), player);
    //Construct response
    std::string server_status = "ReqJoinGame Successful" + '\n' + player.GetName() + " added into " + std::to_string(handler.GetGame().GetGameId());
    return server_status; 
  }else{
    throw UnknownGameException("Game not found: " + request.gameName);
    return "Error with request";
  }
}

std::string viewGame(const std::string& id,
                      serverRequest& request,
                      GameSessionList& sessionHandlerDB){
  std::string server_response = "";
  if (sessionHandlerDB.DoesSessionExist(id)){
    int newAudienceId = RandomIdGenerator::generateUniqueId();
    AudienceMember member("dummy_viewer", newAudienceId); 
    auto handler = sessionHandlerDB.GetGameSessionHandler(id);
    handler.AddAudienceMember(member.GetName(), member);
    auto status = "ReqViewGame Successful" + '\n' + member.GetName() + " added as an audience member for " + std::to_string(handler.GetGame().GetGameId());

    server_response = status;
  }else{
    auto status = std::string("ReqViewGame Unsuccessful") + "\nGame with ID \"" + id + "\" does not exist!";
    server_response = status;
  }
  return server_response;
}

std::string updateGame(serverRequest& request,
                      GameSessionList& sessionHandlerDB){
  std::cout<< "ReqUpdateGame" << std::endl;
  std::string id = request.gameId;
  std::string server_response = "";
  if (sessionHandlerDB.DoesSessionExist(id)){
    // Get variable from request
    auto variableName = request.gameVariables.find("Paper");
    auto variable = request.gameVariables.find("Paper");
    std::string varName = variable->first;
    std::string varVal = variable->second;
    // Create new variable
    GameVariable someVar(varName, RandomIdGenerator::generateUniqueId(), varVal);
    // Set updated variable
    auto handler = sessionHandlerDB.GetGameSessionHandler(id);
    handler.GetGame().AddVariable(varName, someVar);

    // Construct Response
    server_response = "ReqUpdateGame Successful" + '\n' + varName + " was updated with value: " + varVal + ", in game: " + handler.GetGame().GetGameName();
  }else{
    throw UnknownGameException("Game not found: " + request.gameName);
    server_response = "Error with request";
  }
  return server_response;
}

std::string updatePlayer(serverRequest& request,
                      GameSessionList& sessionHandlerDB){
  std::cout << "ReqUpdatePlayer" << std::endl;
  auto id = request.gameId;
  std::string server_response = "";
  if (sessionHandlerDB.DoesSessionExist(id)){
    // TODO: Evaluate if we need to update the player state
    return "Update player still needs to be implemented";
  }else{
    throw UnknownGameException("Game not found: " + request.gameName);
    server_response = "Error with request";
  }
  return server_response;
}

std::string getGamesList(serverRequest& request,
                      GameList& serverGameList){
    std::cout << "Got: ReqGetGamesList" << std::endl;
    auto gamesList = serverGameList.GetGameList();
    std::string server_response = "";

    std::string concatenatedNames = std::accumulate(gamesList.begin(), gamesList.end(), std::string(),
                                                      [](std::string& accumulated, const Game& game) {
                                                          if (!accumulated.empty()) {
                                                              accumulated += ", ";
                                                          }
                                                          return accumulated += "'" + game.GetGameName() + "'";
                                                      });

    std::string final_response = "ReqGetGamesList Successful\n";
    server_response = final_response + "jsonObject={'gamesList':'[" + concatenatedNames + "]'}";
    std::cout << "Server Response: " + server_response << std::endl;
    return server_response;
}

std::string getGame(serverRequest& request,
                      GameList& serverGameList){
  std::cout << "Got: ReqGetGame and game name is: " << request.gameName << std::endl;

  auto foundGame = serverGameList.GetGameSpec(request.gameName);
  std::string server_response = "";


  if(games.find(request.gameName) == games.end()) {
    std::cout << "Server not loaded with game named: " << request.gameName << std::endl;
    return "ReqGetGame Failure: No such game\n";
  }

  SGParser p(games[request.gameName]);
  server_response = "ReqGetGame Success\njsonObject=" + p.setupToJson().dump();
  return server_response;

  
  // if( foundGame.GetGameId() != 0){
  //   std::string final_response = "ReqGetGame Success\n";
  //   server_response = final_response + "jsonObject={'game':" + "'" + foundGame.GetGameName() + "'" + "}";
  // }else{
  //   server_response = "ReqGetGame Failure: No such game\n";
  // }
  // std::cout << server_response << std::endl;
  // return server_response;
}

std::string handleRequest(serverRequest& request,
                          GameList& serverGameList,
                          GameSessionList& sessionHandlerDB,
                          const std::map<std::string, std::string>& demoSessionHandlerDB) {

    std::string requestType = request.request;
    std::string gameId = request.gameId;
    std::string gameName = request.gameName;

    if (requestType == "ReqCreateGame") {
        std::string server_response = createNewGame(gameName, request, serverGameList);
        return server_response;
    } else if (requestType == "ReqJoinGame") {
        std::string server_response = joinGame(gameId, request, sessionHandlerDB);
        return server_response;
    }
    else if (requestType == "ReqViewGame") {
        std::string server_response = viewGame(gameId, request, sessionHandlerDB);
        return server_response;
    }
    else if (requestType == "ReqUpdateGame") {
      std::string server_response = updateGame(request, sessionHandlerDB);
      return server_response;
    }
    else if (requestType == "ReqUpdateGame") {
      std::string server_response = updateGame(request, sessionHandlerDB);
      return server_response;
    }
    else if (requestType == "ReqUpdateGame") {
      std::string server_response = updateGame(request, sessionHandlerDB);
      return server_response;
    }
    else if (requestType == "ReqUpdatePlayer") {
      std::string server_response = updatePlayer(request, sessionHandlerDB);
      return server_response;
    }
    else if (requestType == "ReqGetGamesList") {
      std::string server_response = getGamesList(request, serverGameList);
      return server_response;
    }
    else if (requestType == "ReqGetGame") {
      std::string server_response = getGame(request, serverGameList);
      return server_response;
    }   
    else if (requestType == "ReqCreateGameFilled") {
      std::string server_response = createFilledGame(gameName, request, sessionHandlerDB);
      return server_response;
    } else {
      std::cout << "Bad Request: " + request.request << std::endl;
      throw UnknownRequestException("Unknown Request: " + request.request);
      // If the request type is not recognized, return an error response
      return "Unknown request type: " + requestType;
    }

    // Add more cases for other request types as needed

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

  // load server with games
  loadSupportedGames();
  GameList serverGameList = GameList();
  GameSessionList sessionHandlerDB = GameSessionList();

  Game rockPaperScissors = Game(RandomIdGenerator::generateUniqueId());
  rockPaperScissors.SetGameName("Rock,Paper,Scissors");
  serverGameList.AddGame(rockPaperScissors);

  std::vector<std::string> fakeServerGameList = {"Rock,Paper,Scissors"};
  std::map<std::string, std::string> fakeGameRules = {{"Rock,Paper,Scissors", "Rules:None"}};

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
      for (std::string msg : allMessages) {
        // auto outgoing = buildOutgoing(msg);
        // server.send(outgoing);
        ServerNetworkManager::send(server, msg, clients);
      }
    }else {
      // Space to parse log file into specific sections: request, gameInfo, data, etc
      serverRequest request = demoParseReq(log);
      // Begin logic implementation
      try {
        if (request.request == " " || request.request == ""){
          std::cout << "No message from client" << std::endl;
          server_response = log;
        } else {
          server_response = handleRequest(request, serverGameList, sessionHandlerDB, demoSessionHandlerDB);
        }
      } catch (const UnknownGameException& e) {
        std::cerr << "UnknownGameException caught" << std::endl;
        server_response = "Game not found";
      } catch (const UnknownRequestException& e) {
        std::cerr << "UnknownRequestException caught" << std::endl;
        server_response = "Request Failed";
      }
      // auto outgoing = buildOutgoing(server_response);
      // server.send(outgoing);
      ServerNetworkManager::send(server, server_response, clients);
    }

    if (shouldQuit || errorUpdating) {
      break;
    }

    sleep(1);
  }
  
  return 0;
}

