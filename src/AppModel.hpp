#pragma once

#include <gtk/gtk.h>
#include <string>
#include <vector>
#include <memory>

struct AppEntry {
    std::string name;
    std::string icon_name;
    std::string exec;
    GAppInfo* app_info; // Keep reference to launch
};

class AppModel {
public:
    AppModel();
    ~AppModel();

    void refresh();
    const std::vector<AppEntry>& get_all_apps() const;
    std::vector<AppEntry> filter_apps(const std::string& query) const;

    void launch_app(const AppEntry& app);

private:
    std::vector<AppEntry> m_apps;
};
