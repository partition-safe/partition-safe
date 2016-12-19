//
// Created by Youri Lefers on 19-12-16.
//

#include "NotificationCentre.h"

NotificationCentre::NotificationCentre(PartitionSafe *partitionSafe):
        partitionSafe(partitionSafe) {}

NotificationCentre *NotificationCentre::getInstance(PartitionSafe *partitionSafe) {
    if(!_instance) _instance = new NotificationCentre(partitionSafe);
    return _instance;
}

int NotificationCentre::saveNotification(BaseNotification *notification) {
    sqlite3_stmt *stmt;
    int index;

    // Setup the query
    const char *query = "INSERT INTO NOTIFICATIONS (USER_FROM, USER_TO, TYPE, DETAILS) VALUES (:user_from, :user_to, :type, :details)";

    // Retrieve the handle
    sqlite3 *handle = partitionSafe->getKeyStore()->getSqliteHandle();

    // Prepare the statement
    if(sqlite3_prepare_v2(handle, query, -1, &stmt, 0) != SQLITE_OK) throw "Could not prepare a statement";

    // Bind parameters
    if ((index = sqlite3_bind_parameter_index(stmt, ":user_from")) <= 0) throw "Could not retrieve parameter index in the statement";
    if(sqlite3_bind_int(stmt, index, notification->user_from) != SQLITE_OK) throw "Could not bind parameter";
    if ((index = sqlite3_bind_parameter_index(stmt, ":user_to")) <= 0) throw "Could not retrieve parameter index in the statement";
    if (sqlite3_bind_int(stmt, index, notification->user_to) != SQLITE_OK) throw "Could not bind parameter";
    if ((index = sqlite3_bind_parameter_index(stmt, ":type")) <= 0) throw "Could not retrieve parameter index in the statement";
    if (sqlite3_bind_int(stmt, index, notification->type) != SQLITE_OK) throw "Could not bind parameter";
    if ((index = sqlite3_bind_parameter_index(stmt, ":details")) <= 0) throw "Could not retrieve parameter index in the statement";
    if (sqlite3_bind_text(stmt, index, notification->content.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) throw "Could not bind parameter";

    // Execute query
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_OK && rc != SQLITE_DONE) throw "Could not execute query";

    // Retrieve the last inserted row ID
    return static_cast<int>(sqlite3_last_insert_rowid(handle));
}

void NotificationCentre::loadNotification(int id, BaseNotification **notification) {
    // Prepare the query
    sqlite3_stmt *stmt;
    int index;

    // Retrieve the handle
    sqlite3 *handle = partitionSafe->getKeyStore()->getSqliteHandle();

    // Prepare the statement
    if(sqlite3_prepare_v2(handle, "SELECT ID, USER_FROM, USER_TO, TYPE, DETAILS FROM NOTIFICATIONS WHERE ID = :id", -1, &stmt, 0) != SQLITE_OK) throw "Could not prepare a statement";

    // Bind parameters
    if ((index = sqlite3_bind_parameter_index(stmt, ":id")) <= 0) throw "Could not retrieve parameter index in the statement";
    if(sqlite3_bind_int(stmt, index, id) != SQLITE_OK) throw "Could not bind parameter";

    // Execute query
    int res = sqlite3_step(stmt);
    if (res != SQLITE_ROW) throw "Could not execute query";

    // Get the result of the current row
    const int _id = sqlite3_column_int(stmt, 0);
    const int _user_from = sqlite3_column_int(stmt, 1);
    const int _user_to = sqlite3_column_int(stmt, 2);
    const int _type = sqlite3_column_int(stmt, 3);
    const char *_details = (const char *) sqlite3_column_text(stmt, 4);

    // Create the user
    *notification = new BaseNotification(_id, _user_from, _user_to, _type, std::string(_details));
}
