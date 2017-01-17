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
    char *username;

    /**
     * The salt of the user's password.
     */
    char *salt;

    /**
     * The public key of the user.
     */
    char *publicKey;

    /**
     * The private key of the user.
     */
    char *privateKey;

    /**
     * User constructor.
     *
     * @param id The ID of the user
     * @param username The username of the user
     * @param salt The salt of the user
     * @param publicKey The public encryption key of the user
     * @param privateKey The private encryption key of the user
     */
    User(const unsigned id, const char *username, const char *salt, const char *publicKey, const char *privateKey);

    /**
     * Create a new user.
     *
     * @param username The username
     * @param password The password
     *
     * @return The created user
     */
    static User *create(const char *username, const char *password);

    /**
     * Salt a password.
     *
     * @param password The password of the user
     * @param salt The salt of the user
     * @param saltedPassword OUTPUT The salted password
     */
    static void saltedPassword(const char *password, const char *salt, char **saltedPassword);

    /**
     * Generate a new salt.
     *
     * @param salt OUTPUT The generated salt
     */
    static void generateSalt(char **salt);

};


#endif //PARTITIONSAFE_USER_H
