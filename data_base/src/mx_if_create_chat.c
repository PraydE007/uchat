#include "dbase.h"

static int cb_chat_checking(void *datab, int argc, char **argv, char **azColName) {
    (void)argc;
    (void)azColName;
    if (datab) {
        t_datab *new_datab = (t_datab *)datab;

        if (!mx_strcmp(new_datab->login_db, argv[1])) {
            new_datab->logtrigger = 1;
            return 1;
        }
    }
    return 0;
}

static int cb_chat_id_finder(void *datab, int argc, char **argv,
                                                            char **azColName) {
    (void)argc;
    (void)azColName;
    if (datab) {
        t_datab *new_datab = (t_datab *)datab;

        new_datab->chat_id = mx_strdup(argv[0]);
    }
    return 0;
}

static void insert_chat(json_object *jobj, json_object *j_reslt, sqlite3 *db,
                                                            t_datab *datab) {
    char sql[255];

    sprintf(sql, "insert into CHATS (CHAT_NAME, CHAT_STATUS)" \
            "values('%s', '%s')", mx_json_to_str(jobj, "Name"),
            mx_json_to_str(jobj, "Status"));
    mx_table_creation(db, sql, mx_callback);
    sprintf(sql, "select ID from CHATS where CHAT_NAME = '%s';",
            mx_json_to_str(jobj, "Name"));
    mx_table_setting(db, sql, cb_chat_id_finder, datab);
    sprintf(sql, "insert into USERS_CHATS (USER_id, CHAT_id)" \
            "values('%s', '%s')", datab->id, datab->chat_id);
    mx_table_creation(db, sql, mx_callback);
    mx_js_add(j_reslt, "Answer", MX_CHAT_MES);
}

json_object *mx_if_create_chat(json_object *jobj, sqlite3 *db, t_datab *datab) {
    json_object *j_result = json_object_new_object();
    char sql[30];

    if (mx_is_active(jobj, db, datab)) {
        datab->login_db2 = mx_json_to_str(jobj, "Chat");
        sprintf(sql, "select ID, CHAT_NAME from CHATS;");
        mx_table_setting(db, sql, cb_chat_checking, datab);
        if (datab->logtrigger == 1)
            mx_js_add(j_result, "Answer", MX_CHAT_ERR);
        else
            insert_chat(jobj, j_result, db, datab);
    }
    else
        mx_js_add(j_result, "Answer", MX_CHEAT_MESSAGE);
    printf("mx_if_create_chat(j_result): %s\n", json_object_to_json_string(j_result));//
    mx_strdel(&datab->id);// comment in mx_is_activ
    datab->logtrigger = 0;
    return j_result;
}
