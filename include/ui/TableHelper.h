#ifndef TABLE_HELPER_H
#define TABLE_HELPER_H

#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QWidget>
#include <string_view>
#include <utility>      // ← нужно для std::forward

namespace TableHelper
{
inline int getSelectedRow(const QTableWidget *table)
{
    if (!table)
        return -1;
    return table->currentRow();
}

inline bool isValidRow(const QTableWidget *table, int row)
{
    if (!table)
        return false;
    return row >= 0 && row < table->rowCount();
}

inline QString getSelectedId(const QTableWidget *table, int column = 0)
{
    if (!table)
        return QString();

    int row = getSelectedRow(table);
    if (!isValidRow(table, row))
    {
        return QString();
    }

    const QTableWidgetItem *item = table->item(row, column);
    return item ? item->text() : QString();
}

inline void clearTable(QTableWidget *table)
{
    if (table)
    {
        table->setRowCount(0);
    }
}

inline QString getTransactionStatusText(std::string_view status)
{
    if (status == "pending")
        return "В ожидании";
    if (status == "completed")
        return "Завершена";
    return "Отменена";
}

inline QString getAuctionStatusText(std::string_view status)
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

inline bool hasValidSelection(const QTableWidget *table)
{
    return isValidRow(table, getSelectedRow(table));
}

inline void selectRowById(QTableWidget *table, const QString &id, int column = 0)
{
    if (!table)
        return;
    for (int i = 0; i < table->rowCount(); ++i)
    {
        if (table->item(i, column) && table->item(i, column)->text() == id)
        {
            table->selectRow(i);
            break;
        }
    }
}

inline bool checkTableSelection(const QTableWidget *table, const QString &errorMessage, QWidget *parent = nullptr)
{
    if (!table || !hasValidSelection(table))
    {
        if (!errorMessage.isEmpty() && parent)
        {
            QMessageBox::information(parent, QString("Информация"), errorMessage);
        }
        return false;
    }
    return true;
}

template <typename EditFunc, typename DeleteFunc>
inline QWidget *createActionButtons(
    QTableWidget *table,
    const QString &id,
    QWidget *parent,
    EditFunc &&editAction,
    DeleteFunc &&deleteAction,
    const QString &editText = "Редактировать",
    int editWidth = 110)
{
    auto *actionsWidget = new QWidget;
    auto *actionsLayout = new QHBoxLayout(actionsWidget);
    actionsLayout->setContentsMargins(5, 5, 5, 5);
    actionsLayout->setSpacing(8);

    auto *editBtn = new QPushButton(editText);
    editBtn->setMinimumWidth(editWidth);
    editBtn->setFixedHeight(35);

    auto *deleteBtn = new QPushButton("Удалить");
    deleteBtn->setMinimumWidth(90);
    deleteBtn->setFixedHeight(35);

    // Исправление: убран неверный "parent" как получатель сигнала
    QObject::connect(editBtn, &QPushButton::clicked,
                     [table, id, func = std::forward<EditFunc>(editAction)]()
                     {
                         selectRowById(table, id);
                         func();
                     });

    QObject::connect(deleteBtn, &QPushButton::clicked,
                     [table, id, func = std::forward<DeleteFunc>(deleteAction)]()
                     {
                         selectRowById(table, id);
                         func();
                     });

    actionsLayout->addWidget(editBtn);
    actionsLayout->addWidget(deleteBtn);
    actionsLayout->addStretch();

    return actionsWidget;
}

} // namespace TableHelper

#endif // TABLE_HELPER_H
