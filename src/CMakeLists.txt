add_executable(main)

# Link SDL2, SDL2_gfx, and SDL2_image properly
target_include_directories(main PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_GFX_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
target_link_directories(main PRIVATE ${SDL2_LIBRARY_DIRS} ${SDL2_GFX_LIBRARY_DIRS} ${SDL2_IMAGE_LIBRARY_DIRS})
target_link_libraries(main ${SDL2_LIBRARIES} ${SDL2_GFX_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

# Add compiler flags for all SDL2 libraries
target_compile_options(main PRIVATE ${SDL2_CFLAGS_OTHER} ${SDL2_GFX_CFLAGS_OTHER} ${SDL2_IMAGE_CFLAGS_OTHER})

target_sources(
  main
  PRIVATE main.cpp
          graphics.cpp
          vec2.cpp
          particle.cpp
          shape.cpp
          body.cpp
          collision_detection.cpp
          contact.h
          world.cpp
          vec_n.cpp
          constraint.cpp
          matrix_mn.cpp
          app_constraint.cpp)