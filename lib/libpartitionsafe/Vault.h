//
// Created by marc on 15-11-16.
//

#ifndef PARTITIONSAFE_VAULT_H
#define PARTITIONSAFE_VAULT_H


#include "Partition.h"
#include "KeyStore.h"

class Vault {

    /**
     * Our file storage
     */
    Partition partition;

    /**
     * Our key storage
     */
    KeyStore keyStore;




};


#endif //PARTITIONSAFE_VAULT_H
