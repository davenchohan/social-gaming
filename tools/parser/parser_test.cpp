#include <iostream>

#include <cassert>
#include <cstdio>
#include <memory>
#include <string>

#include <cpp-tree-sitter.h>


extern "C" {
TSLanguage* tree_sitter_socialgaming();
}


int main() {
  // Create a language and parser.
  ts::Language language = tree_sitter_socialgaming();
  ts::Parser parser{language};

  // Parse the provided string into a syntax tree.
  std::string sourcecode = "configuration {"
                            "name: \"\""
                            "player range: (0, 0)"
                            "audience: false"
                            "setup: {}"
                            "}";
  ts::Tree tree = parser.parseString(sourcecode);

  // Get the root node of the syntax tree.
  ts::Node root = tree.getRootNode();

  // Get some child nodes.
  ts::Node configuration = root.getNamedChild(0);
  ts::Node name = configuration.getChildByFieldName("name");
  ts::Node playerRange = configuration.getChildByFieldName("player_range");
  ts::Node hasAudience = configuration.getChildByFieldName("has_audience");

  // Print the syntax tree as an S-expression.
  auto treestring = root.getSExpr();
  printf("Syntax tree: %s\n", treestring.get());

  return 0;
}
