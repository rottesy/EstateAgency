#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>

#ifdef _WIN32
#include <time.h>
#else
#include <time.h>
#endif

namespace Utils
{

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


inline bool stringContains(std::string_view str, std::string_view substr) { return str.contains(substr); }
} 

#endif
