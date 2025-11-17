#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <ctime>
#include <string>

#ifdef _WIN32
#include <time.h>
#else
#include <time.h>
#endif

namespace Utils
{
// Thread-safe localtime wrapper
inline std::tm getLocalTime(std::time_t time)
{
    std::tm result;
#ifdef _WIN32
    localtime_s(&result, &time);
#else
    localtime_r(&time, &result);
#endif
    return result;
}

inline std::string toString(const QString &qstr) { return qstr.toStdString(); }

inline QString toQString(const std::string &str) { return QString::fromStdString(str); }

inline std::string safeToString(const QString &qstr) { return qstr.isEmpty() ? std::string() : qstr.toStdString(); }

inline QString formatPrice(double price) { return QString::number(price, 'f', 2) + " руб."; }

inline QString formatNumber(double number, int decimals = 2) { return QString::number(number, 'f', decimals); }

inline bool isNumericId(const QString &str)
{
    if (str.length() < 6 || str.length() > 8)
    {
        return false;
    }
    for (const QChar &ch : str)
    {
        if (!ch.isDigit())
        {
            return false;
        }
    }
    return true;
}
} // namespace Utils

#endif
