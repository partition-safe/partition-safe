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
    std::string path;

    std::string encryptionKey;

    DirectoryInvitationNotification(const int id, const int user_from, const int user_to, const int type,
                                    const std::string &content);

    static DirectoryInvitationNotification *create(const int user_from, const int user_to, const std::string path, const std::string &encryptionKey);

    static DirectoryInvitationNotification *create(const User *user_from, const User *user_to, const std::string path, const std::string &encryptionKey);

    std::string toString() override;

    int handle(PartitionSafe *psInstance) override;
};


#endif //PARTITIONSAFE_DIRECTORYINVITATIONNOTIFICATION_H
