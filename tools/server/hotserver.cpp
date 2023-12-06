/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#include "Server.h"
#include "CustomExceptions.h"
#include "Player.h"
#include "User.h"
#include "Game.h"
#include "GameSessionHandler.h"
#include "GameVariable.h"
#include "AudienceMember.h"
#include "GameList.h"
#include "ParserLibrary.h"
#include "GameSessionList.h"
#include "RandomIdGenerator.h"
#include "ResponseQueue.h"
#include "ExecutionQueue.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <map>
#include <variant>


using networking::Server;
using networking::Connection;
using networking::Message;
using Json = nlohmann::json;


std::vector<Connection> clients;
std::vector<User> users;


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
  std::string connId;
};

// Struct for return messages from request handler
struct requestMessageResponse {
    std::string message;
    Connection client;
};
class RequestHandler {
  public:
  virtual requestMessageResponse process(serverRequest& request, GameList& serverGameList, GameSessionList& sessionHandlerDB) = 0;
};


// MESSAGE CONSTRUCTORS ====================================
// * display content
// * selection content
// * input content
// =========================================================
// constructs a server response with display content
std::string ConstructResponseForDisplayContent(std::string& description, std::string& buttonLabel, std::string& endpoint) {
  RequestConstructor constructor{"success", description, "display", "[]", "", buttonLabel, "", endpoint};
  return constructor.ConstructRequest();
}
// constructs a server response with selection content
std::string ConstructResponseForSelectionContent(std::string& prompt, Json& options, std::string& buttonLabel, std::string& field, std::string& endpoint) {
  RequestConstructor constructor{"success", "", "selection", options, prompt, buttonLabel, field, endpoint};
  return constructor.ConstructRequest();
}
// constructs a server response with input content
std::string ConstructResponseForInputContent(std::string& prompt, std::string& buttonLabel, std::string& field, std::string& endpoint) {
  RequestConstructor constructor{"success", "", "input", "[]", prompt, buttonLabel, field, endpoint};
  return constructor.ConstructRequest();
}

void registerNewUser(const std::string userName, int id){
  users.push_back(User(userName, id));
}

std::map<std::string, std::shared_ptr<RequestHandler>> requestHandlers;

std::string addConnId(const std::string &message_text, uintptr_t connId) {
  std::string strConnId = std::to_string(connId);
  JsonConverter converter;
  size_t equalsPos = message_text.find('=');
  std::string new_message;
  new_message.reserve(message_text.length() + strConnId.length());
  std::string afterEquals = (equalsPos != std::string::npos) ? message_text.substr(equalsPos + 1) : "";
  if (!afterEquals.empty())
  {
    std::string substringBeforeEqual = message_text.substr(0, equalsPos + 1);
    Json json_object = converter.GetJsonItem(afterEquals);
    json_object["ConnID"] = strConnId;
    if(json_object.find("misc") != json_object.end()){
      std::string clientUserName = json_object["misc"].dump();
      clientUserName.erase(std::remove(clientUserName.begin(), clientUserName.end(), '\"'), clientUserName.end());
      if (clientUserName != "null")
      {
        registerNewUser(clientUserName, connId);
      }
    }
    new_message = json_object.dump();

    return substringBeforeEqual.append(new_message);
  }
  else {
    RequestConstructor reqConstructor("ReqGetGamesList");
    reqConstructor.appendItem("ConnID", strConnId);
    auto json_string = reqConstructor.ConstructRequest();
    return message_text + " jsonData=" + json_string;
  }

}


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
  temp.connId = "";
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
    temp.connId = "";
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
      std::string newConnId = "";
      if (!afterEquals.empty())
      {
        Json json_object = converter.GetJsonItem(afterEquals);
        if(json_object.find("GameName") != json_object.end()){
            newGameName = json_object["GameName"].dump();
            newGameName.erase(std::remove(newGameName.begin(), newGameName.end(), '\"'), newGameName.end());
        }

        if(json_object.find("ConnID") != json_object.end()){
            newConnId = json_object["ConnID"].dump();
            newConnId.erase(std::remove(newConnId.begin(), newConnId.end(), '\"'), newConnId.end());
        }
      }
      
      std::string req = log_cp.substr(0, pos);
      temp.request = req;
      temp.data = "";
      temp.gameInfo = {{"Rule1",""}, {"Rule2",""}};
      temp.gameName = newGameName;
      temp.gameId = "";
      temp.gameVariables = {{"Rock","Beats Scissors"}, {"Paper", "Beats Rock"}, {"Scissors", "Beats Paper"}};
      temp.connId = newConnId;
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
  int userIdToRemove = c.id;
  users.erase(std::remove_if(users.begin(), users.end(),
                  [userIdToRemove](const User& user) {
                      return user.GetUserId() == userIdToRemove;
                  }),
                  users.end());
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
    if (message.text == "quit") {
      server.disconnect(message.connection);
    } else if (message.text == "shutdown") {
      std::cout << "Shutting down.\n";
      quit = true;
    }else if(message.text =="getall"){
      returnAll = true;

    } else {
      result << addConnId(message.text, message.connection.id);;
    }
    std::cout << "New message found: " << result.str() << "\n";
  }
  return MessageResult{result.str(), quit, returnAll};
}


std::deque<Message>
buildOutgoing(const std::string& logWithClient) {
  std::deque<Message> outgoing;
  Connection clientToSend;
  std::string log = logWithClient;

  size_t position = logWithClient.find("ResponseClient:");
  if (position != std::string::npos) {
      // Extract the substring that follows "ResponseClient:"
      std::string connectionID = logWithClient.substr(position + strlen("ResponseClient:"));
      log = logWithClient.substr(0, position);

      for (auto client : clients) {
        if (std::to_string(client.id) == connectionID)
        {
          clientToSend = client;
          break;
        }
      }

      outgoing.push_back({clientToSend, log});
  } else {
      //std::cout << "String does not contain 'ResponseClient:'" << std::endl;
      // TODO: Handle error here
      Connection errorClient;
      errorClient.id = 00000;
      clientToSend = errorClient;
  }

  return outgoing;
}

std::deque<Message>
buildOutgoingToAll(const std::string& log) {
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



requestMessageResponse setResponseClient(std::string& connectionId) {
    // Create a success response and set the client
    requestMessageResponse response;
    response.message = "";

    for (auto client : clients) {
      if (std::to_string(client.id) == connectionId)
      {
        response.client = client;
        break;
      }
    }

    return response;
}


// Request Handlers ================================================
//   REQUEST_TYPE         -> HANDLER_NAME
// -----------------------------------------------------------------
//   ReqCreateGame        -> CreateGameHandler
//   ReqCreateGameFilled  -> CreateFilledGameHandler
//   ReqJoinGame          -> JoinGameHandler
//   ReqViewGame          -> ViewGameHandler
//   ReqUpdateGame        -> UpdateGameHandler
//   ReqUpdatePlayer      -> UpdatePlayerHandler
//   ReqGetGamesList      -> GetGamesListHandler
//   ReqGetGame           -> GetGameHandler
// =================================================================
class CreateGameHandler : public RequestHandler {
  public:
  requestMessageResponse process(serverRequest& request, GameList& serverGameList, GameSessionList& sessionHandlerDB) override {
    // Example: Assume the request format is "ReqCreateGame GameName"
    std::cout << "ReqCreateGame" << std::endl;

    Game newGame(RandomIdGenerator::generateUniqueId());
    newGame.SetGameName(request.gameName);
    serverGameList.AddGame(newGame);

    // Return a success response
    requestMessageResponse response = setResponseClient(request.connId);
    response.message = "Game created: " + request.gameName + ", GameID: " + std::to_string(newGame.GetGameId());

    return response;
  }
};
class CreateFilledGameHandler : public RequestHandler {
  public:
  requestMessageResponse process(serverRequest& request, GameList& serverGameList, GameSessionList& sessionHandlerDB) override {
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

    // Return a success response
    requestMessageResponse response = setResponseClient(request.connId);
    response.message = server_response;

    return response;
  }
};
class JoinGameHandler : public RequestHandler {
  public:
  requestMessageResponse process(serverRequest& request, GameList& serverGameList, GameSessionList& sessionHandlerDB) override {
    // Handle ReqJoinGame logic
    if (sessionHandlerDB.DoesSessionExist(request.gameId)){
      auto handler = sessionHandlerDB.GetGameSessionHandler(request.gameId);
      Player player("dummy_player", 1);
      handler.AddPlayer(player.GetName(), player);
      //Construct response
      std::string server_status = "ReqJoinGame Successful" + '\n' + player.GetName() + " added into " + std::to_string(handler.GetGame().GetGameId());

      // Return a success response
      requestMessageResponse response = setResponseClient(request.connId);
      response.message = server_status;

      return response; 
    }else{
      throw UnknownGameException("Game not found: " + request.gameName);
      // TODO handle error logic
      requestMessageResponse response;
      response.message = "Error with request";
      Connection errorClient;
      errorClient.id = 00000;
      response.client = errorClient;
      return response;
    }
  }
};
class ViewGameHandler : public RequestHandler {
  public:
  requestMessageResponse process(serverRequest& request, GameList& serverGameList, GameSessionList& sessionHandlerDB) override {
    std::string server_response = "";
    if (sessionHandlerDB.DoesSessionExist(request.gameId)){
      int newAudienceId = RandomIdGenerator::generateUniqueId();
      AudienceMember member("dummy_viewer", newAudienceId); 
      auto handler = sessionHandlerDB.GetGameSessionHandler(request.gameId);
      handler.AddAudienceMember(member.GetName(), member);
      auto status = "ReqViewGame Successful" + '\n' + member.GetName() + " added as an audience member for " + std::to_string(handler.GetGame().GetGameId());

      server_response = status;
    }else{
      auto status = std::string("ReqViewGame Unsuccessful") + "\nGame with ID \"" + request.gameId + "\" does not exist!";
      server_response = status;
    }

    // Return a success response
    requestMessageResponse response = setResponseClient(request.connId);
    response.message = server_response;

    return response;
  }
};
class UpdateGameHandler : public RequestHandler {
  public:
  requestMessageResponse process(serverRequest& request, GameList& serverGameList, GameSessionList& sessionHandlerDB) override {
    std::cout<< "ReqUpdateGame" << std::endl;
    std::string id = request.gameId;
    std::string server_response = "";
    if (sessionHandlerDB.DoesSessionExist(id)){
      // Get variable from request
      // auto variableName = request.gameVariables.find("Paper");
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

    // Return a success response
    requestMessageResponse response = setResponseClient(request.connId);
    response.message = server_response;

    return response;
  }
};
class UpdatePlayerHandler : public RequestHandler {
  public:
  requestMessageResponse process(serverRequest& request, GameList& serverGameList, GameSessionList& sessionHandlerDB) override {
    std::cout << "ReqUpdatePlayer" << std::endl;
    auto id = request.gameId;
    std::string server_response = "";
    if (sessionHandlerDB.DoesSessionExist(id)){
      // TODO: Evaluate if we need to update the player state
      server_response = "Update player still needs to be implemented";
    }else{
      throw UnknownGameException("Game not found: " + request.gameName);
      server_response = "Error with request";
    }

    // Return a success response
    requestMessageResponse response = setResponseClient(request.connId);
    response.message = server_response;

    return response;
  }
};
class GetGamesListHandler : public RequestHandler {
  public:
  requestMessageResponse process(serverRequest& request, GameList& serverGameList, GameSessionList& sessionHandlerDB) override {
    std::cout << "Got: ReqGetGamesList" << std::endl;
    auto gamesList = serverGameList.GetGameList();
    std::string server_response = "";

    Json games = Json::array();
    for(const Game& game: gamesList) {
      games.push_back(game.GetGameName());
    }

    std::string prompt = "Select the game you want to play";
    std::string buttonLabel = "Confirm";
    std::string field = "GameName";
    std::string endpoint = "ReqGetGame";
    server_response = ConstructResponseForSelectionContent(prompt, games, buttonLabel, field, endpoint);

    std::cout << "Server Response: " + server_response << std::endl;

    // Return a success response
    requestMessageResponse response = setResponseClient(request.connId);
    response.message = server_response;

    return response;
  }
};
class GetGameHandler : public RequestHandler {
  public:
  requestMessageResponse process(serverRequest& request, GameList& serverGameList, GameSessionList& sessionHandlerDB) override {
    std::cout << "Got: ReqGetGame and game name is: " << request.gameName << std::endl;
    auto foundGame = serverGameList.GetGameSpec(request.gameName);

    std::string server_response = "";
    if( foundGame.GetGameId() != 0){
      std::string prompt = "Select the number of rounds.";
      std::string buttonLabel = "Confirm";
      std::string field = "NumRounds";
      std::string endpoint = "ReqCreateGame";
      server_response = ConstructResponseForInputContent(prompt, buttonLabel, field, endpoint);
    }else{
      std::string description = "Request ReqGetGame failed: No such game";
      std::string buttonLabel = "get games list";
      std::string endpoint = "ReqGetGamesList";
      server_response = ConstructResponseForDisplayContent(description, buttonLabel, endpoint);
    }

    std::cout << "Server Response: " + server_response << std::endl;
    
    // Return a success response
    requestMessageResponse response = setResponseClient(request.connId);
    response.message = server_response;

    return response;
  }
};

requestMessageResponse handleRequest(serverRequest& request,
                          GameList& serverGameList,
                          GameSessionList& sessionHandlerDB,
                          const std::map<std::string, std::string>& demoSessionHandlerDB) {
    
    std::cout << "handleRequest\n";
    std::string requestType = request.request;
    auto it = requestHandlers.find(requestType);

    if(it == requestHandlers.end()) {
      std::cout << "Bad Request: " + requestType << std::endl;
      throw UnknownRequestException("Unknown Request: " + requestType);
      requestMessageResponse error_response;
      //TODO: Handle error here
      error_response.message = "Unknown request type: " + requestType;
      Connection errorClient;
      errorClient.id = 00000;
      error_response.client = errorClient;
      return error_response;
    }

    auto handler = it->second;
    requestMessageResponse server_response = handler->process(request, serverGameList, sessionHandlerDB);
    return server_response;
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

  // add requests that server can handle
  requestHandlers["ReqCreateGame"] = std::make_shared<CreateGameHandler>();
  requestHandlers["ReqCreateGameFilled"] = std::make_shared<CreateFilledGameHandler>();
  requestHandlers["ReqJoinGame"] = std::make_shared<JoinGameHandler>();
  requestHandlers["ReqViewGame"] = std::make_shared<ViewGameHandler>();
  requestHandlers["ReqUpdateGame"] = std::make_shared<UpdateGameHandler>();
  requestHandlers["ReqUpdatePlayer"] = std::make_shared<UpdatePlayerHandler>();
  requestHandlers["ReqGetGamesList"] = std::make_shared<GetGamesListHandler>();
  requestHandlers["ReqGetGame"] = std::make_shared<GetGameHandler>();


  GameList serverGameList = GameList();
  GameSessionList sessionHandlerDB = GameSessionList();

  Game rockPaperScissors = Game(RandomIdGenerator::generateUniqueId());
  rockPaperScissors.SetGameName("Rock,Paper,Scissors");
  serverGameList.AddGame(rockPaperScissors);

  std::vector<std::string> fakeServerGameList = {"Rock,Paper,Scissors"};
  std::map<std::string, std::string> fakeGameRules = {{"Rock,Paper,Scissors", "Rules:None"}};

  std::map<std::string, std::string> demoSessionHandlerDB = {{"Hi", "Rock,Paper,Scissors"}};

  ResponseQueue messageQueue;
  ExecutionQueue excNodeQueue;

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
        auto outgoing = buildOutgoingToAll(str);
        server.send(outgoing);
      }
    }else {
      // Space to parse log file into specific sections: request, gameInfo, data, etc
      serverRequest request = demoParseReq(log);
      // Begin logic implementation
      try {
        if (request.request == " " || request.request == ""){
          std::cout << "No message from client" << std::endl;
          messageQueue.push(log);
        } else {
          // TODO: Make the messageQueue take in a struct of the message string and a client Connection
          requestMessageResponse response = handleRequest(request, serverGameList, sessionHandlerDB, demoSessionHandlerDB);
          std::string responseMessage = response.message.append("ResponseClient:" + std::to_string(response.client.id));
          messageQueue.push(responseMessage);
        }
      } catch (const UnknownGameException& e) {
        std::cerr << "UnknownGameException caught" << std::endl;
        messageQueue.push("Game not found");
      } catch (const UnknownRequestException& e) {
        std::cerr << "UnknownRequestException caught" << std::endl;
        messageQueue.push("Request Failed");
      }
      // Add Responses Queue Here
      auto outgoing = buildOutgoing(messageQueue.pop());
      server.send(outgoing);
    }

    if (shouldQuit || errorUpdating) {
      break;
    }

    sleep(1);
  }
  
  return 0;
}

