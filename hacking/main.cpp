#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <chrono>
#include <iostream>
#include <map>
#include <memory>
#include <notcurses/notcurses.h>
#include <thread>
#include <cstring>
#include <notcurses/nckeys.h>
#include <string>
#include <vector>


#include "config.h"
#include "Asteroid.h"
#include "Player.h"
#include "Bullet.h"
#include "Network.h"
#include "Camera.h"

std::string get_ip_address() {
    struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1) {
        return "N/A";
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;

        family = ifa->ifa_addr->sa_family;

        if (family == AF_INET) {
            s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
                            host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            if (s != 0) {
                continue;
            }
            // Don't return loopback address
            if (strcmp(host, "127.0.0.1") != 0) {
                freeifaddrs(ifaddr);
                return host;
            }
        }
    }

    freeifaddrs(ifaddr);
    return "N/A";
}



std::string get_string_from_user(struct notcurses* nc, const std::string& message, const std::string& default_ip) {
    ncplane* stdplane = notcurses_stdplane(nc);
    unsigned int dimy, dimx;
    ncplane_dim_yx(stdplane, &dimy, &dimx);
    ncplane_erase(stdplane);

    ncplane_putstr_yx(stdplane, dimy / 2 - 2, (dimx - message.length()) / 2, message.c_str());
    notcurses_render(nc);

    std::string input_string;
    while (true) {
        ncinput ni;
        notcurses_get_blocking(nc, &ni);

        if (ni.id == NCKEY_ENTER) {
            break;
        } else if (ni.id == NCKEY_BACKSPACE) {
            if (!input_string.empty()) {
                input_string.pop_back();
            }
        } else if (ni.id == 'l') {
            return default_ip;
        } else if (ni.id >= 32 && ni.id <= 126) { // Printable ASCII
            input_string += (char)ni.id;
        }

        ncplane_erase(stdplane);
        ncplane_putstr_yx(stdplane, dimy / 2 - 2, (dimx - message.length()) / 2, message.c_str());
        ncplane_putstr_yx(stdplane, dimy / 2, (dimx - input_string.length()) / 2, input_string.c_str());
        notcurses_render(nc);
    }
    return input_string;
}

void draw_health_bar(ncplane* plane, const Player& player) {
  int hp = player.getHp();
  std::string health_text = "Health: " + std::to_string(hp);
  ncplane_putstr_yx(plane, 0, 0, health_text.c_str());

  // Display player world position below the health bar
  int px = player.getX();
  int py = player.getY();
  std::string pos_text = "Pos: (" + std::to_string(px) + ", " + std::to_string(py) + ")";
  ncplane_putstr_yx(plane, 1, 0, pos_text.c_str());
}

int show_menu(struct notcurses* nc) {
    ncplane* stdplane = notcurses_stdplane(nc);
    unsigned int dimy, dimx;
    ncplane_dim_yx(stdplane, &dimy, &dimx);

    ncplane_erase(stdplane);
    ncplane_set_bg_rgb8(stdplane, 0, 0, 0);
    ncplane_set_fg_rgb8(stdplane, 200, 200, 200);

    const char* title = "ASTEROIDS";
    const char* host_option = "1. Host Game";
    const char* join_option = "2. Join Game";
    const char* quit_option = "3. Quit";

    ncplane_putstr_yx(stdplane, dimy / 2 - 5, (dimx - strlen(title)) / 2, title);
    ncplane_putstr_yx(stdplane, dimy / 2, (dimx - strlen(host_option)) / 2, host_option);
    ncplane_putstr_yx(stdplane, dimy / 2 + 1, (dimx - strlen(join_option)) / 2, join_option);
    ncplane_putstr_yx(stdplane, dimy / 2 + 2, (dimx - strlen(quit_option)) / 2, quit_option);

    notcurses_render(nc);

    while (true) {
        ncinput input;
        if (notcurses_get_nblock(nc, &input) != (uint32_t)-1) {
            if (input.id == '1') {
                return 1;
            } else if (input.id == '2') {
                return 2;
            } else if (input.id == '3' || input.id == 'q') {
                return 3;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void checkCollisions(Player& player, std::vector<std::unique_ptr<Asteroid>>& asteroids, std::vector<std::unique_ptr<Bullet>>& bullets) {

  // Player-asteroid collisions

  for (auto asteroidIt = asteroids.begin(); asteroidIt != asteroids.end(); ) {

      float dx = player.getX() - (*asteroidIt)->getx();

      float dy = player.getY() - (*asteroidIt)->gety();



      float distSq = dx*dx + dy*dy;



      float minDist = 3; // Assuming a collision radius for player and asteroid

      float minDistSq = minDist * minDist;



      if (distSq < minDistSq) {

          // Collision with player

          player.damage(1); // Player loses 1 health

          asteroidIt = asteroids.erase(asteroidIt); // Asteroid disappears

      } else {

          ++asteroidIt;

      }

  }



  // Bullet-asteroid collisions

  for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); ) {

      bool bulletRemoved = false;



      for (auto asteroidIt = asteroids.begin(); asteroidIt != asteroids.end(); ) {



          float dx = (*bulletIt)->getx() - (*asteroidIt)->getx();

          float dy = (*bulletIt)->gety() - (*asteroidIt)->gety();



          float distSq = dx*dx + dy*dy;



          float minDist = 3;

          float minDistSq = minDist * minDist;



          if (distSq < minDistSq) {

              // collision

              // remove asteroid

              (*asteroidIt)->damage((*bulletIt)->getDamage());

              if ((*asteroidIt)->gethp() <= 0)

                asteroidIt = asteroids.erase(asteroidIt);



              // remove bullet

              bulletIt = bullets.erase(bulletIt);

              bulletRemoved = true;

              break;

          } else {

              ++asteroidIt;

          }

      }



      if (!bulletRemoved)

          ++bulletIt;

  }

}

int main(int argc, char* argv[]){
  printf("%d\n", argc);
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <string>" << std::endl;
    return 1;
  }
  char* s = argv[1];
  notcurses_options ncopts{};
  ncopts.flags = NCOPTION_SUPPRESS_BANNERS;

  struct notcurses* nc = notcurses_init(&ncopts, NULL);
  if (nc == NULL) {
    std::cerr << "Failed to initialize Notcurses." << std::endl;
    return 1;
  }

  Network net;
  int choice = show_menu(nc);

  if (choice == 1) { // Host
    ncplane* stdplane = notcurses_stdplane(nc);
    unsigned int dimy, dimx;
    ncplane_dim_yx(stdplane, &dimy, &dimx);
    ncplane_erase(stdplane);
    std::string ip = get_ip_address();
    std::string msg = "Hosting on IP: " + ip;
    ncplane_putstr_yx(stdplane, dimy / 2 -1, (dimx - msg.length()) / 2, msg.c_str());
    notcurses_render(nc);

    if (!net.host(31415)) {
        notcurses_stop(nc);
        std::cerr << "Failed to host." << std::endl;
        return 1;
    }
    
    while(true) {
        net.accept_new_client();
        std::string wait_msg = "Waiting for players to join... (" + std::to_string(net.getClientCount()) + " joined)";
        ncplane_erase(stdplane);
        ncplane_putstr_yx(stdplane, dimy / 2 -1, (dimx - msg.length()) / 2, msg.c_str());
        ncplane_putstr_yx(stdplane, dimy / 2 + 1, (dimx - wait_msg.length()) / 2, wait_msg.c_str());
        ncplane_putstr_yx(stdplane, dimy / 2 + 2, (dimx - 29) / 2, "Press Enter to start game...");
        notcurses_render(nc);

        ncinput ni;
        notcurses_get_nblock(nc, &ni);
        if (ni.id == NCKEY_ENTER) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    net.send_start();

  } else if (choice == 2) { // Join
    std::string host_ip = get_ip_address();
    std::string ip = get_string_from_user(nc, "Enter host IP: (press 'l' for local)", host_ip);
    
    ncplane* stdplane = notcurses_stdplane(nc);
    unsigned int dimy, dimx;
    ncplane_dim_yx(stdplane, &dimy, &dimx);
    ncplane_erase(stdplane);
    std::string msg = "Joining " + ip + "...";
    ncplane_putstr_yx(stdplane, dimy / 2, (dimx - msg.length()) / 2, msg.c_str());
    notcurses_render(nc);

    if (!net.join(ip, 31415)) {
        notcurses_stop(nc);
        std::cerr << "Failed to join." << std::endl;
        return 1;
    }

    msg = "Waiting for host to start...";
    ncplane_erase(stdplane);
    ncplane_putstr_yx(stdplane, dimy / 2, (dimx - msg.length()) / 2, msg.c_str());
    notcurses_render(nc);

    if (!net.wait_for_start()) {
        notcurses_stop(nc);
        std::cerr << "Failed to start." << std::endl;
        return 1;
    }

  } else if (choice == 3) { // Quit
    notcurses_stop(nc);
    return 0;
  }

  ncplane* stdplane = notcurses_stdplane(nc);
    unsigned int dimy, dimx;
    ncplane_dim_yx(stdplane, &dimy, &dimx);

    ncplane_set_bg_rgb8(stdplane, 0, 0, 0);
    ncplane_set_fg_rgb8(stdplane, 200, 200, 200);

    std::vector<Player> players;
    players.emplace_back(dimx / 2, dimy / 2);
    Player& player = players[0];
    player.setWord(argv[1]);

    Camera camera(0, 0);
    std::vector<std::unique_ptr<Asteroid>> asteroids;
    std::vector<std::unique_ptr<Bullet>> bullets;
    for (int i = 0; i < 0; ++i) {
      asteroids.push_back(std::make_unique<Asteroid>(
          rand() % dimx, rand() % (dimy / 2), (rand() % 3) + 1));
    }
    int indx = 0;

    while (true) {
      // Check for terminal resize
      unsigned int new_dimy, new_dimx;
      ncplane_dim_yx(stdplane, &new_dimy, &new_dimx);
      if (new_dimx != dimx || new_dimy != dimy) {
        dimx = new_dimx;
        dimy = new_dimy;
        // Reposition entities within new bounds
        if (player.getX() >= (int)dimx) player.setPosition(dimx - 1, player.getY());
        if (player.getY() >= (int)dimy) player.setPosition(player.getX(), dimy - 1);
      }
      int len = strlen(argv[1]);
      ncinput input;
      if (notcurses_get_nblock(nc, &input) != (uint32_t)-1) {
        if (input.id == 'q') {
          break;
        }

        switch (input.id) {
        case 'w':
          player.move(0, -1);
          player.setVelocity(0, -PLAYER_SPEED);
          break;
        case 'a':
          player.move(-1, 0);
          player.setVelocity(-PLAYER_SPEED, 0);
          break;
        case 's':
          player.move(0, 1);
          player.setVelocity(0, PLAYER_SPEED);
          break;
        case 'd':
          player.move(1, 0);
          player.setVelocity(PLAYER_SPEED, 0);
          break;
        case 'x':
          player.setVelocity(0, 0);
          break;
        case ' ': // Spacebar for shooting
          char c;
          if (indx < len) {
            c = argv[1][indx];

          } else {
            indx = -1;
            c = '\0';
          }

          bullets.push_back(std::make_unique<Bullet>(player.getX(), player.getY(), player.getVX() * 2, player.getVY() * 2));
          bullets.back()->setWord(c);
          indx++;

          break;
        }
      }

      // update screen dimensions and camera so the player stays centered
      ncplane_dim_yx(stdplane, &dimy, &dimx);
      camX = player.getX() - static_cast<int>(dimx) / 2;
      camY = player.getY() - static_cast<int>(dimy) / 2;

      ncplane_erase(stdplane);
      draw_health_bar(stdplane, player);

      // Update camera to center on player
      camera.setOffset(player.getX() - dimx / 2, player.getY() - dimy / 2);

      for(auto& p : players) {
        p.update();
        p.draw(stdplane);
      }
      for (auto& asteroid : asteroids) asteroid->update();
      for (auto& bullet : bullets) bullet->update();

      checkCollisions(player, asteroids, bullets);  // <-- collision code here

      for (auto& asteroid : asteroids) asteroid->draw(stdplane);
      for (auto& bullet : bullets) bullet->draw(stdplane);


      // Render only if notcurses is ready
      if (notcurses_render(nc) == 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
      } else {
        // If rendering failed, wait a bit longer before retrying
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
    }

  notcurses_stop(nc);
  std::cout << "Game ran for 5 seconds successfully." << std::endl;
  return 0;
}
