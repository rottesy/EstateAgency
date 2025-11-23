#include "../../include/ui/ClientsWidget.h"
#include "../../include/core/Constants.h"
#include "../../include/core/Utils.h"
#include "../../include/entities/Client.h"
#include "../../include/entities/Property.h"
#include "../../include/entities/Transaction.h"
#include "../../include/services/ClientManager.h"
#include "../../include/ui/ClientDialog.h"
#include "../../include/ui/TableHelper.h"
#include <QAbstractItemView>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QTableWidgetItem>

ClientsWidget::ClientsWidget(EstateAgency *agency, QWidget *parent) : QWidget(parent), agency(agency) { setupUI(); }

void ClientsWidget::setupUI()
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    auto *headerLayout = new QHBoxLayout;
    auto *title = new QLabel("Клиенты");
    title->setStyleSheet("color: #ffffff; font-size: 22pt; font-weight: bold;");
    headerLayout->addWidget(title);
    headerLayout->addStretch();

    searchClientEdit = new QLineEdit;
    searchClientEdit->setPlaceholderText("🔍 Поиск по ID или имени");
    searchClientEdit->setFixedWidth(300);
    searchClientBtn = new QPushButton("Найти");
    headerLayout->addWidget(searchClientEdit);
    headerLayout->addWidget(searchClientBtn);

    addClientBtn = new QPushButton("➕ Добавить");
    refreshClientBtn = new QPushButton("🔄 Обновить");
    headerLayout->addWidget(addClientBtn);
    headerLayout->addWidget(refreshClientBtn);
    layout->addLayout(headerLayout);

    auto *splitter = new QSplitter(Qt::Horizontal);

    clientsTable = new QTableWidget;
    clientsTable->setColumnCount(6);
    clientsTable->setHorizontalHeaderLabels({"ID", "Имя", "Телефон", "Email", "Дата регистрации", "Действия"});
    clientsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    clientsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    clientsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    clientsTable->setAlternatingRowColors(true);
    clientsTable->verticalHeader()->setDefaultSectionSize(60);
    clientsTable->setColumnWidth(0, 100);
    clientsTable->setColumnWidth(1, 200);
    clientsTable->setColumnWidth(2, 150);
    clientsTable->setColumnWidth(3, 250);
    clientsTable->setColumnWidth(4, 150);
    clientsTable->setColumnWidth(5, 300);
    clientsTable->horizontalHeader()->setStretchLastSection(false);

    auto *detailsFrame = new QFrame;
    detailsFrame->setFixedWidth(400);
    detailsFrame->setStyleSheet("background-color: #252525; border-radius: 8px; padding: 15px;");
    auto *detailsLayout = new QVBoxLayout(detailsFrame);
    auto *detailsTitle = new QLabel("Информация о клиенте");
    detailsTitle->setStyleSheet("color: #ffffff; font-size: 14pt; font-weight: "
                                "bold; padding-bottom: 10px;");
    detailsLayout->addWidget(detailsTitle);
    clientDetailsText = new QTextEdit;
    clientDetailsText->setReadOnly(true);
    clientDetailsText->setStyleSheet("background-color: #2d2d2d; color: #d0d0d0; border: none;");
    detailsLayout->addWidget(clientDetailsText);

    splitter->addWidget(clientsTable);
    splitter->addWidget(detailsFrame);
    splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 1);

    layout->addWidget(splitter);

    connect(addClientBtn, &QPushButton::clicked, this, &ClientsWidget::addClient);
    connect(refreshClientBtn, &QPushButton::clicked, this, &ClientsWidget::refresh);
    connect(searchClientBtn, &QPushButton::clicked, this, &ClientsWidget::searchClients);
    connect(clientsTable, &QTableWidget::itemSelectionChanged, this, &ClientsWidget::clientSelectionChanged);
}

void ClientsWidget::refresh() { updateTable(); }

void ClientsWidget::updateTable()
{
    if (!clientsTable)
    {
        return;
    }

    TableHelper::clearTable(clientsTable);
    auto clients = agency->getClientManager().getAllClients();

    for (const Client *client : clients)
    {
        if (!client)
            continue;
        addClientToTable(client);
    }
}

void ClientsWidget::addClient()
{
    ClientDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        try
        {
            auto client =
                std::make_shared<Client>(Utils::toString(dialog.getId()), Utils::toString(dialog.getName()),
                                         Utils::toString(dialog.getPhone()), Utils::toString(dialog.getEmail()));
            agency->getClientManager().addClient(client);
            refresh();
            emit dataChanged();
        }
        catch (const std::invalid_argument &e)
        {
            QMessageBox::warning(this, Constants::Messages::ERROR,
                                 QString("%1: %2").arg(Constants::ErrorMessages::ADD_ERROR, e.what()));
        }
        catch (const ClientManagerException &e)
        {
            QMessageBox::warning(this, Constants::Messages::ERROR,
                                 QString("%1: %2").arg(Constants::ErrorMessages::ADD_ERROR, e.what()));
        }
    }
}

void ClientsWidget::editClient()
{
    if (!checkTableSelection(clientsTable, Constants::SelectionMessages::SELECT_CLIENT_TO_EDIT))
    {
        return;
    }

    QString id = getSelectedIdFromTable(clientsTable);
    const Client *const client = agency->getClientManager().findClient(Utils::toString(id));
    if (client == nullptr)
    {
        return;
    }

    ClientDialog dialog(this, client);
    if (dialog.exec() == QDialog::Accepted)
    {
        try
        {
            agency->getClientManager().removeClient(Utils::toString(id));
            auto newClient =
                std::make_shared<Client>(Utils::toString(dialog.getId()), Utils::toString(dialog.getName()),
                                         Utils::toString(dialog.getPhone()), Utils::toString(dialog.getEmail()));
            agency->getClientManager().addClient(newClient);
            refresh();
            emit dataChanged();
        }
        catch (const std::invalid_argument &e)
        {
            QMessageBox::warning(this, Constants::Messages::ERROR,
                                 QString("%1: %2").arg(Constants::ErrorMessages::EDIT_ERROR, e.what()));
        }
        catch (const ClientManagerException &e)
        {
            QMessageBox::warning(this, Constants::Messages::ERROR,
                                 QString("%1: %2").arg(Constants::ErrorMessages::EDIT_ERROR, e.what()));
        }
    }
}

void ClientsWidget::deleteClient()
{
    if (!checkTableSelection(clientsTable, Constants::SelectionMessages::SELECT_CLIENT_TO_DELETE))
    {
        return;
    }

    QString id = getSelectedIdFromTable(clientsTable);
    int ret = QMessageBox::question(this, Constants::Messages::CONFIRMATION,
                                    Constants::ConfirmationMessages::DELETE_CLIENT, QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes)
    {
        agency->getClientManager().removeClient(Utils::toString(id));
        refresh();
        emit dataChanged();
    }
}

void ClientsWidget::searchClients()
{
    if (!clientsTable || !searchClientEdit)
    {
        return;
    }

    QString searchText = searchClientEdit->text();
    if (searchText.isEmpty())
    {
        refresh();
        return;
    }

    clientsTable->setRowCount(0);

    bool isNumericOnly = true;
    for (QChar ch : searchText)
    {
        if (!ch.isDigit())
        {
            isNumericOnly = false;
            break;
        }
    }

    std::vector<Client *> clients;
    if (isNumericOnly && searchText.length() >= 6 && searchText.length() <= 8)
    {
        if (Client *client = agency->getClientManager().findClient(searchText.toStdString()); client)
        {
            clients.push_back(client);
        }
    }
    else
    {
        clients = agency->getClientManager().searchByName(searchText.toStdString());
    }

    for (const Client *client : clients)
    {
        if (!client)
            continue;
        addClientToTable(client);
    }
}

void ClientsWidget::clientSelectionChanged()
{
    if (!clientsTable)
    {
        return;
    }

    int row = clientsTable->currentRow();
    if (row >= 0)
    {
        QString id = clientsTable->item(row, 0)->text();
        if (const Client *client = agency->getClientManager().findClient(id.toStdString()); client)
        {
            showClientDetails(client);
            showClientTransactions(id.toStdString());
        }
    }
}

void ClientsWidget::showClientDetails(const Client *client)
{
    if (!client)
        return;

    QString html;
    html += "<html><body style='font-family: Arial, sans-serif;'>";
    html += "<h2 style='font-weight: bold; margin-bottom: 15px;'>ИНФОРМАЦИЯ О "
            "КЛИЕНТЕ</h2>";
    html += "<p><b>Идентификатор:</b> " + QString::fromStdString(client->getId()) + "</p>";
    html += "<p><b>Имя:</b> " + QString::fromStdString(client->getName()) + "</p>";
    html += "<p><b>Телефон:</b> " + QString::fromStdString(client->getPhone()) + "</p>";
    html += "<p><b>Электронная почта:</b> " + QString::fromStdString(client->getEmail()) + "</p>";
    html += "<p><b>Дата регистрации:</b> " + QString::fromStdString(client->getRegistrationDate()) + "</p>";
    html += "</body></html>";

    clientDetailsText->setHtml(html);
}

void ClientsWidget::showClientTransactions(const std::string &clientId)
{
    auto transactions = agency->getTransactionManager().getTransactionsByClient(clientId);

    QString html;
    if (QString existingHtml = clientDetailsText->toHtml(); !existingHtml.isEmpty() && !existingHtml.contains("<body>"))
    {
        html = "<html><body style='font-family: Arial, sans-serif;'>";
    }
    else if (!existingHtml.isEmpty())
    {
        html = existingHtml;
        html = html.replace("</body></html>", "");
    }
    else
    {
        html = "<html><body style='font-family: Arial, sans-serif;'>";
    }

    html += "<h3 style='font-weight: bold; margin-top: 20px; margin-bottom: "
            "10px;'>СВЯЗАННЫЕ СДЕЛКИ (" +
            QString::number(transactions.size()) + ")</h3>";

    if (transactions.empty())
    {
        html += "<p>У клиента пока нет связанных сделок.</p>";
    }
    else
    {
        int num = 1;
        for (const Transaction *trans : transactions)
        {
            if (!trans)
                continue;

            const Property *prop = agency->getPropertyManager().findProperty(trans->getPropertyId());
            html += "<div style='margin-bottom: 15px;'>";
            html += "<h4 style='font-weight: bold; margin-bottom: 5px;'>Сделка #" + QString::number(num) + "</h4>";
            html += "<p><b>ID сделки:</b> " + QString::fromStdString(trans->getId()) + "</p>";
            html +=
                "<p><b>Недвижимость:</b> " + QString::fromStdString(prop ? prop->getAddress() : "Не указана") + "</p>";
            html += "<p><b>Цена:</b> " + QString::number(trans->getFinalPrice(), 'f', 2) + " руб.</p>";
            html += "<p><b>Статус:</b> " + TableHelper::getTransactionStatusText(trans->getStatus()) + "</p>";
            html += "<p><b>Дата:</b> " + QString::fromStdString(trans->getDate()) + "</p>";
            html += "</div>";
            num++;
        }
    }

    html += "</body></html>";
    clientDetailsText->setHtml(html);
}

void ClientsWidget::selectRowById(QTableWidget *table, const QString &id) const
{
    TableHelper::selectRowById(table, id);
}

QString ClientsWidget::getSelectedIdFromTable(const QTableWidget *table) const
{
    return TableHelper::getSelectedId(table);
}

bool ClientsWidget::checkTableSelection(const QTableWidget *table, const QString &errorMessage)
{
    return TableHelper::checkTableSelection(table, errorMessage, this);
}

void ClientsWidget::addClientToTable(const Client *client)
{
    if (!client || !clientsTable)
        return;

    int row = clientsTable->rowCount();
    clientsTable->insertRow(row);

    clientsTable->setItem(row, 0, new QTableWidgetItem(Utils::toQString(client->getId())));
    clientsTable->setItem(row, 1, new QTableWidgetItem(Utils::toQString(client->getName())));
    clientsTable->setItem(row, 2, new QTableWidgetItem(Utils::toQString(client->getPhone())));
    clientsTable->setItem(row, 3, new QTableWidgetItem(Utils::toQString(client->getEmail())));
    clientsTable->setItem(row, 4, new QTableWidgetItem(Utils::toQString(client->getRegistrationDate())));

    QString clientId = Utils::toQString(client->getId());
    QWidget *actionsWidget = TableHelper::createActionButtons(
        clientsTable, clientId, this, [this]() { editClient(); }, [this]() { deleteClient(); });
    clientsTable->setCellWidget(row, 5, actionsWidget);
}
