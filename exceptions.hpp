#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include <string>
#include <QString>

namespace Acadence
{

    class Exception : public std::exception
    {
    protected:
        std::string msg;

    public:
        explicit Exception(const QString &message) : msg(message.toStdString()) {}
        const char *what() const noexcept override { return msg.c_str(); }
    };

    class FileException : public Exception
    {
    public:
        explicit FileException(const QString &message) : Exception(message) {}
    };

}

#endif // EXCEPTIONS_HPP