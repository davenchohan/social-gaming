#pragma once
#include <algorithm>
#include <iterator>
#include <ostream>
#include <string>
#include <vector>

namespace constants
{
enum class ReqType : char {
  GETGAMES, JOINGAME, CREATEGAME, VIEWGAME, UPDATEGAME, DEMOGETGAMES, DEMOGETGAME,CREATEGAMEFILLED, UPDATEPLAYER
};

constexpr inline std::string_view
to_string(const constants::ReqType request) noexcept {
  switch (request) {
    case constants::ReqType::GETGAMES:   return "ReqGetGame"; // this doesn't exist in the backend right now
    case constants::ReqType::JOINGAME: return "ReqJoinGame";
    case constants::ReqType::CREATEGAME: return "ReqCreateGame";
    case constants::ReqType::CREATEGAMEFILLED: return "ReqCreateGameFilled";
    case constants::ReqType::UPDATEPLAYER: return "ReqUpdatePlayer";
    case constants::ReqType::VIEWGAME:  return "ReqViewGame";
    case constants::ReqType::UPDATEGAME:  return "ReqUpdateGame";
    case constants::ReqType::DEMOGETGAMES:  return "DemoReqGetGamesList";
    case constants::ReqType::DEMOGETGAME:  return "DemoReqGetGame";
  }
  __builtin_unreachable();
}

    
}