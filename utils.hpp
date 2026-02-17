#ifndef UTILS_HPP
#define UTILS_HPP

#include <QString>

/**
 * @brief Utility class for common validation logic and helper functions.
 */
class Utils
{
public:
    /**
     * @brief Validates a password based on system rules (length, characters).
     * @param password The password to check.
     * @return An error message string if invalid, or an empty string if valid.
     */
    static QString validatePassword(const QString &password);

    /**
     * @brief Validates a username format.
     * @param username The username to check.
     * @return An error message string if invalid, or an empty string if valid.
     */
    static QString validateUsername(const QString &username);
};

#endif // UTILS_HPP