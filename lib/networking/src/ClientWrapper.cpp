#include <string>
#include "../../constants/include/Constants.h"
#include "DataClasses.h"
#include "Client.h"
#include "ClientWrapper.h"
#include <string_view>


namespace networking {



void ClientWrapper::sendReq(constants::ReqType reqType, DataClass& body, Client& client) {
    std::string jsonBody = body.toJSON();
    std::string_view reqHeader = to_string(reqType);
    client.send(std::string(reqHeader)+", jsonData=" +jsonBody);
    
}
void ClientWrapper::sendReq(std::string reqType, DataClass& body, Client& client) {
    std::string jsonBody = body.toJSON();
    client.send(reqType+", jsonData=" +jsonBody);
}
void ClientWrapper::sendNoBody(constants::ReqType reqType, Client& client){
    std::string_view reqHeader = to_string(reqType);
    client.send(std::string(reqHeader)+",");
}

} // namespace networking