//
// Created by marc on 15-11-16.
//

#include "KeyStore.h"

KeyStore::KeyStore(const char *path, FILE *fh):
    path(path), fd(fh) {}
