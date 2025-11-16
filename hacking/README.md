# HackRPI-2025

## Asteroids Game
Modern take on the game ``Astroids``, the retro game from 1979. This is using the TUI library ``notcurses`` in C++ to render the game. It is peer to peer connection with the person hosting being the server.

The map should be a very large square, but not infinite. The players are spawned on the borders randomly. 

Astroids will glide around. The initial map is given to the client when started and all players can calculate the astroid positions live. Astroids will bounce off the wall.

All players start the game at the same time. To start the game, the host will host create a room where as many people can join at once. Then the host dictates when the players start.

Players can move with WASD (or arrow keys) and space to fire.

Players have 3 hp. Players can shoot other players. Players can gain HP by picking up health packs that spawn randomly on the map. Astroid health is based on their size. Players can shoot astroids.

There is a point system. If you break an astroid you get points based on the size. If you kill a player you get 1/3 of their points.

There is an exponentially increasing exp system where you can level up your weaponds based on points. Level up can randomly change your weapon attributes.

## Object-Oriented Design
To improve the game's design, an object-oriented programming (OOP) concept has been applied. The game has been restructured to include the following hierarchy:

### Entities Object
An abstract base class `Entity` has been introduced. It represents generic game objects with basic properties and methods. Both Player and Asteroid inherit from this class.

#### Player
Players are now represented by the `Player` class, which extends `Entity`. Each player contains attributes like health, position, and weapon type, customized using the point system.

#### Asteroid
Asteroids are represented by the `Astroid` class, which also extends `Entity`. Each astroid has properties such as size, health, and velocity.

This OOP approach makes it easier to add new types of entities or modify existing ones without drastically changing the codebase.

Players will be a triangle generated with ASCII
Astroids will be a rough circle generated with ASCII


## AI Generated Content
Gemini will generate the player's weapons based off the points system such that each player can have a set of attributes or weapons that are different from each other. 

## Current Features
- **Movement:** Players can move using WASD keys.
- **Shooting:** Players can shoot bullets using the spacebar.
- **Health Bar:** A health bar is displayed at the top left of the screen.
- **Multiplayer Menu:** A menu is displayed on startup to allow the user to host or join a game.

## How players communicates with each other
The game uses a client-server model for multiplayer. One player acts as the host (server) and other players connect as clients. The server is responsible for managing the game state and communicating it to the clients. More details can be found in `Communications.md`.

## Instructions
There will be a instruction manuals (saved as Instructions.md) on how to play the game.



