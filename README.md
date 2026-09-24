# && A roguelike CLI RPG game with random rooms to explore! &&

## This is a game where you run around in randomly generated rooms and fight evil C operations for as long as you can until you die!

### Key features of the game:
  - ASCII-based player rendering and movement
  - Dynamic collision detection with map boundaries
  - Randomly generated rooms
  - Fog of war obstructing unexplored parts of the current room (boss rooms are fully visible)
  - Different kinds of enemies to fight (ADDRESS, MODULO, and the BOSS)
  - A RPG-inspired battle screen with a rolling battle log
  - Only leaving the room after beating every enemy
  - A highscore feature to track how many enemies you have beaten
  - Power ups to make you stronger (and enemies that scale with each boss, so the game gets harder while playing)
  - Boss fights that drop a special item
  - Items to use in battle and out of it (heal, damage, poison, and stat boosts)

### DEMO URL:

coming soon!

### HOW TO PLAY

  - Move: W A S D
  - Battle: 1 = attack, 2 = inventory, 3 = run. Press ENTER to confirm.
  - Inventory: A / D to move the cursor, ENTER to use an item, ESC toleave without picking.
  - Quit: q
  - Also check out the controls menu in game
### BUILDING THE PROJECT YOURSELF

You'll need the following packages:

  - clang
  - clang-tools
  - gdb
  - gnumake
  - ncurses

The Makefile is included. From the project folder:

    make
    ./main

To clean up build artifacts:

    make clean

### PROJECT STRUCTURE

  - main.c           - game loop and state management
  - map.c / map.h    - map generation, collision, enemy pursuit
  - battle_screen.c  - battle logic and rendering
  - battle_log.c     - battle log messages
  - inventory.c      - inventory screen
  - item.c / item.h  - item types and effects
  - npc.c / npc.h    - enemies and their sprites
  - entity.c         - entity stats and health bars
  - generation.c     - room generation / rng
  - menu.c / menu.h  - main menu
  - highscore.c      - highscore handler
  - config.h         - constant values

### STATUS

Build isn't fully polished. Bugs may occur. There are no known crashes, but things can still not work as intended.

### LICENSE

MIT (see license file)