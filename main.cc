#include <boost/asio/io_service.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/system_timer.hpp>

#include "NotifyService.h"
#include "Pomodoro.h"

void
timer_expires(boost::system::error_code const& ec,
              Notify::Notification& notification,
              boost::asio::system_timer& timer, Pomodoro::Application& app)
{
  app.next();
  notification.update(app.summary(), app.message());
  notification.show();
  timer.expires_from_now(app.next_duration());

  timer.async_wait(
    [&notification, &timer, &app](boost::system::error_code const& ec) {
      timer_expires(ec, notification, timer, app);
    });
}

void
sigusr1(boost::system::error_code const& ec, int signal_number,
        Notify::Notification& notification, boost::asio::signal_set& signals,
        Pomodoro::Application& app)
{
  notification.update(app.summary(), app.message());
  notification.show();

  signals.async_wait([&notification, &signals, &app](
                       boost::system::error_code const& ec, int signal_number) {
    sigusr1(ec, signal_number, notification, signals, app);
  });
}

int
main(int argc, char* argv[])
{
  boost::asio::io_service io_context;
  boost::asio::system_timer timer(io_context);
  boost::asio::signal_set signals(io_context, SIGUSR1);
  Notify::Service notify_service{ "pomodoro" };
  Pomodoro::Application app;

  Notify::Notification timer_notification{ app.summary(), app.message(), "" };
  timer_notification.show();
  Notify::Notification status_notification{ app.summary(), app.message(), "" };
  timer.expires_from_now(app.next_duration());

  timer.async_wait(
    [&timer_notification, &timer, &app](boost::system::error_code const& ec) {
      timer_expires(ec, timer_notification, timer, app);
    });

  signals.async_wait([&status_notification, &signals, &app](
                       boost::system::error_code const& ec, int signal_number) {
    sigusr1(ec, signal_number, status_notification, signals, app);
  });

  io_context.run();

  return 0;
}
