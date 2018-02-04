#include <chrono>
#include <libnotify/notification.h>
#include <stdexcept>
#include <string>

namespace Notify {
class Notification
{
public:
  Notification(std::string const& summary, std::string const& body = "",
               std::string const& icon = "");
  Notification(Notification const& lhs);
  Notification& operator=(Notification const& lhs);
  ~Notification();

  void update(std::string const& summary, std::string const& body = "",
              std::string const& icon = "");
  void setTimeout(std::chrono::microseconds const& timeout) noexcept;
  void setTimeout(int millisecond) noexcept;
  void setAppName(std::string const& name) noexcept;
  void setCategory(std::string const& category) noexcept;
  void setImage(GdkPixbuf* pixbuf) noexcept;
  void setHint(std::string const& key, GVariant* value) noexcept;
  void clearHints() noexcept;
  void addAction(std::string const& action, std::string const& label,
                 NotifyActionCallback callback, gpointer user_data,
                 GFreeFunc free_func) noexcept;
  void clearActions() noexcept;
  bool show();
  bool close();
  gint getClosedReason();

private:
  NotifyNotification* impl;
};

class Service
{
public:
  Service(std::string const& app_name);
  ~Service();
};
} // namespace Notify
