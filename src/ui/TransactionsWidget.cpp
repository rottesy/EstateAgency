#include "../../include/ui/TransactionsWidget.h"
#include "../../include/core/Constants.h"
#include "../../include/core/Utils.h"
#include "../../include/entities/Client.h"
#include "../../include/entities/Property.h"
#include "../../include/entities/Transaction.h"
#include "../../include/ui/TableHelper.h"
#include "../../include/ui/TransactionDialog.h"
#include <QAbstractItemView>
#include <QHeaderView>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <functional>

TransactionsWidget::TransactionsWidget(EstateAgency *agency, QWidget *parent) : QWidget(parent), agency(agency)
{
    setupUI();
}

void TransactionsWidget::setupUI()
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    auto *headerLayout = new QHBoxLayout;
    auto *title = new QLabel("Сделки");
    title->setStyleSheet("color: #ffffff; font-size: 22pt; font-weight: bold;");
    headerLayout->addWidget(title);
    headerLayout->addStretch();

    searchTransactionEdit = new QLineEdit;
    searchTransactionEdit->setPlaceholderText("🔍 Поиск по ID");
    searchTransactionEdit->setFixedWidth(250);
    searchTransactionBtn = new QPushButton("Найти");
    headerLayout->addWidget(searchTransactionEdit);
    headerLayout->addWidget(searchTransactionBtn);

    addTransactionBtn = new QPushButton("➕ Добавить");
    refreshTransactionBtn = new QPushButton("🔄 Обновить");
    headerLayout->addWidget(addTransactionBtn);
    headerLayout->addWidget(refreshTransactionBtn);
    layout->addLayout(headerLayout);

    auto *splitter = new QSplitter(Qt::Horizontal);

    transactionsTable = new QTableWidget;
    transactionsTable->setColumnCount(7);
    transactionsTable->setHorizontalHeaderLabels(
        {"ID", "Недвижимость", "Клиент", "Цена", "Дата", "Статус", "Действия"});
    transactionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    transactionsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    transactionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    transactionsTable->setAlternatingRowColors(true);
    transactionsTable->verticalHeader()->setDefaultSectionSize(60);
    transactionsTable->setColumnWidth(0, 100);
    transactionsTable->setColumnWidth(1, 250);
    transactionsTable->setColumnWidth(2, 200);
    transactionsTable->setColumnWidth(3, 150);
    transactionsTable->setColumnWidth(4, 120);
    transactionsTable->setColumnWidth(5, 120);
    transactionsTable->setColumnWidth(6, 300);
    transactionsTable->horizontalHeader()->setStretchLastSection(false);

    auto *detailsFrame = new QFrame;
    detailsFrame->setFixedWidth(400);
    detailsFrame->setStyleSheet("background-color: #252525; border-radius: 8px; padding: 15px;");
    auto *detailsLayout = new QVBoxLayout(detailsFrame);
    auto *detailsTitle = new QLabel("Детали сделки");
    detailsTitle->setStyleSheet("color: #ffffff; font-size: 14pt; font-weight: "
                                "bold; padding-bottom: 10px;");
    detailsLayout->addWidget(detailsTitle);
    transactionDetailsText = new QTextEdit;
    transactionDetailsText->setReadOnly(true);
    transactionDetailsText->setStyleSheet("background-color: #2d2d2d; color: #d0d0d0; border: none;");
    detailsLayout->addWidget(transactionDetailsText);

    splitter->addWidget(transactionsTable);
    splitter->addWidget(detailsFrame);
    splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 1);

    layout->addWidget(splitter);

    connect(addTransactionBtn, &QPushButton::clicked, this, &TransactionsWidget::addTransaction);
    connect(refreshTransactionBtn, &QPushButton::clicked, this, &TransactionsWidget::refresh);
    connect(searchTransactionBtn, &QPushButton::clicked, this, &TransactionsWidget::searchTransactions);
    connect(transactionsTable, &QTableWidget::itemSelectionChanged, this,
            &TransactionsWidget::transactionSelectionChanged);
}

void TransactionsWidget::refresh() { updateTable(); }

void TransactionsWidget::updateTable()
{
    if (!transactionsTable)
        return;

    TableHelper::clearTable(transactionsTable);
    auto transactions = agency->getTransactionManager().getAllTransactions();

    for (Transaction *trans : transactions)
    {
        if (!trans)
            continue;

        int row = transactionsTable->rowCount();
        transactionsTable->insertRow(row);

        Property *prop = agency->getPropertyManager().findProperty(trans->getPropertyId());
        Client *client = agency->getClientManager().findClient(trans->getClientId());

        transactionsTable->setItem(row, 0, new QTableWidgetItem(Utils::toQString(trans->getId())));
        transactionsTable->setItem(row, 1,
                                   new QTableWidgetItem(prop ? Utils::toQString(prop->getAddress()) : "Не указана"));
        transactionsTable->setItem(row, 2,
                                   new QTableWidgetItem(client ? Utils::toQString(client->getName()) : "Не указан"));
        transactionsTable->setItem(row, 3, new QTableWidgetItem(Utils::formatNumber(trans->getFinalPrice())));
        transactionsTable->setItem(row, 4, new QTableWidgetItem(Utils::toQString(trans->getDate())));
        transactionsTable->setItem(row, 5,
                                   new QTableWidgetItem(TableHelper::getTransactionStatusText(trans->getStatus())));

        QString transId = Utils::toQString(trans->getId());
        QWidget *actionsWidget = createActionButtons(
            transactionsTable, transId, [this]() { editTransaction(); }, [this]() { deleteTransaction(); });
        transactionsTable->setCellWidget(row, 6, actionsWidget);
    }
}

void TransactionsWidget::addTransaction()
{
    QStringList propertyIds, clientIds;
    auto properties = agency->getPropertyManager().getAllProperties();
    auto clients = agency->getClientManager().getAllClients();

    if (properties.empty() || clients.empty())
    {
        QMessageBox::information(this, "Информация", "Недостаточно данных для создания сделки");
        return;
    }

    for (Property *p : properties)
    {
        if (p)
            propertyIds << QString::fromStdString(p->getId() + " - " + p->getAddress());
    }
    for (Client *c : clients)
    {
        if (c)
            clientIds << QString::fromStdString(c->getId() + " - " + c->getName());
    }

    TransactionDialog dialog(this, nullptr, propertyIds, clientIds);
    if (dialog.exec() == QDialog::Accepted)
    {
        try
        {
            std::string propertyId = dialog.getPropertyId().toStdString();
            std::string clientId = dialog.getClientId().toStdString();
            std::string status = dialog.getStatus().toStdString();

            if (!validateTransaction(propertyId, clientId, status))
                return;

            auto trans = std::make_shared<Transaction>(dialog.getId().toStdString(), propertyId, clientId,
                                                       dialog.getFinalPrice(), status, dialog.getNotes().toStdString());
            agency->getTransactionManager().addTransaction(trans);

            Property *prop = agency->getPropertyManager().findProperty(propertyId);
            if (prop &&
                (status == Constants::TransactionStatus::PENDING || status == Constants::TransactionStatus::COMPLETED))
                prop->setAvailable(false);

            refresh();
            emit dataChanged();
        }
        catch (const std::exception &e)
        {
            QMessageBox::warning(this, "Ошибка", QString("Ошибка добавления: %1").arg(e.what()));
        }
    }
}

void TransactionsWidget::editTransaction()
{
    if (!checkTableSelection(transactionsTable, "Выберите сделку для редактирования"))
        return;

    QString id = getSelectedIdFromTable(transactionsTable);
    Transaction *trans = agency->getTransactionManager().findTransaction(id.toStdString());
    if (!trans)
        return;

    QStringList propertyIds, clientIds;
    auto properties = agency->getPropertyManager().getAllProperties();
    auto clients = agency->getClientManager().getAllClients();

    for (Property *p : properties)
    {
        if (p)
            propertyIds << QString::fromStdString(p->getId() + " - " + p->getAddress());
    }
    for (Client *c : clients)
    {
        if (c)
            clientIds << QString::fromStdString(c->getId() + " - " + c->getName());
    }

    TransactionDialog dialog(this, trans, propertyIds, clientIds);
    if (dialog.exec() == QDialog::Accepted)
    {
        try
        {
            std::string propertyId = dialog.getPropertyId().toStdString();
            std::string clientId = dialog.getClientId().toStdString();
            std::string status = dialog.getStatus().toStdString();

            if (!validateTransaction(propertyId, clientId, status, id.toStdString()))
                return;

            std::string oldPropertyId = trans->getPropertyId();
            Property *oldProp = agency->getPropertyManager().findProperty(oldPropertyId);

            agency->getTransactionManager().removeTransaction(id.toStdString());
            auto newTrans =
                std::make_shared<Transaction>(dialog.getId().toStdString(), propertyId, clientId,
                                              dialog.getFinalPrice(), status, dialog.getNotes().toStdString());
            agency->getTransactionManager().addTransaction(newTrans);

            Property *prop = agency->getPropertyManager().findProperty(propertyId);
            if (prop)
                prop->setAvailable(status == "cancelled");

            if (oldProp && oldPropertyId != propertyId && !hasActiveTransactions(oldPropertyId))
                oldProp->setAvailable(true);

            refresh();
            emit dataChanged();
        }
        catch (const std::exception &e)
        {
            QMessageBox::warning(this, "Ошибка", QString("Ошибка редактирования: %1").arg(e.what()));
        }
    }
}

void TransactionsWidget::deleteTransaction()
{
    if (!checkTableSelection(transactionsTable, "Выберите сделку для удаления"))
        return;

    QString id = getSelectedIdFromTable(transactionsTable);
    int ret = QMessageBox::question(this, "Подтверждение", "Удалить сделку?", QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes)
    {
        Transaction *trans = agency->getTransactionManager().findTransaction(id.toStdString());
        std::string propertyId = trans ? trans->getPropertyId() : "";

        agency->getTransactionManager().removeTransaction(id.toStdString());

        if (!propertyId.empty())
        {
            Property *prop = agency->getPropertyManager().findProperty(propertyId);
            if (prop && !hasActiveTransactions(propertyId))
                prop->setAvailable(true);
        }

        refresh();
        emit dataChanged();
    }
}

void TransactionsWidget::searchTransactions()
{
    if (!transactionsTable || !searchTransactionEdit)
        return;

    QString searchText = searchTransactionEdit->text();
    if (searchText.isEmpty())
    {
        refresh();
        return;
    }

    transactionsTable->setRowCount(0);

    bool isNumericOnly = true;
    for (QChar ch : searchText)
    {
        if (!ch.isDigit())
        {
            isNumericOnly = false;
            break;
        }
    }

    if (isNumericOnly && searchText.length() >= 6 && searchText.length() <= 8)
    {
        Transaction *trans = agency->getTransactionManager().findTransaction(searchText.toStdString());
        if (trans)
        {
            int row = transactionsTable->rowCount();
            transactionsTable->insertRow(row);

            Property *prop = agency->getPropertyManager().findProperty(trans->getPropertyId());
            Client *client = agency->getClientManager().findClient(trans->getClientId());

            transactionsTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(trans->getId())));
            transactionsTable->setItem(row, 1,
                                       new QTableWidgetItem(prop ? QString::fromStdString(prop->getAddress()) : "N/A"));
            transactionsTable->setItem(
                row, 2, new QTableWidgetItem(client ? QString::fromStdString(client->getName()) : "N/A"));
            transactionsTable->setItem(row, 3, new QTableWidgetItem(QString::number(trans->getFinalPrice(), 'f', 2)));
            transactionsTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(trans->getDate())));
            transactionsTable->setItem(row, 5,
                                       new QTableWidgetItem(TableHelper::getTransactionStatusText(trans->getStatus())));

            QString transId = QString::fromStdString(trans->getId());
            QWidget *actionsWidget = createActionButtons(
                transactionsTable, transId, [this]() { editTransaction(); }, [this]() { deleteTransaction(); });
            transactionsTable->setCellWidget(row, 6, actionsWidget);
        }
    }
}

void TransactionsWidget::transactionSelectionChanged()
{
    if (!transactionsTable)
        return;

    int row = transactionsTable->currentRow();
    if (row >= 0)
    {
        QString id = transactionsTable->item(row, 0)->text();
        Transaction *trans = agency->getTransactionManager().findTransaction(id.toStdString());
        if (trans)
            showTransactionDetails(trans);
    }
}

void TransactionsWidget::showTransactionDetails(Transaction *trans)
{
    if (!trans)
        return;

    Property *prop = agency->getPropertyManager().findProperty(trans->getPropertyId());
    Client *client = agency->getClientManager().findClient(trans->getClientId());

    QString html;
    html += "<html><body style='font-family: Arial, sans-serif;'>";
    html += "<h2 style='font-weight: bold; margin-bottom: 15px;'>ДЕТАЛИ СДЕЛКИ</h2>";
    html += "<p><b>ID сделки:</b> " + QString::fromStdString(trans->getId()) + "</p>";
    html += "<p><b>Дата:</b> " + QString::fromStdString(trans->getDate()) + "</p>";
    html += "<p><b>Цена:</b> " + QString::number(trans->getFinalPrice(), 'f', 2) + " руб.</p>";
    html += "<p><b>Статус:</b> " + TableHelper::getTransactionStatusText(trans->getStatus()) + "</p>";
    if (!trans->getNotes().empty())
        html += "<p><b>Примечания:</b> " + QString::fromStdString(trans->getNotes()) + "</p>";

    html += "<h3 style='font-weight: bold; margin-top: 20px; margin-bottom: 10px;'>НЕДВИЖИМОСТЬ</h3>";
    if (prop)
    {
        html += "<p><b>ID:</b> " + QString::fromStdString(prop->getId()) + "</p>";
        html += "<p><b>Тип:</b> " + TableHelper::getPropertyTypeText(prop->getType()) + "</p>";
        html += "<p><b>Адрес:</b> " + QString::fromStdString(prop->getAddress()) + "</p>";
        html += "<p><b>Цена:</b> " + QString::number(prop->getPrice(), 'f', 2) + " руб.</p>";
    }
    else
        html += "<p>Недвижимость не найдена.</p>";

    html += "<h3 style='font-weight: bold; margin-top: 20px; margin-bottom: 10px;'>КЛИЕНТ</h3>";
    if (client)
    {
        html += "<p><b>ID:</b> " + QString::fromStdString(client->getId()) + "</p>";
        html += "<p><b>Имя:</b> " + QString::fromStdString(client->getName()) + "</p>";
        html += "<p><b>Телефон:</b> " + QString::fromStdString(client->getPhone()) + "</p>";
    }
    else
        html += "<p>Клиент не найден.</p>";

    html += "</body></html>";
    transactionDetailsText->setHtml(html);
}

bool TransactionsWidget::validateTransaction(const std::string &propertyId, const std::string &clientId,
                                             const std::string &status, const std::string &excludeTransactionId)
{
    Property *prop = agency->getPropertyManager().findProperty(propertyId);
    if (!prop)
    {
        QMessageBox::warning(this, "Ошибка валидации",
                             QString("Недвижимость с ID '%1' не найдена!").arg(QString::fromStdString(propertyId)));
        return false;
    }

    Client *client = agency->getClientManager().findClient(clientId);
    if (!client)
    {
        QMessageBox::warning(this, "Ошибка валидации",
                             QString("Клиент с ID '%1' не найден!").arg(QString::fromStdString(clientId)));
        return false;
    }

    if (status == "pending" || status == "completed")
    {
        auto existingTrans = agency->getTransactionManager().getTransactionsByProperty(propertyId);
        for (Transaction *t : existingTrans)
        {
            if (t && !excludeTransactionId.empty() && t->getId() == excludeTransactionId)
                continue;

            if (t && (t->getStatus() == "pending" || t->getStatus() == "completed"))
            {
                QMessageBox::warning(this, "Ошибка валидации",
                                     "Недвижимость уже используется в другой активной сделке!");
                return false;
            }
        }
    }

    return true;
}

bool TransactionsWidget::hasActiveTransactions(const std::string &propertyId)
{
    auto transactions = agency->getTransactionManager().getTransactionsByProperty(propertyId);
    for (Transaction *t : transactions)
    {
        if (t && (t->getStatus() == Constants::TransactionStatus::PENDING ||
                  t->getStatus() == Constants::TransactionStatus::COMPLETED))
            return true;
    }
    return false;
}

QWidget *TransactionsWidget::createActionButtons(QTableWidget *table, const QString &id,
                                                 std::function<void()> editAction, std::function<void()> deleteAction)
{
    auto *actionsWidget = new QWidget;
    auto *actionsLayout = new QHBoxLayout(actionsWidget);
    actionsLayout->setContentsMargins(5, 5, 5, 5);
    actionsLayout->setSpacing(8);

    auto *editBtn = new QPushButton("Редактировать");
    editBtn->setMinimumWidth(110);
    editBtn->setFixedHeight(35);
    auto *deleteBtn = new QPushButton("Удалить");
    deleteBtn->setMinimumWidth(90);
    deleteBtn->setFixedHeight(35);

    connect(editBtn, &QPushButton::clicked, this,
            [this, table, id, editAction]()
            {
                selectRowById(table, id);
                editAction();
            });
    connect(deleteBtn, &QPushButton::clicked, this,
            [this, table, id, deleteAction]()
            {
                selectRowById(table, id);
                deleteAction();
            });

    actionsLayout->addWidget(editBtn);
    actionsLayout->addWidget(deleteBtn);
    actionsLayout->addStretch();

    return actionsWidget;
}

void TransactionsWidget::selectRowById(QTableWidget *table, const QString &id)
{
    if (!table)
        return;
    for (int i = 0; i < table->rowCount(); ++i)
    {
        if (table->item(i, 0) && table->item(i, 0)->text() == id)
        {
            table->selectRow(i);
            break;
        }
    }
}

QString TransactionsWidget::getSelectedIdFromTable(QTableWidget *table) { return TableHelper::getSelectedId(table); }

bool TransactionsWidget::checkTableSelection(QTableWidget *table, const QString &errorMessage)
{
    if (!table || !TableHelper::hasValidSelection(table))
    {
        if (!errorMessage.isEmpty())
            QMessageBox::information(this, Constants::Messages::INFORMATION, errorMessage);
        return false;
    }
    return true;
}
