#include <chrono>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace Pomodoro {
using namespace std::chrono_literals;

class PomodoroState
{
public:
  virtual ~PomodoroState() = default;
  virtual std::chrono::minutes next_duration() const = 0;
  virtual std::string summary() const = 0;
  std::chrono::minutes next_timer() const;
  void next_endpoint();

private:
  std::chrono::system_clock::time_point endpoint;
};

class LongBreak : public PomodoroState
{
public:
  virtual std::chrono::minutes next_duration() const;
  virtual std::string summary() const;
};

class ShortBreak : public PomodoroState
{
public:
  virtual std::chrono::minutes next_duration() const;
  virtual std::string summary() const;
};

class FocusTime : public PomodoroState
{
public:
  virtual std::chrono::minutes next_duration() const;
  virtual std::string summary() const;
};

class Application
{
public:
  void next();
  std::chrono::minutes next_duration();
  std::string summary();
  std::string message();
  Application();

private:
  std::vector<std::unique_ptr<PomodoroState>>::iterator current;
  std::vector<std::unique_ptr<PomodoroState>> states;
};

} // namespace Pomodoro
