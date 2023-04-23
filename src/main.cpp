#include <iostream>

#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/component/screen_interactive.hpp"

#include <thread>

int main(void) {
  using namespace ftxui;

  auto my_graph = [](int width, int height) {
    if (width < 0) {
      std::cerr << "Width is <0 and I'll now crash :(" << std::endl;
    }

    return std::vector<int>(width, height / 2);
  };

  auto main_window = Renderer([&] {
    return window(
      text("Outer window"),
        hbox({
          text("I am here to waste some space"),
          window(
            text("Inner window"),
            graph(std::ref(my_graph))
          ),
      })      
    );
  });

  auto screen = ScreenInteractive::Fullscreen();

  std::atomic<bool> refresh_ui_continue = true;
  std::thread refresh_ui([&] {
    while (refresh_ui_continue) {
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(0.05s);
      screen.Post(Event::Custom);
    }
  });

  screen.Loop(main_window);
  refresh_ui_continue = false;
  refresh_ui.join();

  return EXIT_SUCCESS;
}
