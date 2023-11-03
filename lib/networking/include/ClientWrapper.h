#pragma once

#include <string>
#include "../../constants/include/Constants.h"
#include "DataClasses.h"
#include "Client.h"


namespace networking{

class ClientWrapper
{
private:
    
public:
    void sendReq(constants::ReqType reqtype,  DataClass& body,Client& client )  ;
    void sendNoBody(constants::ReqType reqType, Client& client);
};


}