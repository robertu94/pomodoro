#include "Pomodoro.h"

namespace Pomodoro {

std::chrono::minutes
PomodoroState::next_timer() const
{
  return std::chrono::duration_cast<std::chrono::minutes>(
    endpoint - std::chrono::system_clock::now());
}

void
PomodoroState::next_endpoint()
{
  endpoint = std::chrono::system_clock::now() + next_duration();
}

std::chrono::minutes
LongBreak::next_duration() const
{
  return 20min;
}

std::string
LongBreak::summary() const
{
  return "Time to take a long break!";
}

std::chrono::minutes
ShortBreak::next_duration() const
{
  return 5min;
}

std::string
ShortBreak::summary() const
{
  return "Time to take a short break!";
}

std::chrono::minutes
FocusTime::next_duration() const
{
  return 25min;
}

std::string
FocusTime::summary() const
{
  return "Time to Focus!";
}

void
Application::next()
{
  current++;
  if (current == std::end(states))
    current = std::begin(states);
  (*current)->next_endpoint();
}

std::chrono::minutes
Application::next_duration()
{
  return (*current)->next_duration();
}

std::string
Application::summary()
{
  return (*current)->summary();
}

std::string
Application::message()
{
  std::stringstream message;
  message << (*current)->next_timer().count() << " minutes until the next one";
  return message.str();
}

Application::Application()
{
  states.emplace_back(std::make_unique<FocusTime>());
  states.emplace_back(std::make_unique<ShortBreak>());
  states.emplace_back(std::make_unique<FocusTime>());
  states.emplace_back(std::make_unique<ShortBreak>());
  states.emplace_back(std::make_unique<FocusTime>());
  states.emplace_back(std::make_unique<ShortBreak>());
  states.emplace_back(std::make_unique<FocusTime>());
  states.emplace_back(std::make_unique<LongBreak>());
  current = std::begin(states);
  (*current)->next_endpoint();
}
} // namespace Pomodoro
