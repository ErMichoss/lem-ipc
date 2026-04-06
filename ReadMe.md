# Lem-IPC

> A multiplayer battle game using Unix IPC mechanisms — shared memory, message queues, and semaphores.

---

## Description

Lem-IPC is a 2D board game where players organized in teams battle each other using inter-process communication. Each player is an independent process. The last team standing wins.

### Game Rules

- Players are organized in teams and battle on a 2D grid
- A player dies when surrounded by **2 or more players from the same enemy team** (diagonals count)
- One tile can only hold one player at a time
- The last team on the board wins

---

## Requirements

- GCC with C99 support
- SDL2, SDL2_image, SDL2_ttf (compiled locally, see Installation)

---

## Installation

### 1. Clone the repo
```bash
git clone https://github.com/ErMichoss/lem-ipc.git
cd lem-ipc
```

### 2. Compile SDL2 libraries locally
```bash
# SDL2
wget https://github.com/libsdl-org/SDL/releases/download/release-2.26.5/SDL2-2.26.5.tar.gz
tar -xzf SDL2-2.26.5.tar.gz
cd SDL2-2.26.5
./configure --prefix=$(pwd)/../sdl2
make && make install
cd ..
rm -rf SDL2-2.26.5 SDL2-2.26.5.tar.gz

# SDL2_image
wget https://github.com/libsdl-org/SDL_image/releases/download/release-2.6.3/SDL2_image-2.6.3.tar.gz
tar -xzf SDL2_image-2.6.3.tar.gz
cd SDL2_image-2.6.3
./configure --prefix=$(pwd)/../sdl2_image --with-sdl-prefix=$(pwd)/../sdl2
make && make install
cd ..
rm -rf SDL2_image-2.6.3 SDL2_image-2.6.3.tar.gz

# SDL2_ttf
wget https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.20.2/SDL2_ttf-2.20.2.tar.gz
tar -xzf SDL2_ttf-2.20.2.tar.gz
cd SDL2_ttf-2.20.2
./configure --prefix=$(pwd)/../sdl2_ttf --with-sdl-prefix=$(pwd)/../sdl2
make && make install
cd ..
rm -rf SDL2_ttf-2.20.2 SDL2_ttf-2.20.2.tar.gz
```

### 3. Compile the project
```bash
make
```

Custom board size:
```bash
make BOARD="30 40"   # MAX_H=30 MAX_W=40
make TEAMS=10        # MAX_TEAMS=10
```

---

## Usage

### Add players
```bash
./lemipc <team_id>
```

### Launch the display
```bash
./lemipc_display
```

The **first player** to join will be prompted to press ENTER to start the game. Add as many players as you want before pressing ENTER.

### Example — 2 teams, 2 players each
```bash
# Terminal 1
./lemipc_display

# Terminal 2 (press ENTER here to start)
./lemipc 1

# Terminal 3
./lemipc 1

# Terminal 4
./lemipc 2

# Terminal 5
./lemipc 2
```

---

## Technical Architecture

### IPC Mechanisms

| Mechanism | Usage |
|---|---|
| **Shared Memory** | Stores the board grid, player count, team info, winner |
| **Message Queue** | Players communicate target positions to teammates |
| **Semaphores** | Protects concurrent access to shared memory |

### IPC Lifecycle

- **First player** creates all IPC resources (SHM, MSGQ, semaphores)
- **Last player** destroys all IPC resources on exit
- **SIGINT** is handled — resources are always cleaned up

### Player AI

Each player follows this logic every turn:
1. Check if surrounded by 2+ enemies → die
2. Read teammate messages to get shared target
3. If no target → find nearest enemy → broadcast target
4. Move towards best adjacent position around target
5. If blocked → try perpendicular directions

---

## Project Structure

```
lemipc/
├── Makefile
├── incl/
│   ├── lem_ipc.h       ← game structs, IPC keys, defines
│   └── display.h       ← SDL2 structs and display functions
├── src/
│   ├── main.c          ← entry point, player init
│   ├── ipc_init.c      ← SHM, MSGQ, semaphore init
│   ├── ipc_clean.c     ← IPC cleanup
│   ├── board.c         ← grid operations
│   ├── player.c        ← main game loop, death detection
│   ├── brain.c         ← AI movement logic
│   ├── messages.c      ← send/recv MSGQ
│   ├── display.c       ← SDL2 rendering
│   ├── display_main.c  ← display process entry point
│   └── aux.c           ← utility functions
└── assets/
    ├── ant_left.png
    ├── ant_right.png
    ├── ant_up.png
    ├── ant_down.png
    ├── floor_a.png
    ├── floor_b.png
    ├── Roboto-Regular.ttf
    └── Monoton-Regular.ttf
```

---

## Cleaning up

If the program crashes and leaves orphan IPCs:
```bash
for id in $(ipcs -m | grep $USER | awk '{print $2}'); do ipcrm -m $id; done
for id in $(ipcs -q | grep $USER | awk '{print $2}'); do ipcrm -q $id; done
for id in $(ipcs -s | grep $USER | awk '{print $2}'); do ipcrm -s $id; done
```

---