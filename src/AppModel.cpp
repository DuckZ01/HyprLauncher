#include "AppModel.hpp"
#include <algorithm>
#include <iostream>

AppModel::AppModel() { refresh(); }

AppModel::~AppModel() {
  for (auto &app : m_apps) {
    if (app.app_info) {
      g_object_unref(app.app_info);
    }
  }
}

void AppModel::refresh() {
  // Clear existing
  for (auto &app : m_apps) {
    if (app.app_info) {
      g_object_unref(app.app_info);
    }
  }
  m_apps.clear();

  GList *apps = g_app_info_get_all();
  GList *l;

  for (l = apps; l != NULL; l = l->next) {
    GAppInfo *info = (GAppInfo *)l->data;

    if (!g_app_info_should_show(info)) {
      continue;
    }

    AppEntry entry;
    const char *name = g_app_info_get_name(info);
    entry.name = name ? std::string(name) : "Unknown";

    GIcon *icon = g_app_info_get_icon(info);
    if (icon) {
      const char *icon_str = g_icon_to_string(icon);
      entry.icon_name =
          icon_str ? std::string(icon_str) : "application-x-executable";
      g_free((gpointer)icon_str);
    } else {
      entry.icon_name = "application-x-executable";
    }

    entry.app_info = (GAppInfo *)g_object_ref(info);

    m_apps.push_back(entry);
  }

  g_list_free_full(apps, g_object_unref);

  // Sort alphabetically
  std::sort(
      m_apps.begin(), m_apps.end(),
      [](const AppEntry &a, const AppEntry &b) { return a.name < b.name; });
}

const std::vector<AppEntry> &AppModel::get_all_apps() const { return m_apps; }

std::vector<AppEntry> AppModel::filter_apps(const std::string &query) const {
  if (query.empty()) {
    return m_apps;
  }

  std::vector<AppEntry> filtered;
  std::string q = query;
  std::transform(q.begin(), q.end(), q.begin(), ::tolower);

  for (const auto &app : m_apps) {
    std::string name = app.name;
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    if (name.find(q) != std::string::npos) {
      filtered.push_back(app);
    }
  }
  return filtered;
}

void AppModel::launch_app(const AppEntry &app) {
  if (app.app_info) {
    GError *error = NULL;
    g_app_info_launch(app.app_info, NULL, NULL, &error);
    if (error) {
      std::cerr << "Failed to launch app: " << error->message << std::endl;
      g_error_free(error);
    }
  }
}
