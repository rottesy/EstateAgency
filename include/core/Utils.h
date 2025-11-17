#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <ranges>
#include <sstream>
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

// Format current time using std::chrono
inline std::string getCurrentTimeString(const char *format = "%Y-%m-%d %H:%M:%S")
{
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = getLocalTime(time);
    std::ostringstream oss;
    oss << std::put_time(&tm, format);
    return oss.str();
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
    return std::ranges::all_of(str, [](const QChar &ch) { return ch.isDigit(); });
}

// Helper function to check if string contains substring (C++20 compatible)
inline bool stringContains(const std::string &str, const std::string &substr)
{
    return str.find(substr) != std::string::npos;
}
} // namespace Utils

#endif
