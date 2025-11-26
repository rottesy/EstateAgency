#include "../../include/ui/TableHelper.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QWidget>

namespace TableHelper
{

QWidget *createActionButtons(QTableWidget *table, const QString &id, const QWidget * /*parent*/,
                             const std::function<void()> &editAction, const std::function<void()> &deleteAction,
                             const QString &editText, int editWidth)
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

    QObject::connect(editBtn, &QPushButton::clicked,
                     [table, id, editAction]()
                     {
                         selectRowById(table, id);
                         editAction();
                     });

    QObject::connect(deleteBtn, &QPushButton::clicked,
                     [table, id, deleteAction]()
                     {
                         selectRowById(table, id);
                         deleteAction();
                     });

    actionsLayout->addWidget(editBtn);
    actionsLayout->addWidget(deleteBtn);
    actionsLayout->addStretch();

    return actionsWidget;
}

} 
