//
// Created by Youri Lefers on 11-01-17.
//

#include "DirectoryInvitationNotification.h"

DirectoryInvitationNotification::DirectoryInvitationNotification(const int id, const int user_from, const int user_to,
                                                                 const int type, const std::string &content)
        : BaseNotification(id, user_from, user_to, type, content) {
    // Split content
    this->inode = std::stoi(content.substr(0, content.find("::")));
    this->encryptionKey = content.substr(content.find("::") + 2);
}

std::string DirectoryInvitationNotification::toString() {
    return "User '" + std::to_string(user_from) + "' wants to share a directory (" + std::to_string(inode) + ") with you.";
}

int DirectoryInvitationNotification::handle(PartitionSafe *psInstance) {
    return BaseNotification::handle(psInstance);
}

DirectoryInvitationNotification *
DirectoryInvitationNotification::create(const int user_from, const int user_to, const int inode,
                                        const std::string &encryptionKey) {
    return new DirectoryInvitationNotification(0, user_from, user_to, 3, std::to_string(inode) + "::" + encryptionKey);
}
