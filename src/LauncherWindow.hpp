#pragma once

#include "AppModel.hpp"
#include <gtk/gtk.h>
#include <memory>

class LauncherWindow {
public:
  LauncherWindow(GtkApplication *app);
  ~LauncherWindow();

  void show();

private:
  void setup_ui();
  void load_css();
  void populate_apps(const std::vector<AppEntry> &apps);
  void on_search_changed(GtkSearchEntry *entry);
  void on_app_activated(GtkFlowBox *box, GtkFlowBoxChild *child);

  GtkWidget *m_window;
  GtkWidget *m_search_entry;
  GtkWidget *m_flowbox;
  std::unique_ptr<AppModel> m_app_model;
};
