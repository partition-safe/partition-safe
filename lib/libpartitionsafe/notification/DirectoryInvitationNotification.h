//
// Created by Youri Lefers on 11-01-17.
//

#ifndef PARTITIONSAFE_DIRECTORYINVITATIONNOTIFICATION_H
#define PARTITIONSAFE_DIRECTORYINVITATIONNOTIFICATION_H


#include "BaseNotification.h"

/**
 * The content of a directory invitation should be formatted as follows:
 *
 * <inode>::<encryption key>
 */
class DirectoryInvitationNotification : public BaseNotification {
public:
    int inode;

    std::string encryptionKey;

    DirectoryInvitationNotification(const int id, const int user_from, const int user_to, const int type,
                                    const std::string &content);

    static DirectoryInvitationNotification *create(const int user_from, const int user_to, const int inode, const std::string &encryptionKey);

    std::string toString() override;

    int handle(PartitionSafe *psInstance) override;
};


#endif //PARTITIONSAFE_DIRECTORYINVITATIONNOTIFICATION_H
