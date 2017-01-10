//
// Created by Youri Lefers on 19-12-16.
//

#ifndef PARTITIONSAFE_NOTIFICATIONCENTRE_H
#define PARTITIONSAFE_NOTIFICATIONCENTRE_H


#include "PartitionSafe.h"
#include "notification/BaseNotification.h"

class NotificationCentre {
private:
    // We do not want these
    NotificationCentre(NotificationCentre const&);
    void operator=(NotificationCentre const&);

protected:
    /**
     * The partition safe instance
     */
    PartitionSafe *partitionSafe;

    /**
     * Private constructor for singleton
     */
    NotificationCentre(PartitionSafe *partitionSafe);

public:
    /**
     * Retrieve the notification centre instance
     *
     * @param partitionSafe The partition safe instance to use
     * @return
     */
    static NotificationCentre& getInstance(PartitionSafe *partitionSafe = nullptr);

    /**
     * Save a new notification.
     *
     * @param notification
     * @return The notification ID
     */
    int saveNotification(BaseNotification *notification);

    /**
     * Load a simple notification.
     *
     * @param id
     * @param notification
     */
    void loadNotification(int id, BaseNotification **notification);

    /**
     * Delete a notification.
     *
     * @param id
     */
    void deleteNotification(BaseNotification *notification);

    /**
     * Load all notifications for a specific user.
     *
     * @param user_id
     * @return
     */
    std::vector<BaseNotification *> *loadNotificationsForUser(int user_id);

};


#endif //PARTITIONSAFE_NOTIFICATIONCENTRE_H