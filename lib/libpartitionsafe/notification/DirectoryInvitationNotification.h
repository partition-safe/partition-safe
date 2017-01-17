//
// Created by Youri Lefers on 11-01-17.
//

#ifndef PARTITIONSAFE_DIRECTORYINVITATIONNOTIFICATION_H
#define PARTITIONSAFE_DIRECTORYINVITATIONNOTIFICATION_H


#include "BaseNotification.h"
#include "../database/User.h"

/**
 * The content of a directory invitation should be formatted as follows:
 *
 * <inode>::<encryption key>
 */
class DirectoryInvitationNotification : public BaseNotification {
public:
    /**
     * The path of the directory the user was invited for
     */
    std::string path;

    /**
     * The encrypted encryption key
     */
    std::string encryptionKey;

    /**
     * Directory invitation notification constructor.
     *
     * @param id The ID of the notification
     * @param user_from The user who send the notification
     * @param user_to The user who received the notification
     * @param type The type of the notification
     * @param content The content of the notification
     */
    DirectoryInvitationNotification(const int id, const int user_from, const int user_to, const int type,
                                    const std::string &content);

    /**
     * Create a directory invitation notification.
     *
     * @param user_from The user ID who invited an user
     * @param user_to The user ID to invite
     * @param path The path of the directory to share
     * @param encryptionKey The encrypted encryption key
     *
     * @return The created notification
     */
    static DirectoryInvitationNotification *create(const int user_from, const int user_to, const std::string path,
                                                   const std::string &encryptionKey);

    /**
     * Create a directory invitation notification.
     *
     * @param user_from The user who invited an user
     * @param user_to The user to invite
     * @param path The path of the directory to share
     * @param encryptionKey The encrypted encryption key
     *
     * @return The created notification
     */
    static DirectoryInvitationNotification *create(const User *user_from, const User *user_to, const std::string path,
                                                   const std::string &encryptionKey);

    /**
     * Override the toString to show proper information.
     *
     * @return The string representation
     */
    std::string toString() override;

    /**
     * Handle that will add the directory to the key store.
     *
     * @param psInstance The PartitionSafe instance
     *
     * @return 0 on OK, > 0 when NOT OK
     */
    int handle(PartitionSafe *psInstance) override;
};


#endif //PARTITIONSAFE_DIRECTORYINVITATIONNOTIFICATION_H
