#include "ui.h"
#include "car_entry_exit.h"
#include "file_handling.h"
#include <string.h>
#include <stdio.h>

#ifdef ENABLE_MYSQL
#include "mysql_integration.h"
#endif

static UIState *g_ui_state = NULL;

void ui_show_error(UIState *ui, const char *message) {
    if (ui->status_label) {
        char msg[200];
        snprintf(msg, sizeof(msg), "<span color='red'>%s</span>", message);
        gtk_label_set_markup(GTK_LABEL(ui->status_label), msg);
    }
}

void ui_show_success(UIState *ui, const char *message) {
    if (ui->status_label) {
        char msg[200];
        snprintf(msg, sizeof(msg), "<span color='green'>%s</span>", message);
        gtk_label_set_markup(GTK_LABEL(ui->status_label), msg);
    }
}

void ui_update_revenue(UIState *ui) {
    float total = get_total_revenue(ui->app_state->history_cars, ui->app_state->history_count);
    char text[100];
    snprintf(text, sizeof(text), "<span size='x-large' weight='bold'>$%.2f</span>", total);
    gtk_label_set_markup(GTK_LABEL(ui->total_revenue_label), text);
}

void ui_update_garage_availability(UIState *ui) {
    char text[500] = "";
    char line[100];
    
    for (int i = 0; i < ui->app_state->garage_count; i++) {
        snprintf(line, sizeof(line), "%s: %d/%d slots available\n",
                ui->app_state->garages[i].name,
                ui->app_state->garages[i].available_slots,
                ui->app_state->garages[i].total_capacity);
        strcat(text, line);
    }
    
    gtk_label_set_text(GTK_LABEL(ui->garage_availability_label), text);
}

void ui_update_car_list(UIState *ui) {
    gtk_list_store_clear(ui->car_list_store);
    
    for (int i = 0; i < ui->app_state->active_count; i++) {
        char entry_time_str[50];
        format_timestamp(ui->app_state->active_cars[i].entry_time, entry_time_str, sizeof(entry_time_str));
        
        GtkTreeIter iter;
        gtk_list_store_append(ui->car_list_store, &iter);
        gtk_list_store_set(ui->car_list_store, &iter,
                          0, ui->app_state->active_cars[i].car_number,
                          1, vehicle_type_to_string(ui->app_state->active_cars[i].vehicle_type),
                          2, ui->app_state->active_cars[i].garage_id,
                          3, ui->app_state->active_cars[i].duration_hours,
                          4, ui->app_state->active_cars[i].bill_amount,
                          5, entry_time_str,
                          -1);
    }
}

void on_add_car_clicked(GtkWidget *widget, gpointer data) {
    (void)widget;
    UIState *ui = (UIState *)data;
    
    const char *car_number = gtk_entry_get_text(GTK_ENTRY(ui->car_number_entry));
    if (strlen(car_number) == 0) {
        ui_show_error(ui, "Please enter car number");
        return;
    }
    
    int garage_idx = gtk_combo_box_get_active(GTK_COMBO_BOX(ui->garage_combo));
    int garage_id = ui->app_state->garages[garage_idx].id;
    VehicleType vehicle_type = (VehicleType)gtk_combo_box_get_active(GTK_COMBO_BOX(ui->vehicle_combo));
    int duration = gtk_combo_box_get_active(GTK_COMBO_BOX(ui->duration_combo)) + 1;
    
    int result = add_car_entry(ui->app_state, car_number, vehicle_type, garage_id, duration);
    
    if (result == 0) {
        char msg[200];
        float bill = calculate_bill(&ui->app_state->garages[garage_idx], vehicle_type, duration);
        snprintf(msg, sizeof(msg), "Car %s added! Bill: $%.2f", car_number, bill);
        ui_show_success(ui, msg);
        
        gtk_entry_set_text(GTK_ENTRY(ui->car_number_entry), "");
        
        gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(ui->garage_combo));
        for (int i = 0; i < ui->app_state->garage_count; i++) {
            char buffer[200];
            snprintf(buffer, sizeof(buffer), "%d. %s (%d available)", 
                     ui->app_state->garages[i].id, ui->app_state->garages[i].name,
                     ui->app_state->garages[i].available_slots);
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(ui->garage_combo), buffer);
        }
        gtk_combo_box_set_active(GTK_COMBO_BOX(ui->garage_combo), 0);
        
        ui_update_car_list(ui);
        ui_update_garage_availability(ui);
        
        save_active_cars(ui->app_state->active_cars, ui->app_state->active_count);
        
#ifdef ENABLE_MYSQL
        MYSQL *conn = db_connect();
        if (conn) {
            int idx = find_active_car(ui->app_state, car_number);
            if (idx >= 0) {
                db_add_active_car(conn, &ui->app_state->active_cars[idx]);
            }
            db_disconnect(conn);
        }
#endif
    } else {
        if (result == -1) {
            ui_show_error(ui, "Invalid car number");
        } else if (result == -2) {
            ui_show_error(ui, "Invalid duration (1-12 hours)");
        } else if (result == -3) {
            ui_show_error(ui, "Maximum cars reached");
        } else if (result == -4) {
            ui_show_error(ui, "Car already parked");
        } else if (result == -5) {
            ui_show_error(ui, "Garage not found");
        } else if (result == -6) {
            ui_show_error(ui, "Garage is full");
        }
    }
}

void on_exit_car_clicked(GtkWidget *widget, gpointer data) {
    (void)widget;
    UIState *ui = (UIState *)data;
    
    const char *car_number = gtk_entry_get_text(GTK_ENTRY(ui->exit_car_entry));
    if (strlen(car_number) == 0) {
        ui_show_error(ui, "Please enter car number");
        return;
    }
    
    int result = exit_car(ui->app_state, car_number);
    
    if (result == 0) {
        char msg[200];
        snprintf(msg, sizeof(msg), "Car %s exited successfully", car_number);
        ui_show_success(ui, msg);
        
        gtk_entry_set_text(GTK_ENTRY(ui->exit_car_entry), "");
        
        gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(ui->garage_combo));
        for (int i = 0; i < ui->app_state->garage_count; i++) {
            char buffer[200];
            snprintf(buffer, sizeof(buffer), "%d. %s (%d available)", 
                     ui->app_state->garages[i].id, ui->app_state->garages[i].name,
                     ui->app_state->garages[i].available_slots);
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(ui->garage_combo), buffer);
        }
        gtk_combo_box_set_active(GTK_COMBO_BOX(ui->garage_combo), 0);
        
        ui_update_car_list(ui);
        ui_update_garage_availability(ui);
        ui_update_revenue(ui);
        
        save_active_cars(ui->app_state->active_cars, ui->app_state->active_count);
        
#ifdef ENABLE_MYSQL
        MYSQL *conn = db_connect();
        if (conn) {
            db_remove_active_car(conn, car_number);
            int idx = ui->app_state->history_count - 1;
            if (idx >= 0) {
                db_add_history_car(conn, &ui->app_state->history_cars[idx]);
            }
            db_disconnect(conn);
        }
#endif
    } else {
        ui_show_error(ui, "Car not found in active list");
    }
}

void on_save_clicked(GtkWidget *widget, gpointer data) {
    (void)widget; // Suppress unused parameter warning
    UIState *ui = (UIState *)data;
    save_active_cars(ui->app_state->active_cars, ui->app_state->active_count);
    save_history_cars(ui->app_state->history_cars, ui->app_state->history_count);
    save_garages(ui->app_state->garages, ui->app_state->garage_count);
    ui_show_success(ui, "Data saved successfully");
}

void on_load_clicked(GtkWidget *widget, gpointer data) {
    (void)widget; // Suppress unused parameter warning
    UIState *ui = (UIState *)data;
    load_active_cars(ui->app_state->active_cars, &ui->app_state->active_count);
    load_history_cars(ui->app_state->history_cars, &ui->app_state->history_count);
    load_garages(ui->app_state->garages, &ui->app_state->garage_count);
    
    for (int i = 0; i < ui->app_state->garage_count; i++) {
        ui->app_state->garages[i].available_slots = ui->app_state->garages[i].total_capacity;
    }
    for (int i = 0; i < ui->app_state->active_count; i++) {
        update_garage_availability(ui->app_state->garages, ui->app_state->garage_count,
                                   ui->app_state->active_cars[i].garage_id, -1);
    }
    
    ui_update_car_list(ui);
    ui_update_garage_availability(ui);
    ui_update_revenue(ui);
    
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(ui->garage_combo));
    for (int i = 0; i < ui->app_state->garage_count; i++) {
        char buffer[200];
        snprintf(buffer, sizeof(buffer), "%d. %s (%d available)", 
                 ui->app_state->garages[i].id, ui->app_state->garages[i].name,
                 ui->app_state->garages[i].available_slots);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(ui->garage_combo), buffer);
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(ui->garage_combo), 0);
    
    ui_show_success(ui, "Data loaded successfully");
}

void ui_show_login(UIState *ui) {
    (void)ui;
}

void ui_show_dashboard(UIState *ui) {
    GtkWidget *window, *main_box, *left_panel, *right_panel;
    GtkWidget *entry_frame, *exit_frame, *list_frame;
    GtkWidget *grid, *button_box;
    
    window = gtk_application_window_new(ui->app);
    gtk_window_set_title(GTK_WINDOW(window), "Smart Parking Management System - Dashboard");
    gtk_window_set_default_size(GTK_WINDOW(window), 1200, 700);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    
    main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_add(GTK_CONTAINER(window), main_box);
    gtk_container_set_border_width(GTK_CONTAINER(main_box), 10);
    
    left_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(main_box), left_panel, FALSE, FALSE, 0);
    gtk_widget_set_size_request(left_panel, 400, -1);
    
    entry_frame = gtk_frame_new("Add Car Entry");
    gtk_box_pack_start(GTK_BOX(left_panel), entry_frame, FALSE, FALSE, 0);
    
    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(entry_frame), grid);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 15);
    
    GtkWidget *car_label = gtk_label_new("Car Number:");
    gtk_widget_set_halign(car_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), car_label, 0, 0, 1, 1);
    
    ui->car_number_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(ui->car_number_entry), "e.g., ABC-1234");
    gtk_grid_attach(GTK_GRID(grid), ui->car_number_entry, 1, 0, 1, 1);
    
    GtkWidget *vehicle_label = gtk_label_new("Vehicle Type:");
    gtk_widget_set_halign(vehicle_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), vehicle_label, 0, 1, 1, 1);
    
    ui->vehicle_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(ui->vehicle_combo), "Car");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(ui->vehicle_combo), "Bike");
    gtk_combo_box_set_active(GTK_COMBO_BOX(ui->vehicle_combo), 0);
    gtk_grid_attach(GTK_GRID(grid), ui->vehicle_combo, 1, 1, 1, 1);
    
    GtkWidget *garage_label = gtk_label_new("Garage:");
    gtk_widget_set_halign(garage_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), garage_label, 0, 2, 1, 1);
    
    ui->garage_combo = gtk_combo_box_text_new();
    for (int i = 0; i < ui->app_state->garage_count; i++) {
        char buffer[200];
        snprintf(buffer, sizeof(buffer), "%d. %s (%d available)", 
                 ui->app_state->garages[i].id, ui->app_state->garages[i].name,
                 ui->app_state->garages[i].available_slots);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(ui->garage_combo), buffer);
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(ui->garage_combo), 0);
    gtk_grid_attach(GTK_GRID(grid), ui->garage_combo, 1, 2, 1, 1);
    
    GtkWidget *duration_label = gtk_label_new("Duration (hours):");
    gtk_widget_set_halign(duration_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), duration_label, 0, 3, 1, 1);
    
    ui->duration_combo = gtk_combo_box_text_new();
    for (int i = 1; i <= 12; i++) {
        char buffer[20];
        snprintf(buffer, sizeof(buffer), "%d", i);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(ui->duration_combo), buffer);
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(ui->duration_combo), 0);
    gtk_grid_attach(GTK_GRID(grid), ui->duration_combo, 1, 3, 1, 1);
    
    GtkWidget *add_button = gtk_button_new_with_label("Add Car");
    gtk_widget_set_size_request(add_button, -1, 40);
    g_signal_connect(add_button, "clicked", G_CALLBACK(on_add_car_clicked), ui);
    gtk_grid_attach(GTK_GRID(grid), add_button, 0, 4, 2, 1);
    
    exit_frame = gtk_frame_new("Exit Car");
    gtk_box_pack_start(GTK_BOX(left_panel), exit_frame, FALSE, FALSE, 0);
    
    GtkWidget *exit_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(exit_frame), exit_box);
    gtk_container_set_border_width(GTK_CONTAINER(exit_box), 15);
    
    GtkWidget *exit_label = gtk_label_new("Car Number:");
    gtk_widget_set_halign(exit_label, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(exit_box), exit_label, FALSE, FALSE, 0);
    
    ui->exit_car_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(ui->exit_car_entry), "Enter car number");
    gtk_box_pack_start(GTK_BOX(exit_box), ui->exit_car_entry, FALSE, FALSE, 0);
    
    GtkWidget *exit_button = gtk_button_new_with_label("Exit Car");
    gtk_widget_set_size_request(exit_button, -1, 40);
    g_signal_connect(exit_button, "clicked", G_CALLBACK(on_exit_car_clicked), ui);
    gtk_box_pack_start(GTK_BOX(exit_box), exit_button, FALSE, FALSE, 0);
    
    GtkWidget *avail_frame = gtk_frame_new("Garage Availability");
    gtk_box_pack_start(GTK_BOX(left_panel), avail_frame, FALSE, FALSE, 0);
    
    ui->garage_availability_label = gtk_label_new("");
    gtk_label_set_justify(GTK_LABEL(ui->garage_availability_label), GTK_JUSTIFY_LEFT);
    gtk_container_add(GTK_CONTAINER(avail_frame), ui->garage_availability_label);
    gtk_container_set_border_width(GTK_CONTAINER(avail_frame), 10);
    
    ui->status_label = gtk_label_new("Ready");
    gtk_label_set_justify(GTK_LABEL(ui->status_label), GTK_JUSTIFY_LEFT);
    gtk_widget_set_halign(ui->status_label, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(left_panel), ui->status_label, FALSE, FALSE, 0);
    
    right_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(main_box), right_panel, TRUE, TRUE, 0);
    
    list_frame = gtk_frame_new("Active Parked Cars");
    gtk_box_pack_start(GTK_BOX(right_panel), list_frame, TRUE, TRUE, 0);
    
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(list_frame), scrolled);
    
    ui->car_list_store = gtk_list_store_new(6, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_FLOAT, G_TYPE_STRING);
    ui->car_list_treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(ui->car_list_store));
    
    gtk_tree_view_append_column(GTK_TREE_VIEW(ui->car_list_treeview),
                                gtk_tree_view_column_new_with_attributes("Car Number", gtk_cell_renderer_text_new(), "text", 0, NULL));
    gtk_tree_view_append_column(GTK_TREE_VIEW(ui->car_list_treeview),
                                gtk_tree_view_column_new_with_attributes("Vehicle", gtk_cell_renderer_text_new(), "text", 1, NULL));
    gtk_tree_view_append_column(GTK_TREE_VIEW(ui->car_list_treeview),
                                gtk_tree_view_column_new_with_attributes("Garage ID", gtk_cell_renderer_text_new(), "text", 2, NULL));
    gtk_tree_view_append_column(GTK_TREE_VIEW(ui->car_list_treeview),
                                gtk_tree_view_column_new_with_attributes("Duration", gtk_cell_renderer_text_new(), "text", 3, NULL));
    gtk_tree_view_append_column(GTK_TREE_VIEW(ui->car_list_treeview),
                                gtk_tree_view_column_new_with_attributes("Bill", gtk_cell_renderer_text_new(), "text", 4, NULL));
    gtk_tree_view_append_column(GTK_TREE_VIEW(ui->car_list_treeview),
                                gtk_tree_view_column_new_with_attributes("Entry Time", gtk_cell_renderer_text_new(), "text", 5, NULL));
    
    gtk_container_add(GTK_CONTAINER(scrolled), ui->car_list_treeview);
    
    button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(right_panel), button_box, FALSE, FALSE, 0);
    
    GtkWidget *save_button = gtk_button_new_with_label("Save");
    g_signal_connect(save_button, "clicked", G_CALLBACK(on_save_clicked), ui);
    gtk_box_pack_start(GTK_BOX(button_box), save_button, TRUE, TRUE, 0);
    
    GtkWidget *load_button = gtk_button_new_with_label("Load");
    g_signal_connect(load_button, "clicked", G_CALLBACK(on_load_clicked), ui);
    gtk_box_pack_start(GTK_BOX(button_box), load_button, TRUE, TRUE, 0);
    
    GtkWidget *revenue_frame = gtk_frame_new("Total Revenue");
    gtk_box_pack_start(GTK_BOX(right_panel), revenue_frame, FALSE, FALSE, 0);
    
    ui->total_revenue_label = gtk_label_new("$0.00");
    gtk_label_set_markup(GTK_LABEL(ui->total_revenue_label), "<span size='xx-large' weight='bold'>$0.00</span>");
    gtk_container_add(GTK_CONTAINER(revenue_frame), ui->total_revenue_label);
    gtk_container_set_border_width(GTK_CONTAINER(revenue_frame), 10);
    
    ui->dashboard_window = window;
    
    ui_update_car_list(ui);
    ui_update_garage_availability(ui);
    ui_update_revenue(ui);
    
    gtk_widget_show_all(window);
}

void ui_init(UIState *ui, AppState *app_state, GtkApplication *app) {
    memset(ui, 0, sizeof(UIState));
    ui->app_state = app_state;
    ui->app = app;
    g_ui_state = ui;
}

