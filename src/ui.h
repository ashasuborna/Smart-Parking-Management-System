#ifndef UI_H
#define UI_H

#include <gtk/gtk.h>
#include "models.h"

// UI state structure
typedef struct {
    AppState *app_state;
    GtkApplication *app;
    GtkWidget *login_window;
    GtkWidget *dashboard_window;
    
    // REMOVED: Login widgets - not part of Suborna's tasks
    
    // Dashboard widgets
    GtkWidget *car_number_entry;
    GtkWidget *vehicle_combo;
    GtkWidget *garage_combo;
    GtkWidget *duration_combo;
    GtkWidget *exit_car_entry;
    // REMOVED: search_entry - not part of Suborna's tasks
    GtkWidget *car_list_treeview;
    GtkListStore *car_list_store;
    GtkWidget *total_revenue_label;
    GtkWidget *garage_availability_label;
    GtkWidget *status_label;
    // REMOVED: sort_combo - not part of Suborna's tasks
} UIState;

// UI functions
void ui_init(UIState *ui, AppState *app_state, GtkApplication *app);
// REMOVED: ui_show_login - not part of Suborna's tasks
void ui_show_dashboard(UIState *ui);
void ui_update_car_list(UIState *ui);
void ui_update_garage_availability(UIState *ui);
void ui_update_revenue(UIState *ui);
void ui_show_error(UIState *ui, const char *message);
void ui_show_success(UIState *ui, const char *message);

// Callback functions - Only Suborna's tasks
// REMOVED: on_login_clicked - not part of Suborna's tasks
void on_add_car_clicked(GtkWidget *widget, gpointer data);  // Suborna's: Car Entry
void on_exit_car_clicked(GtkWidget *widget, gpointer data); // Suborna's: Car Exit
// REMOVED: on_search_changed, on_clear_search_clicked - not part of Suborna's tasks
void on_save_clicked(GtkWidget *widget, gpointer data);   // Suborna's: File Handling
void on_load_clicked(GtkWidget *widget, gpointer data);   // Suborna's: File Handling
// REMOVED: on_logout_clicked, on_exit_app_clicked, on_sort_changed - not part of Suborna's tasks

#endif // UI_H

