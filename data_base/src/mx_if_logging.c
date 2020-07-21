#include "dbase.h"

static int cb_loganswer(void *datab, int argc, char **argv, char **azColName) {
    t_datab *new_datab = (t_datab *)datab;

    (void)argc;
    (void)azColName;
    if (!mx_strcmp(new_datab->login_db, argv[1])) {
        new_datab->logtrigger = 1;
        if (!mx_strcmp(new_datab->password_db, argv[2])) {
            new_datab->passtrigger = 1;
            new_datab->id = mx_strdup(argv[0]);
        }
        return 1;
    }
    return 0;
}

// static int cb_chs_cnts(void *datab, int argc, char **argv,char **colName) {
//     char **new_str = (char **)datab;

//     for(int i = 0; i < argc; i++) {
//         printf("%s = %s\n", colName[i], argv[i] ? argv[i] : "NULL");//
//         *new_str = mx_strjoin_free_js_value(*new_str, argv[i]);
//         printf("new_str = %s\n", *new_str);//
//     }
//     return 0;
// }

// static void call_to_db(json_object *j_result, sqlite3 *db, t_datab *datab,
//                                                                 char *sql) {
//     int connection_point;

//     connection_point = sqlite3_exec(db, sql, cb_chs_cnts, &datab->commd, NULL);
//     if (connection_point != SQLITE_OK)
//         fprintf(stderr, "error: %s\n", sqlite3_errmsg(db));
//     if (datab->commd)
//         mx_add_str_to_js(j_result, datab->type, datab->commd);
//     mx_strdel(&datab->commd);
//     mx_strdel(&datab->type);
// }

// static void js_chts_conts(json_object *j_result, sqlite3 *db, t_datab *datab,
//                                                                    char *sql) {

//     mx_add_str_to_js(j_result, "Answer", MX_LOG_MES);
//     mx_add_str_to_js(j_result, "Security_key", datab->security_key);
//     sprintf(sql, "select CHAT_NAME from CHATS INNER JOIN USERS_CHATS " \
//             "ON ID = CHAT_id WHERE USER_id = %s;", datab->id);
//     datab->type = mx_strdup("Chats");
//     call_to_db(j_result, db, datab, sql);
//     sprintf(sql, "select LOGIN from USERS INNER JOIN CONTACTS " \
//             "ON ID = FOLLOWER_id WHERE OWNER_id = %s;", datab->id);
//     datab->type = mx_strdup("Contacts");
//     call_to_db(j_result, db, datab, sql);
// }

json_object *mx_if_logging(json_object *jobj, sqlite3 *db, t_datab *datab) {
    json_object *j_result = json_object_new_object();
    char sql[255];

    mx_js_to_datab(jobj, datab);
    sprintf(sql, "select ID, LOGIN, PASSWORD from USERS;");
    mx_table_setting(db, sql, cb_loganswer, datab);
    if (datab->logtrigger == 1 && datab->passtrigger == 1) {
        mx_user_activate(db, datab, datab->socket);
        mx_add_str_to_js(j_result, "Answer", MX_LOG_MES);
        mx_add_str_to_js(j_result, "Security_key", datab->security_key);
        mx_js_chts_conts_for_sender(j_result, db, datab, sql);
    }
    else
        mx_add_str_to_js(j_result, "Answer", MX_LOG_ERR);
    datab->logtrigger = 0;
    datab->passtrigger = 0;
    mx_strdel(&datab->id);
    mx_strdel(&datab->security_key);
    return j_result;
}
