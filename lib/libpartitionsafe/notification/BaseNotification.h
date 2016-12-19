//
// Created by Youri Lefers on 19-12-16.
//

#ifndef PARTITIONSAFE_BASENOTIFICATION_H
#define PARTITIONSAFE_BASENOTIFICATION_H


#include <string>

class BaseNotification {
public:
    /**
     * The ID of the notification
     */
    const int id;

    /**
     * The user whom send the notification
     */
    const int user_from;

    /**
     * The user whom receives the notification
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
     * @param id
     * @param user_from
     * @param user_to
     * @param type
     * @param content
     */
    BaseNotification(const int id, const int user_from, const int user_to, const int type, const std::string content);

};


#endif //PARTITIONSAFE_BASENOTIFICATION_H
