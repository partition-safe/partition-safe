//
// Created by Youri Lefers on 19-12-16.
//

#include "NotificationCentre.h"

NotificationCentre::NotificationCentre(PartitionSafe *partitionSafe):
        partitionSafe(partitionSafe) {}

NotificationCentre& NotificationCentre::getInstance(PartitionSafe *partitionSafe) {
    static NotificationCentre instance(partitionSafe);
    return instance;
}

int NotificationCentre::saveNotification(BaseNotification *notification) {
    // Save the thing
    return partitionSafe->getKeyStore()->saveNotification(notification);
}

void NotificationCentre::loadNotification(int id, BaseNotification **notification) {
    // Save the thing
    partitionSafe->getKeyStore()->loadNotification(id, notification);
}

void NotificationCentre::deleteNotification(BaseNotification *notification) {
    // Save the thing
    partitionSafe->getKeyStore()->deleteNotification(notification);
}

std::vector<BaseNotification *> *NotificationCentre::loadNotificationsForUser(int user_id) {
    // Save the thing
    return partitionSafe->getKeyStore()->loadNotificationsForUser(user_id);
}
