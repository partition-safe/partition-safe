//
// Created by Youri Lefers on 19-12-16.
//

#ifndef PARTITIONSAFE_BASENOTIFICATION_H
#define PARTITIONSAFE_BASENOTIFICATION_H


#include <string>

class PartitionSafe;

class BaseNotification {
public:
    /**
     * The ID of the notification
     */
    const int id;

    /**
     * The user who send the notification
     */
    const int user_from;

    /**
     * The user who receives the notification
     */
    const int user_to;

    /**
     * The notification type
     */
    const int type;

    /**
     * The plain text string content
     */
    const std::string content;

    /**
     * Base notification constructor.
     *
     * @param id The ID of the notification
     * @param user_from The user who send the notification
     * @param user_to The user who received the notification
     * @param type The type of the notification
     * @param content The content of the notification
     */
    BaseNotification(const int id, const int user_from, const int user_to, const int type, const std::string content);

    /**
     * String representation of the base notification.
     *
     * @return The string representation
     */
    virtual std::string toString();

    /**
     * Handle the notification.
     *
     * The notification knows best what to do.
     *
     * @param psInstance The partition safe instance
     *
     * @return 0 when ok, > 0 when not ok
     */
    virtual int handle(PartitionSafe *psInstance);

};


#endif //PARTITIONSAFE_BASENOTIFICATION_H
