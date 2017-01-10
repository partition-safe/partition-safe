//
// Created by Youri Lefers on 19-12-16.
//

#include "BaseNotification.h"
#include "../NotificationCentre.h"

BaseNotification::BaseNotification(const int id, const int user_from, const int user_to, const int type,
                                   const std::string content): id(id), user_from(user_from), user_to(user_to), type(type),
                                                               content(content) {}

std::string BaseNotification::toString() {
    return "New notification from '" + std::to_string(user_from) + "' of type '" + std::to_string(type) + "' containing '" + content + "'.";
}

int BaseNotification::handle(PartitionSafe *psInstance) {
    // Remove this notification
    NotificationCentre::getInstance().deleteNotification(this);

    // DONE
    return 0;
}
