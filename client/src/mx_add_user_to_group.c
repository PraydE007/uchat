#include "client.h"

void mx_add_user_to_group(GtkButton *btn, gpointer data) {
    t_s_glade *gui = (t_s_glade *)data;
    char *send_data = NULL;
    const char *user_name = gtk_entry_get_text(GTK_ENTRY(gui->e_g_nickname));
    char *login = (char *)gtk_entry_get_text(GTK_ENTRY(gui->e_l_login));
    // if (!mx_strcmp(user_name, "")) {
    //     GdkRGBA *color = NULL;
    //     gdk_rgba_parse (color, "red");
    //     gtk_widget_override_background_color(gui->e_g_nickname, GTK_STATE_FLAG_NORMAL, color);
    // }
    if (mx_strcmp(user_name, "")) {
        json_object *jobj = json_object_new_object();
        json_object *j_type = json_object_new_string("Add_user_to_chat");
        json_object *j_login = json_object_new_string(login);
        json_object *j_user_name = json_object_new_string(user_name);
        json_object *j_chat_name = json_object_new_string(gui->send_to);
        json_object *j_key = json_object_new_string(gui->key);
        json_object_object_add(jobj,"Type", j_type);
        json_object_object_add(jobj,"Login", j_login);
        json_object_object_add(jobj,"Contact_login", j_user_name);
        json_object_object_add(jobj,"Chat_name", j_chat_name);
        json_object_object_add(jobj,"Security_key", j_key);
        send_data = (char *)json_object_to_json_string(jobj);
        printf("SEND_DATA :%s\n", send_data);
    }
    (void)btn;
}
