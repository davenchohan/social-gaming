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
enum class GameComponentType : char {
  DISPLAY, SINGLE_SELECT, MULTI_SELECT, INPUT
};

// CONSIDER: having reqtype when handling response from server

constexpr inline std::string_view
to_string(const constants::ReqType request) noexcept {
  switch (request) {
    case constants::ReqType::GETGAMES:   return "ReqGetGamesList";
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

constexpr inline std::string_view
to_string(const constants::GameComponentType component) noexcept {
  switch (component) {
    case constants::GameComponentType::DISPLAY:   return "Display";
    case constants::GameComponentType::SINGLE_SELECT:   return "SingleSelect";
    case constants::GameComponentType::MULTI_SELECT: return "MultiSelect";
    case constants::GameComponentType::INPUT: return "Input";
  }
  __builtin_unreachable();
}

    
}