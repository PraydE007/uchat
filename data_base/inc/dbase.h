#ifndef DBASE_H
#define DBASE_H

#include "../../inc/general.h"
#include <sqlite3.h>

typedef struct s_datab {
    int logtrigger;
    int passtrigger;
    int socket;
    char *id;
    char *second_id;
    char *commd;
    char *type;
    char *security_key;
    const char *login_db;
    const char *login_db2;
    const char *password_db;
    const char *security_db;
}               t_datab;

bool mx_is_active(json_object *jobj, sqlite3 *db, t_datab *datab);
char *mx_strjoin_free_js_value(char *s1, const char *s2);
const char *mx_json_to_str(json_object *jobj, char *key);
int mx_callback(void *datab, int argc, char **argv, char **azColName);
int mx_json_to_int(json_object *jobj, char *key);
json_object *mx_dbase_handler(json_object *jobj, sqlite3 *db);
json_object *mx_if_chat(json_object *jobj, sqlite3 *db, t_datab *datab);
json_object *mx_if_contact(json_object *jobj, sqlite3 *db, t_datab *datab);
json_object *mx_if_logging(json_object *jobj, sqlite3 *db, t_datab *datab);
json_object *mx_if_registr(json_object *jobj, sqlite3 *db, t_datab *datab);
t_datab *mx_create_datab_node(void);
void mx_data_base_creation(sqlite3 **db);
void mx_dealloc_datab(t_datab **datab);
void mx_js_add(json_object *j_result, char *key, char *value);
void mx_json_to_datab(json_object *jobj, t_datab *datab);
void mx_table_activity(sqlite3 *db, char *sql);
void mx_table_chats(sqlite3 *db, char *sql);
void mx_table_contacts(sqlite3 *db, char *sql);
void mx_table_creation(sqlite3 *db, char *sql, int (*callback)(void*, int, char**, char**));
void mx_table_messages(sqlite3 *db, char *sql);
void mx_table_setting(sqlite3 *db, char *sql, int (*callback)(void*, int, char**, char**), void *datab);
void mx_table_users(sqlite3 *db, char *sql);
void mx_table_users_chats(sqlite3 *db, char *sql);
void mx_trigger_for_activity(sqlite3 *db, char *sql);
void mx_trigger_for_messages(sqlite3 *db, char *sql);
void mx_trigger_for_users(sqlite3 *db, char *sql);
void mx_user_activate(sqlite3 *db, t_datab *datab, int socket);
void mx_user_deactivate(sqlite3 *db, int socket);


#endif
