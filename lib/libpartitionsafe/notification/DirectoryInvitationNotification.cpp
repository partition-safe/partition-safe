//
// Created by Youri Lefers on 11-01-17.
//

#include "DirectoryInvitationNotification.h"
#include "../database/Key.h"

class PartitionSafe;

DirectoryInvitationNotification::DirectoryInvitationNotification(const int id, const int user_from, const int user_to,
                                                                 const int type, const std::string &content)
        : BaseNotification(id, user_from, user_to, type, content) {
    // Split content
    this->path = content.substr(0, content.find("::"));
    this->encryptionKey = content.substr(content.find("::") + 2);
}

std::string DirectoryInvitationNotification::toString() {
    return "User '" + std::to_string(user_from) + "' wants to share a directory (" + path + ") with you.";
}

int DirectoryInvitationNotification::handle(PartitionSafe *psInstance) {
    // Remove notification
    return BaseNotification::handle(psInstance);
}

DirectoryInvitationNotification *
DirectoryInvitationNotification::create(const int user_from, const int user_to, const std::string path,
                                        const std::string &encryptionKey) {
    return new DirectoryInvitationNotification(0, user_from, user_to, 3, path + "::" + encryptionKey);
}

DirectoryInvitationNotification *
DirectoryInvitationNotification::create(const User *user_from, const User *user_to, const std::string path,
                                        const std::string &encryptionKey) {
    return new DirectoryInvitationNotification(0, user_from->id, user_to->id, 3, path + "::" + encryptionKey);
}
