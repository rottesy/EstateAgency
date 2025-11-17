#ifndef TABLE_HELPER_H
#define TABLE_HELPER_H

#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>

namespace TableHelper
{
inline int getSelectedRow(QTableWidget *table)
{
    if (!table)
        return -1;
    return table->currentRow();
}

inline bool isValidRow(QTableWidget *table, int row)
{
    if (!table)
        return false;
    return row >= 0 && row < table->rowCount();
}

inline QString getSelectedId(QTableWidget *table, int column = 0)
{
    if (!table)
        return QString();

    int row = getSelectedRow(table);
    if (!isValidRow(table, row))
    {
        return QString();
    }

    QTableWidgetItem *item = table->item(row, column);
    return item ? item->text() : QString();
}

inline void clearTable(QTableWidget *table)
{
    if (table)
    {
        table->setRowCount(0);
    }
}

inline QString getTransactionStatusText(const std::string &status)
{
    if (status == "pending")
        return "В ожидании";
    if (status == "completed")
        return "Завершена";
    return "Отменена";
}

inline QString getAuctionStatusText(const std::string &status)
{
    if (status == "active")
        return "Активен";
    if (status == "completed")
        return "Завершен";
    return "Отменен";
}

inline QString getPropertyTypeText(const std::string &type)
{
    if (type == "Apartment")
        return "Квартира";
    if (type == "House")
        return "Дом";
    if (type == "Commercial")
        return "Коммерческая недвижимость";
    return QString::fromStdString(type);
}

inline bool hasValidSelection(QTableWidget *table) { return isValidRow(table, getSelectedRow(table)); }
} // namespace TableHelper

#endif // TABLE_HELPER_H
