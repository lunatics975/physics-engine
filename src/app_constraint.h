#ifndef APP_CONSTRAINT_H
#define APP_CONSTRAINT_H

#include "body.h"
#include "graphics.h"
#include "vec2.h"
#include "world.h"
#include <SDL2/SDL_rect.h>
#include <vector>

class AppConstraint {
  private:
    bool debug = false;
    bool running = false;
    World *world;

    SDL_Texture *bg_texture;

    Vec2 push_force = Vec2(0, 0); // controlled by keyboard
    Vec2 mouse_cursor = Vec2(0, 0);
    [[maybe_unused]] bool left_mouse_button_down = false;

    // SDL_Rect liquid;

    Vec2 anchor;
    [[maybe_unused]] float rest_chain_length = 15;
    [[maybe_unused]] float rest_soft_body_length = 200;
    [[maybe_unused]] float k_chain = 300;
    [[maybe_unused]] float k_soft_body = 1500;
    [[maybe_unused]] const int NUM_CHAINED_BODIES = 15;
    [[maybe_unused]] const int NUM_SOFT_BODY_BODIES = 4;

  public:
    AppConstraint() = default;
    ~AppConstraint() = default;
    bool is_running();
    void setup();
    void input();
    void update();
    void render();
    void destroy();
};

#endif
