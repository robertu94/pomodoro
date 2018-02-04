#include "NotifyService.h"

#include <libnotify/notify.h>
#include <stdexcept>

#include "GPropertyHelper.h"

namespace Notify {
Service::Service(std::string const& app_name)
{
  if (!notify_init(app_name.c_str())) {
    throw std::runtime_error("failed to init libnotify");
  }
}

Service::~Service()
{
  notify_uninit();
}

Notification::Notification(std::string const& summary, std::string const& body,
                           std::string const& icon)
  : impl(notify_notification_new(summary.c_str(), body.c_str(), icon.c_str()))
{
  if (!impl) {
    throw std::runtime_error("failed to create notification");
  }
}

Notification::Notification(Notification const& lhs)
  : impl(notify_notification_new(
      GPropertyHelper::get<GPropertyHelper::GCharH>(lhs.impl, "summary")
        .c_str(),
      GPropertyHelper::get<GPropertyHelper::GCharH>(lhs.impl, "body").c_str(),
      GPropertyHelper::get<GPropertyHelper::GCharH>(lhs.impl, "icon").c_str()))
{
  if (!impl) {
    throw std::runtime_error("failed to create notification");
  }
}

Notification&
Notification::operator=(Notification const& lhs)
{
  notify_notification_update(
    impl,
    GPropertyHelper::get<GPropertyHelper::GCharH>(lhs.impl, "summary").c_str(),
    GPropertyHelper::get<GPropertyHelper::GCharH>(lhs.impl, "body").c_str(),
    GPropertyHelper::get<GPropertyHelper::GCharH>(lhs.impl, "icon").c_str());
  return *this;
}

void
Notification::update(std::string const& summary, std::string const& body,
                     std::string const& icon)
{
  notify_notification_update(impl, summary.c_str(), body.c_str(), icon.c_str());
}

Notification::~Notification()
{
  g_object_unref(G_OBJECT(impl));
}

void
Notification::setTimeout(std::chrono::microseconds const& timeout) noexcept
{
  notify_notification_set_timeout(impl, timeout.count());
}

void
Notification::setTimeout(int millisecond) noexcept
{
  notify_notification_set_timeout(impl, millisecond);
}

void
Notification::setAppName(std::string const& name) noexcept
{
  notify_notification_set_app_name(impl, name.c_str());
}

void
Notification::setCategory(std::string const& category) noexcept
{
  notify_notification_set_category(impl, category.c_str());
}

void
Notification::setImage(GdkPixbuf* pixbuf) noexcept
{
  notify_notification_set_image_from_pixbuf(impl, pixbuf);
}

void
Notification::setHint(std::string const& key, GVariant* value) noexcept
{
  notify_notification_set_hint(impl, key.c_str(), value);
}

void
Notification::clearHints() noexcept
{
  notify_notification_clear_hints(impl);
}

void
Notification::addAction(std::string const& action, std::string const& label,
                        NotifyActionCallback callback, gpointer user_data,
                        GFreeFunc free_func) noexcept
{
  notify_notification_add_action(impl, action.c_str(), label.c_str(), callback,
                                 user_data, free_func);
}

void
Notification::clearActions() noexcept
{
  notify_notification_clear_actions(impl);
}

bool
Notification::show()
{
  GError* error = nullptr;
  auto ret = notify_notification_show(impl, &error);
  if (error) {
    throw std::runtime_error(error->message);
  }
  return ret;
}

bool
Notification::close()
{
  GError* error = nullptr;
  return notify_notification_close(impl, &error);
  if (error) {
    throw std::runtime_error(error->message);
  }
}

gint
Notification::getClosedReason()
{
  return notify_notification_get_closed_reason(impl);
}

} // namespace Notify
