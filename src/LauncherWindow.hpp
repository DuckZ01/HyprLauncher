#pragma once

#include "AppModel.hpp"
#include <gtk/gtk.h>
#include <memory>

class LauncherWindow {
public:
  LauncherWindow(GtkApplication *app);
  ~LauncherWindow();

  void show();

  friend gboolean on_key_press_static(GtkEventControllerKey *controller,
                                      guint keyval, guint keycode,
                                      GdkModifierType state,
                                      gpointer user_data);
  friend void on_focus_change_static(GObject *object, GParamSpec *pspec,
                                     gpointer user_data);
  friend void search_changed_cb(GtkSearchEntry *entry,
                                gpointer user_data); // if needed

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
