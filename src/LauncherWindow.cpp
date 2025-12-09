#include "LauncherWindow.hpp"
#include <glib-object.h>
#include <gtk4-layer-shell.h>
#include <iostream>

// Callback wrappers
// Callback wrappers removed, using static members

// Forward declarations of static callbacks
// Forward declarations removed

LauncherWindow::LauncherWindow(GtkApplication *app) {
  m_app_model = std::make_unique<AppModel>();

  m_window = gtk_application_window_new(app);

  // Layer Shell Setup
  gtk_layer_init_for_window(GTK_WINDOW(m_window));
  gtk_layer_set_layer(GTK_WINDOW(m_window), GTK_LAYER_SHELL_LAYER_OVERLAY);
  gtk_layer_set_namespace(GTK_WINDOW(m_window), "launcher");

  // Keyboarding: Exclusive to type immediately
  gtk_layer_set_keyboard_mode(GTK_WINDOW(m_window),
                              GTK_LAYER_SHELL_KEYBOARD_MODE_EXCLUSIVE);

  // Anchors: Center it, or fill screen?
  // User asked for "something like rofi", which is usually centered or top.
  // Let's make it a centered box, not fullscreen.
  // To do that, we anchor on none (default center) or set specific margins.
  // Actually, rofi acts often as a centered window.
  // But to have a background blur on the whole screen vs just the window?
  // If we want "transparent with a blur", often we want the whole screen
  // covered or a big box. Let's make a decently sized window centered.

  gtk_widget_set_size_request(m_window, 600, 400); // Initial size

  // Apply CSS
  load_css();

  setup_ui();
}

LauncherWindow::~LauncherWindow() {}

void LauncherWindow::show() { gtk_window_present(GTK_WINDOW(m_window)); }

void LauncherWindow::load_css() {
  GtkCssProvider *provider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(provider, "resources/style.css");

  gtk_style_context_add_provider_for_display(
      gdk_display_get_default(), GTK_STYLE_PROVIDER(provider),
      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  g_object_unref(provider);
}

void LauncherWindow::setup_ui() {
  // Main container: Vertical Box
  GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_set_margin_start(vbox, 20);
  gtk_widget_set_margin_end(vbox, 20);
  gtk_widget_set_margin_top(vbox, 20);
  gtk_widget_set_margin_bottom(vbox, 20);
  gtk_window_set_child(GTK_WINDOW(m_window), vbox);

  // Search Bar
  m_search_entry = gtk_search_entry_new();
  gtk_box_append(GTK_BOX(vbox), m_search_entry);
  g_signal_connect(m_search_entry, "search-changed",
                   G_CALLBACK(on_search_changed_static), this);

  // Capture Escape key to close
  GtkEventController *controller = gtk_event_controller_key_new();
  g_signal_connect(controller, "key-pressed", G_CALLBACK(on_key_press_static),
                   this);
  g_signal_connect(controller, "key-pressed", G_CALLBACK(on_key_press_static),
                   this);
  gtk_widget_add_controller(m_window, controller);

  // Close on lost focus (click outside)
  g_signal_connect(m_window, "notify::is-active",
                   G_CALLBACK(on_focus_change_static), this);

  // Scrolled Window for apps
  GtkWidget *scrolled = gtk_scrolled_window_new();
  gtk_widget_set_vexpand(scrolled, TRUE);
  gtk_box_append(GTK_BOX(vbox), scrolled);

  // FlowBox for App Grid
  m_flowbox = gtk_flow_box_new();
  // gtk_flow_box_set_valign(GTK_FLOW_BOX(m_flowbox), GTK_ALIGN_START); //
  // REMOVED: Obsolete in GTK4
  gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(m_flowbox), 5);
  gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(m_flowbox),
                                  GTK_SELECTION_SINGLE);
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), m_flowbox);

  g_signal_connect(m_flowbox, "child-activated",
                   G_CALLBACK(on_app_activated_static), this);

  // Populate initial
  populate_apps(m_app_model->get_all_apps());
}

void LauncherWindow::populate_apps(const std::vector<AppEntry> &apps) {
  gtk_flow_box_remove_all(GTK_FLOW_BOX(m_flowbox));

  for (const auto &app : apps) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    GtkWidget *icon = gtk_image_new_from_icon_name(app.icon_name.c_str());
    gtk_image_set_pixel_size(GTK_IMAGE(icon), 48);
    gtk_box_append(GTK_BOX(box), icon);

    GtkWidget *label = gtk_label_new(app.name.c_str());
    gtk_label_set_ellipsize(GTK_LABEL(label), PANGO_ELLIPSIZE_END);
    gtk_label_set_max_width_chars(GTK_LABEL(label), 10);
    gtk_box_append(GTK_BOX(box), label);

    // Store implicit data? We can just re-lookup or use g_object_set_data
    // Or simpler: We are just filtering.
    // For launch, we need the AppEntry.
    // Let's store the index or name in the widget data.
    // But the indices change on filter.
    // Let's store the pointer to AppEntry in the widget, BUT AppEntries are in
    // a vector that might realloc? No, 'filter_apps' returns copies. Better:
    // Store the specific GAppInfo* or just the command. Let's attach the app
    // name as string data.

    // Wait, GtkFlowBoxChild is created automatically or we wrap it?
    // We append the 'box' to the flowbox. The flowbox creates a child wrapper.
    // We can set data on the 'box' widget.

    g_object_set_data_full(
        G_OBJECT(box), "app_launch_context",
        g_strdup(app.app_info ? g_app_info_get_id(app.app_info) : ""), g_free);

    // Actually, we need the AppEntry to launch.
    // To be safe, let's keep it simple: We have the model.
    // When activated, we can search the model for the app name/ID.

    gtk_flow_box_append(GTK_FLOW_BOX(m_flowbox), box);
  }
}

void LauncherWindow::on_search_changed(GtkSearchEntry *entry) {
  const char *text = gtk_editable_get_text(GTK_EDITABLE(entry));
  auto filtered = m_app_model->filter_apps(text ? text : "");
  populate_apps(filtered);
}

void LauncherWindow::on_app_activated(GtkFlowBox *box, GtkFlowBoxChild *child) {
  GtkWidget *widget = gtk_flow_box_child_get_child(child); // This is our VBox
  // We need to identify which app this is.
  // In populate, we set data on the VBox.
  const char *app_id =
      (const char *)g_object_get_data(G_OBJECT(widget), "app_launch_context");

  if (app_id) {
    // Find in model (naive search)
    for (const auto &app : m_app_model->get_all_apps()) {
      if (app.app_info &&
          g_strcmp0(g_app_info_get_id(app.app_info), app_id) == 0) {
        m_app_model->launch_app(app);
        // Close launcher
        g_application_quit(
            G_APPLICATION(gtk_window_get_application(GTK_WINDOW(m_window))));
        break;
      }
    }
  }
}

// Static Trampolines
void LauncherWindow::on_search_changed_static(GtkSearchEntry *entry,
                                              gpointer user_data) {
  auto *self = static_cast<LauncherWindow *>(user_data);
  self->on_search_changed(entry);
}

void LauncherWindow::on_app_activated_static(GtkFlowBox *box,
                                             GtkFlowBoxChild *child,
                                             gpointer user_data) {
  auto *self = static_cast<LauncherWindow *>(user_data);
  self->on_app_activated(box, child);
}

gboolean LauncherWindow::on_key_press_static(GtkEventControllerKey *controller,
                                             guint keyval, guint keycode,
                                             GdkModifierType state,
                                             gpointer user_data) {
  if (keyval == GDK_KEY_Escape) {
    // auto *self = static_cast<LauncherWindow *>(user_data);
    g_application_quit(G_APPLICATION(g_application_get_default()));
    return TRUE;
  }
  return FALSE;
}

void LauncherWindow::on_focus_change_static(GObject *object, GParamSpec *pspec,
                                            gpointer user_data) {
  // auto *self = static_cast<LauncherWindow *>(user_data);
  GtkWindow *window = GTK_WINDOW(object);
  if (!gtk_window_is_active(window)) {
    g_application_quit(G_APPLICATION(g_application_get_default()));
  }
}
