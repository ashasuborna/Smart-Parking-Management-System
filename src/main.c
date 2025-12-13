#include <gtk/gtk.h>
#include "models.h"
#include "parking.h"
#include "fileio.h"
#include "billing.h"
#include "ui.h"

#ifdef ENABLE_MYSQL
#include "db.h"
#endif

static AppState app_state;
static UIState ui_state;

static void on_activate(GtkApplication *app, gpointer user_data) {
    (void)user_data; // Suppress unused parameter warning
    // Initialize app state
    init_app_state(&app_state);
    
    // Initialize garages (default or load from file)
    initialize_default_garages(app_state.garages, &app_state.garage_count);
    load_garages(app_state.garages, &app_state.garage_count);
    if (app_state.garage_count == 0) {
        initialize_default_garages(app_state.garages, &app_state.garage_count);
        save_garages(app_state.garages, app_state.garage_count);
    }
    
    // Load data
    ensure_data_directory();
    load_active_cars(app_state.active_cars, &app_state.active_count);
    load_history_cars(app_state.history_cars, &app_state.history_count);
    
    // Recalculate garage availability
    for (int i = 0; i < app_state.garage_count; i++) {
        app_state.garages[i].available_slots = app_state.garages[i].total_capacity;
    }
    for (int i = 0; i < app_state.active_count; i++) {
        update_garage_availability(app_state.garages, app_state.garage_count,
                                   app_state.active_cars[i].garage_id, -1);
    }
    
    // Calculate total revenue
    app_state.total_revenue = get_total_revenue(app_state.history_cars, app_state.history_count);
    
#ifdef ENABLE_MYSQL
    // Initialize MySQL if enabled
    MYSQL *conn = db_connect();
    if (conn) {
        db_init_tables(conn);
        db_disconnect(conn);
    }
#endif
    
    // Initialize UI
    ui_init(&ui_state, &app_state, app);
    ui_show_login(&ui_state);
}

static void on_shutdown(GtkApplication *app, gpointer user_data) {
    (void)app;
    (void)user_data; // Suppress unused parameter warnings
    // Save data on exit
    ensure_data_directory();
    save_active_cars(app_state.active_cars, app_state.active_count);
    save_history_cars(app_state.history_cars, app_state.history_count);
    save_garages(app_state.garages, app_state.garage_count);
}

int main(int argc, char *argv[]) {
    GtkApplication *app;
    int status;
    
    app = gtk_application_new("com.smartparking.system", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    g_signal_connect(app, "shutdown", G_CALLBACK(on_shutdown), NULL);
    
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    
    return status;
}

