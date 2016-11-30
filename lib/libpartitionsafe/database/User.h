//
// Created by Youri Lefers on 29-11-16.
//

#ifndef PARTITIONSAFE_USER_H
#define PARTITIONSAFE_USER_H


class User {
public:
    /**
     * The ID of the user.
     */
    const unsigned id;

    /**
     * The username of the user.
     */
    const char *username;

    /**
     * The salt of the user's password.
     */
    const char *salt;

    /**
     * The public key of the user.
     */
    const char *publicKey;

    /**
     * The private key of the user.
     */
    const char *privateKey;

    /**
     * User constructor.
     *
     * @param id
     * @param username
     * @param salt
     * @param publicKey
     * @param privateKey
     */
    User(const unsigned id, const char *username, const char *salt, const char *publicKey, const char *privateKey);

    /**
     * Create a new user.
     *
     * @param username
     * @param password
     * @return
     */
    static User *create(const char *username, const char *password);

};


#endif //PARTITIONSAFE_USER_H
