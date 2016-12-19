//
// Created by Youri Lefers on 19-12-16.
//

#include "BaseNotification.h"

BaseNotification::BaseNotification(const int id, const int user_from, const int user_to, const int type,
                                   const std::string content): id(id), user_from(user_from), user_to(user_to), type(type),
                                                               content(content) {}
