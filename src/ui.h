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
    
    // Login widgets
    GtkWidget *login_username_entry;
    GtkWidget *login_password_entry;
    GtkWidget *login_status_label;
    
    // Dashboard widgets
    GtkWidget *car_number_entry;
    GtkWidget *vehicle_combo;
    GtkWidget *garage_combo;
    GtkWidget *duration_combo;
    GtkWidget *exit_car_entry;
    GtkWidget *search_entry;
    GtkWidget *car_list_treeview;
    GtkListStore *car_list_store;
    GtkWidget *total_revenue_label;
    GtkWidget *garage_availability_label;
    GtkWidget *status_label;
    GtkWidget *sort_combo;
    
    int sort_column;
    int sort_order; // 0 = ascending, 1 = descending
} UIState;

// UI functions
void ui_init(UIState *ui, AppState *app_state, GtkApplication *app);
void ui_show_login(UIState *ui);
void ui_show_dashboard(UIState *ui);
void ui_update_car_list(UIState *ui);
void ui_update_garage_availability(UIState *ui);
void ui_update_revenue(UIState *ui);
void ui_show_error(UIState *ui, const char *message);
void ui_show_success(UIState *ui, const char *message);

// Callback functions (declared here for reference)
void on_login_clicked(GtkWidget *widget, gpointer data);
void on_add_car_clicked(GtkWidget *widget, gpointer data);
void on_exit_car_clicked(GtkWidget *widget, gpointer data);
void on_search_changed(GtkWidget *widget, gpointer data);
void on_clear_search_clicked(GtkWidget *widget, gpointer data);
void on_save_clicked(GtkWidget *widget, gpointer data);
void on_load_clicked(GtkWidget *widget, gpointer data);
void on_logout_clicked(GtkWidget *widget, gpointer data);
void on_exit_app_clicked(GtkWidget *widget, gpointer data);
void on_sort_changed(GtkWidget *widget, gpointer data);

#endif // UI_H

