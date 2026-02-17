#include "utils.hpp"

QString Utils::validatePassword(const QString &password)
{
    if (password.length() < 4 || password.length() > 12)
    {
        return "Password must be between 4 and 12 characters.";
    }
    for (QChar c : password)
    {
        if (c.toLatin1() < 32 || c.toLatin1() > 126)
        {
            return "Password contains non-printable ASCII characters.";
        }
    }
    return "";
}

QString Utils::validateUsername(const QString &username)
{
    if (username.trimmed().isEmpty())
    {
        return "Username cannot be empty.";
    }
    return "";
}