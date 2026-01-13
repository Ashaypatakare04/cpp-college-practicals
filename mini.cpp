
#include <bits/stdc++.h>
using namespace std;

// =============== Cross-platform nonblocking keyboard ===============
#if defined(_WIN32)
  #include <conio.h>
  bool kb_hit() { return _kbhit() != 0; }
  int kb_getch() { return _getch(); }
  void kb_setup() {}
  void kb_restore() {}
#else
  #include <termios.h>
  #include <unistd.h>
  #include <fcntl.h>
  static struct termios orig_termios;
  void kb_setup() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
  }
  void kb_restore() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
  }
  bool kb_hit() {
    unsigned char ch;
    if (read(STDIN_FILENO, &ch, 1) == 1) {
      // push back into a static buffer
      ungetc(ch, stdin);
      return true;
    }
    return false;
  }
  int kb_getch() {
    int ch = getchar();
    if (ch == 0x1B) { // ESC sequence for arrows
      // Try to read next two bytes if available: [ A/B/C/D
      int ch1 = getchar();
      int ch2 = getchar();
      if (ch1 == '[') {
        switch (ch2) {
          case 'A': return 0x4800; // up
          case 'B': return 0x5000; // down
          case 'C': return 0x4D00; // right
          case 'D': return 0x4B00; // left
        }
      }
    }
    return ch;
  }
#endif

// =============== ANSI helpers (clears + cursor) ====================
void ansi_clear() {
  // Clear screen and move cursor to 1,1
  cout << "\x1b[2J\x1b[H";
}
void ansi_hide_cursor(bool hide) {
  cout << (hide ? "\x1b[?25l" : "\x1b[?25h");
}
void ansi_move(int row, int col) {
  cout << "\x1b[" << row << ";" << col << "H";
}

// =============== Game types ========================================
struct Obstacle {
  int lane;   // which lane index
  int row;    // current vertical row (0 = top)
};

struct GameConfig {
  int lanes = 3;          // number of lanes (2–4 feels good)
  int roadWidth = 21;     // inner road width (odd looks nicer)
  int roadHeight = 24;    // visible rows
  int borderPad = 2;      // spaces left/right outside road
  int lanePad = 2;        // min gap between lane bars
};

struct GameState {
  int playerLane = 1;     // start in middle lane (0..lanes-1)
  vector<Obstacle> obs;
  bool paused = false;
  bool gameOver = false;
  long long score = 0;
  double tickMs = 80.0;   // frame duration; decreases for speedup
  double spawnEvery = 11; // frames between spawns; decreases too
  int frame = 0;
};

// =============== Lane positions ====================================
vector<int> laneCenters(const GameConfig& cfg) {
  // Compute x cols for each lane center inside road area
  vector<int> centers;
  int inner = cfg.roadWidth;
  int lanes = cfg.lanes;
  if (lanes == 1) {
    centers = { inner/2 };
  } else {
    // distribute evenly
    for (int i = 0; i < lanes; ++i) {
      double t = (lanes == 1 ? 0.5 : (double)i / (lanes - 1));
      int x = (int)round(t * (inner - 1));
      centers.push_back(x);
    }
  }
  return centers;
}

// =============== Drawing ===========================================
void drawFrame(const GameConfig& cfg, const GameState& st) {
  // We will render from scratch each frame
  ansi_clear();

  int outer = cfg.borderPad * 2 + cfg.roadWidth + 2; // +2 for | borders
  // Header
  ansi_move(1, 1);
  cout << "Car Runner  —  Score: " << st.score
       << (st.paused ? "  [PAUSED]" : "")
       << (st.gameOver ? "  [GAME OVER]" : "")
       << "\n";

  // Build a 2D char buffer (rows x cols)
  vector<string> canvas(cfg.roadHeight, string(outer, ' '));

  // Draw borders and road
  for (int r = 0; r < cfg.roadHeight; ++r) {
    int left = cfg.borderPad;
    int right = left + cfg.roadWidth + 1;
    if (left >= 0 && left < outer) canvas[r][left] = '|';
    if (right >= 0 && right < outer) canvas[r][right] = '|';

    // dotted center lines (just for vibe)
    if (r % 2 == 0) {
      int cMid = left + 1 + (cfg.roadWidth / 2);
      if (cMid > left && cMid < right) canvas[r][cMid] = ':';
    }
  }

  // Draw obstacles (▼)
  auto centers = laneCenters(cfg);
  for (const auto& ob : st.obs) {
    int laneX = cfg.borderPad + 1 + centers[ob.lane];
    int row = ob.row;
    if (row >= 0 && row < cfg.roadHeight &&
        laneX >= 0 && laneX < (int)canvas[row].size()) {
      canvas[row][laneX] = 'V';
    }
  }

  // Draw player car (▲)
  {
    int x = cfg.borderPad + 1 + laneCenters(cfg)[st.playerLane];
    int y = cfg.roadHeight - 2; // near bottom
    if (y >= 0 && y < cfg.roadHeight &&
        x >= 0 && x < (int)canvas[y].size()) {
      canvas[y][x] = 'A';
    }
  }

  // Print canvas
  for (int r = 0; r < cfg.roadHeight; ++r) {
    cout << canvas[r] << "\n";
  }

  // Footer / help
  cout << "\nControls: A/D or ←/→ to move   |   P to pause   |   Q to quit\n";
  cout.flush();
}

// =============== Input handling ====================================
void applyInput(GameState& st, const GameConfig& cfg) {
  while (kb_hit()) {
    int c = kb_getch();

    // Normalize arrows on POSIX mapping above & on Windows with getch+extended
#if defined(_WIN32)
    if (c == 0 || c == 224) {
      int e = kb_getch();
      if (e == 75) c = 0x4B00; // left
      if (e == 77) c = 0x4D00; // right
    }
#endif

    // Translate to intents
    if (c == 'a' || c == 'A' || c == 0x4B00) { // left
      if (!st.paused && !st.gameOver) st.playerLane = max(0, st.playerLane - 1);
    } else if (c == 'd' || c == 'D' || c == 0x4D00) { // right
      if (!st.paused && !st.gameOver) st.playerLane = min(cfg.lanes - 1, st.playerLane + 1);
    } else if (c == 'p' || c == 'P') {
      if (!st.gameOver) st.paused = !st.paused;
    } else if (c == 'q' || c == 'Q' || c == 27 /*ESC*/) {
      st.gameOver = true;
    } else if ((c == 'r' || c == 'R') && st.gameOver) {
      // quick restart (hidden)
    }
  }
}

// =============== Game update =======================================
void spawnObstacle(GameState& st, const GameConfig& cfg, mt19937& rng) {
  uniform_int_distribution<int> laneDist(0, cfg.lanes - 1);
  Obstacle ob;
  ob.lane = laneDist(rng);
  ob.row = -1; // just above the screen
  st.obs.push_back(ob);
}

void update(GameState& st, const GameConfig& cfg, mt19937& rng) {
  if (st.paused || st.gameOver) return;

  // Spawn logic
  if (st.frame % (int)round(st.spawnEvery) == 0) {
    spawnObstacle(st, cfg, rng);
  }

  // Move obstacles down
  for (auto& ob : st.obs) ob.row++;

  // Remove off-screen
  st.obs.erase(remove_if(st.obs.begin(), st.obs.end(),
                         [&](const Obstacle& o){ return o.row >= cfg.roadHeight; }),
               st.obs.end());

  // Collision check (player seat)
  int playerRow = cfg.roadHeight - 2;
  for (const auto& ob : st.obs) {
    if (ob.row == playerRow && ob.lane == st.playerLane) {
      st.gameOver = true;
      break;
    }
  }

  // Score + difficulty
  st.score += 1;
  if (st.frame % 50 == 0) {             // every ~50 frames, increase speed
    st.tickMs = max(35.0, st.tickMs - 2.5);
    st.spawnEvery = max(5.0, st.spawnEvery - 0.25);
  }

  st.frame++;
}

// =============== Main ==============================================
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  GameConfig cfg;
  cfg.lanes = 3;          // tweak to 2..4 for variety
  cfg.roadWidth = 21;
  cfg.roadHeight = 24;

  GameState st;
  st.playerLane = min(max(0, cfg.lanes / 2), cfg.lanes - 1);

  // Randomness
  std::random_device rd;
  std::mt19937 rng(rd());

  // Set up terminal
  kb_setup();
  ansi_hide_cursor(true);

  // Intro screen
  ansi_clear();
  cout << "Car Runner (ASCII)\n\n"
       << "Dodge the falling obstacles.\n\n"
       << "Controls:\n"
       << "  A / D or ← / →  move\n"
       << "  P               pause\n"
       << "  Q               quit\n\n"
       << "Press any key to start...\n";
  cout.flush();

  // wait for first key
  while (!kb_hit()) {
    this_thread::sleep_for(chrono::milliseconds(10));
  }
  if (kb_hit()) (void)kb_getch();

  // Game loop
  auto last = chrono::steady_clock::now();

  while (true) {
    // Input
    applyInput(st, cfg);

    if (st.gameOver) {
      // Render final screen
      drawFrame(cfg, st);
      cout << "\nGame Over! Final score: " << st.score << "\n"
           << "Press R to restart, or Q to quit.\n";
      cout.flush();

      // Wait for R / Q
      while (true) {
        if (kb_hit()) {
          int c = kb_getch();
          if (c == 'q' || c == 'Q' || c == 27) {
            ansi_hide_cursor(false);
            kb_restore();
            cout << "\nThanks for playing!\n";
            return 0;
          }
          if (c == 'r' || c == 'R') {
            // reset state
            st = GameState{};
            st.playerLane = min(max(0, cfg.lanes / 2), cfg.lanes - 1);
            ansi_clear();
            break;
          }
        }
        this_thread::sleep_for(chrono::milliseconds(10));
      }
    }

    // Timing
    auto now = chrono::steady_clock::now();
    double elapsed = chrono::duration<double, milli>(now - last).count();
    if (elapsed < st.tickMs) {
      this_thread::sleep_for(chrono::milliseconds(1));
      continue;
    }
    last = now;

    // Update + Draw
    update(st, cfg, rng);
    drawFrame(cfg, st);
  }

  // Cleanup (not reached)
  ansi_hide_cursor(false);
  kb_restore();
  return 0;
}
