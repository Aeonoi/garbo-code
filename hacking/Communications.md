# Communications Protocol

This document outlines the communication model for the Asteroids game.

**Note:** The client-server model described here is the intended architecture. The current implementation is local-only.

The game will use a **client-server model**. One player acts as the host (server), and other players connect as clients.

## Server Responsibilities
- Managing game state (player positions, asteroid positions, scores).
- Broadcasting game state updates to all clients.
- Handling new client connections.
- Initiating the game start.

## Client Responsibilities
- Sending user input (movement, shooting) to the server.
- Receiving game state updates from the server and rendering the game.

## Network Protocol
Communication will be done over TCP sockets. A simple custom protocol will be used to exchange game data.
