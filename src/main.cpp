#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_joystick.h>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "lcd.hpp"
#include "nodes/FunctionNode.hpp"
#include "nodes/GroupNode.hpp"
#include "nodes/LabelNode.hpp"

int main() {
  // Init LCD Panel
  LCD::init();
  LCD::clearAndSet("Loading", "");

  GroupNode *currentParentNode = nullptr;

  auto homeNode =
      std::make_unique<GroupNode>("Home", nullptr, &currentParentNode);

  auto musicGroupNode =
      std::make_unique<GroupNode>("Media", homeNode.get(), &currentParentNode);
  {
    musicGroupNode->addNode(std::make_unique<FunctionNode>(
        "Toggle", musicGroupNode.get(), []() { system("mpc toggle"); }));

    musicGroupNode->addNode(std::make_unique<FunctionNode>(
        "Skip", musicGroupNode.get(), []() { system("mpc next"); }));

    musicGroupNode->addNode(
        std::make_unique<FunctionNode>("All_Music", musicGroupNode.get(), []() {
          system("mpc clear; mpc add Music; mpc shuffle; mpc play");
        }));
  }

  auto ytGroupNode = std::make_unique<GroupNode>("yt_Test", homeNode.get(),
                                                 &currentParentNode);
  {
    ytGroupNode->addNode(
        std::make_unique<FunctionNode>("Dwnld&Ply", musicGroupNode.get(), []() {
          const std::string testURL =
              "https://www.youtube.com/watch?v=r8AEUP5IyPc";
          const std::string ytdlpCommand =
              "/usr/local/bin/yt-dlp -x -o \"~/yt/%(title)s.%(ext)s\" '" +
              testURL + "'";

          system("mpc clear");

          std::cout << "Downloading yt content...\n";
          LCD::clearAndSet("Downloading...", "");
          system(ytdlpCommand.c_str());
          std::cout << "Finished Downloading yt content.\n";
          LCD::clearAndSet("Finished", "");

          system("mpc update");
          system("sleep 1");
          system("mpc add yt");
          system("mpc play");
        }));
  }

  homeNode->addNode(std::move(musicGroupNode));
  homeNode->addNode(std::move(ytGroupNode));

  currentParentNode = homeNode.get();
  currentParentNode->render();

  // Init our joystick
  if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
    std::cout << "Error: " << SDL_GetError() << "\n";
  }

  if (SDL_NumJoysticks() < 1) {
    printf("Warning: No joysticks connected!\n");
  } else {
    if (SDL_JoystickOpen(0) == NULL) {
      printf("Warning: Unable to open game controller! SDL Error: %s\n",
             SDL_GetError());
    }
  }

  while (true) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_JOYBUTTONDOWN:
        switch (e.jbutton.button) {
        case 0:
          currentParentNode->getSelectedNode()->action();
          currentParentNode->render();
          break;
        case 1:
          currentParentNode->right();
          currentParentNode->render();
          break;
        case 2:
          currentParentNode->left();
          currentParentNode->render();
          break;
        case 3:
          currentParentNode = homeNode.get();
          currentParentNode->render();
          break;
        }
        break;
      }
    }

    signal(SIGINT, [](int sig) { std::exit(1); });
  }
}
