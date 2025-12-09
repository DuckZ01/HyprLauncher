#pragma once

#include "AppModel.hpp"
#include <gtk/gtk.h>
#include <memory>

class LauncherWindow {
public:
  LauncherWindow(GtkApplication *app);
  ~LauncherWindow();

  void show();

  static gboolean on_key_press_static(GtkEventControllerKey *controller,
                                      guint keyval, guint keycode,
                                      GdkModifierType state,
                                      gpointer user_data);
  static void on_focus_change_static(GObject *object, GParamSpec *pspec,
                                     gpointer user_data);
  static void on_search_changed_static(GtkSearchEntry *entry,
                                       gpointer user_data);
  static void on_app_activated_static(GtkFlowBox *box, GtkFlowBoxChild *child,
                                      gpointer user_data);

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
