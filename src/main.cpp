#include "LauncherWindow.hpp"
#include <gtk/gtk.h>

static void on_activate(GtkApplication *app, gpointer user_data) {
  // Determine if window is already created?
  // For a launcher: usually we create a new window instance or show existing.
  // Here we just create one.
  // We need to manage the lifetime of LauncherWindow properly.
  // For simplicity, we can just `new` it and let it manage itself or attach to
  // app.

  // We use a static pointer or just leak it for the app duration (common in
  // simple apps). Or attach to the application window list.
  static LauncherWindow *window = nullptr;
  if (!window) {
    window = new LauncherWindow(app);
    window->show();
  } else {
    window->show();
  }
}

int main(int argc, char *argv[]) {
  // Create GtkApplication
  // "com.duckz.launcher"
  GtkApplication *app =
      gtk_application_new("com.duckz.launcher", G_APPLICATION_DEFAULT_FLAGS);

  g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

  int status = g_application_run(G_APPLICATION(app), argc, argv);

  g_object_unref(app);
  return status;
}
