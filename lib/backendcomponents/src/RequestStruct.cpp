#include "RequestStruct.h"

void setRequest(ServerRequest someReq, std::string req){
    someReq.request = std::move(req);
    return;
}