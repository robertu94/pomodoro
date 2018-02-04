#include <glib-object.h>

namespace GPropertyHelper {

struct GIntH
{
  using c_value = int;
  static void free(GValue*) {} // intentional no-op; no free function
  static c_value convert(const GValue* v) { return g_value_get_int(v); }
};

struct GObjectH
{
  using c_value = GObject*;
  static void free(GValue* obj) {} // intentional no-op; returning object
  static c_value convert(const GValue* v)
  {
    return G_OBJECT(g_value_get_object(v));
  }
};

struct GCharH
{
  using c_value = std::string;
  static void free(GValue* str) { g_free(str); }
  static c_value convert(const GValue* v) { return g_value_get_string(v); }
};

template <class T>
typename T::c_value
get(gpointer object, std::string const& property)
{
  GValue val = G_VALUE_INIT;
  g_object_get_property(G_OBJECT(object), property.c_str(), &val);
  typename T::c_value ret = T::convert(&val);
  T::free(&val);
  return ret;
}
} // namespace GPropertyHelper
