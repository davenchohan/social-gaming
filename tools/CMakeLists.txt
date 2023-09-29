
if (NETWORKING_EMSCRIPTEN_BUILD)
  message("Skipping server and NCurses client under emscripten build.")
else()
  add_subdirectory(server)
endif()



if (ENABLE_FTXUI_CLIENT)
  add_subdirectory(client-ftxui)
endif()
