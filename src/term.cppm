module;
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
export module term;
export namespace term {

class term {
public:
  term() {
    tcgetattr(STDIN_FILENO, &oldt);

    termios t = oldt;
    t.c_lflag &= ~(ICANON | ECHO); // 不等回车，不回显
    tcsetattr(STDIN_FILENO, TCSANOW, &t);

    old_flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, old_flags | O_NONBLOCK);
  }

  ~term() {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, old_flags);
  }

  bool has_key() const {
    char c;
    return ::read(STDIN_FILENO, &c, 1) > 0;
  }

  char get_key() const {
    char c = 0;
    ::read(STDIN_FILENO, &c, 1);
    return c;
  }

private:
  termios oldt{};
  int old_flags{};
};

} // namespace mrender
