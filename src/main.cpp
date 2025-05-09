#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_joystick.h>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
  if (!SDL_Init(SDL_INIT_JOYSTICK)) {
    std::cout << "Error: " << SDL_GetError() << "\n";
  }

  if (!SDL_HasJoystick()) {
    throw std::logic_error("Joystick not connected");
  }

  SDL_JoystickID joystickID = SDL_GetJoysticks(NULL)[0];
  SDL_Joystick *joystick = SDL_OpenJoystick(joystickID);

  while (true) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
        switch (e.jbutton.button) {
        case 0:
          std::cout << "Button 0 pressed\n";
          break;
        }
        break;
      }
    }

    signal(SIGINT, [](int sig) { std::exit(1); });
  }
}
