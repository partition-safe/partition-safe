//
// Created by Youri Lefers on 19-12-16.
//

#ifndef PARTITIONSAFE_NOTIFICATIONCENTRE_H
#define PARTITIONSAFE_NOTIFICATIONCENTRE_H


#include "PartitionSafe.h"
#include "notification/BaseNotification.h"

class NotificationCentre {
private:
    // We do not want these in singletons
    NotificationCentre(NotificationCentre const &);
    void operator=(NotificationCentre const &);

protected:
    /**
     * The partition safe instance
     */
    PartitionSafe *partitionSafe;

    /**
     * Private constructor for singleton
     *
     * @param partitionSafe The PartitionSafe instance to use
     */
    NotificationCentre(PartitionSafe *partitionSafe);

public:
    /**
     * Retrieve the notification centre instance
     *
     * @param partitionSafe The partition safe instance to use (may be empty)
     *
     * @return The notification centre instance
     */
    static NotificationCentre &getInstance(PartitionSafe *partitionSafe = nullptr);

    /**
     * Save a new notification.
     *
     * @param notification The notification to save
     *
     * @return The created notification ID
     */
    int saveNotification(BaseNotification *notification);

    /**
     * Load a simple notification.
     *
     * @param id The ID of the notification
     * @param notification OUTPUT The retrieved notification
     */
    void loadNotification(int id, BaseNotification **notification);

    /**
     * Delete a notification.
     *
     * @param notification The notification to delete
     */
    void deleteNotification(BaseNotification *notification);

    /**
     * Load all notifications for a specific user.
     *
     * @param user_id The user ID to retrieve notification for
     *
     * @return The vector of notifications
     */
    std::vector<BaseNotification *> *loadNotificationsForUser(int user_id);

};


#endif //PARTITIONSAFE_NOTIFICATIONCENTRE_H
