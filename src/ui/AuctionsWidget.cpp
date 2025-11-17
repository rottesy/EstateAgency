#include "../../include/ui/AuctionsWidget.h"
#include "../../include/core/Constants.h"
#include "../../include/core/EstateAgency.h"
#include "../../include/entities/Auction.h"
#include "../../include/entities/Bid.h"
#include "../../include/entities/Property.h"
#include "../../include/entities/Transaction.h"
#include "../../include/ui/AuctionDialog.h"
#include "../../include/ui/TableHelper.h"
#include <QAbstractItemView>
#include <QHeaderView>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <functional>

AuctionsWidget::AuctionsWidget(EstateAgency *agency, QWidget *parent) : QWidget(parent), agency(agency) { setupUI(); }

void AuctionsWidget::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    QHBoxLayout *headerLayout = new QHBoxLayout;
    QLabel *title = new QLabel("Аукционы");
    title->setStyleSheet("color: #ffffff; font-size: 22pt; font-weight: bold;");
    headerLayout->addWidget(title);
    headerLayout->addStretch();

    searchAuctionEdit = new QLineEdit;
    searchAuctionEdit->setPlaceholderText("🔍 Поиск по ID");
    searchAuctionEdit->setFixedWidth(250);
    searchAuctionBtn = new QPushButton("Найти");
    headerLayout->addWidget(searchAuctionEdit);
    headerLayout->addWidget(searchAuctionBtn);

    addAuctionBtn = new QPushButton("➕ Создать");
    refreshAuctionBtn = new QPushButton("🔄 Обновить");
    headerLayout->addWidget(addAuctionBtn);
    headerLayout->addWidget(refreshAuctionBtn);
    layout->addLayout(headerLayout);

    QSplitter *splitter = new QSplitter(Qt::Horizontal);

    auctionsTable = new QTableWidget;
    auctionsTable->setColumnCount(7);
    auctionsTable->setHorizontalHeaderLabels(
        {"ID", "Недвижимость", "Начальная цена", "Текущая ставка", "Статус", "Ставок", "Действия"});
    auctionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    auctionsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    auctionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    auctionsTable->setAlternatingRowColors(true);
    auctionsTable->verticalHeader()->setDefaultSectionSize(60);
    auctionsTable->setColumnWidth(0, 100);
    auctionsTable->setColumnWidth(1, 250);
    auctionsTable->setColumnWidth(2, 150);
    auctionsTable->setColumnWidth(3, 150);
    auctionsTable->setColumnWidth(4, 120);
    auctionsTable->setColumnWidth(5, 80);
    auctionsTable->setColumnWidth(6, 300);
    auctionsTable->horizontalHeader()->setStretchLastSection(false);

    QFrame *detailsFrame = new QFrame;
    detailsFrame->setFixedWidth(400);
    detailsFrame->setStyleSheet("background-color: #252525; border-radius: 8px; padding: 15px;");
    QVBoxLayout *detailsLayout = new QVBoxLayout(detailsFrame);
    QLabel *detailsTitle = new QLabel("Детали аукциона");
    detailsTitle->setStyleSheet("color: #ffffff; font-size: 14pt; font-weight: "
                                "bold; padding-bottom: 10px;");
    detailsLayout->addWidget(detailsTitle);
    auctionDetailsText = new QTextEdit;
    auctionDetailsText->setReadOnly(true);
    auctionDetailsText->setStyleSheet("background-color: #2d2d2d; color: #d0d0d0; border: none;");
    detailsLayout->addWidget(auctionDetailsText);

    splitter->addWidget(auctionsTable);
    splitter->addWidget(detailsFrame);
    splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 1);

    layout->addWidget(splitter);

    connect(addAuctionBtn, &QPushButton::clicked, this, &AuctionsWidget::addAuction);
    connect(refreshAuctionBtn, &QPushButton::clicked, this, &AuctionsWidget::refresh);
    connect(searchAuctionBtn, &QPushButton::clicked, this, &AuctionsWidget::searchAuctions);
    connect(auctionsTable, &QTableWidget::itemSelectionChanged, this, &AuctionsWidget::auctionSelectionChanged);
}

void AuctionsWidget::refresh() { updateTable(); }

void AuctionsWidget::updateTable()
{
    if (!auctionsTable)
        return;

    auctionsTable->setRowCount(0);
    auto auctions = agency->getAuctionManager().getAllAuctions();

    for (Auction *auction : auctions)
    {
        if (!auction)
            continue;

        int row = auctionsTable->rowCount();
        auctionsTable->insertRow(row);

        double currentBid = auction->getCurrentHighestBid();

        auctionsTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(auction->getId())));
        auctionsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(auction->getPropertyAddress())));
        auctionsTable->setItem(row, 2,
                               new QTableWidgetItem(QString::number(auction->getStartingPrice(), 'f', 2) + " руб."));

        QString currentBidText = (currentBid > 0) ? QString::number(currentBid, 'f', 2) + " руб." : "Нет ставок";
        auctionsTable->setItem(row, 3, new QTableWidgetItem(currentBidText));

        QString statusText;
        if (auction->getStatus() == "active")
            statusText = "Активен";
        else if (auction->getStatus() == "completed")
            statusText = "Завершен";
        else
            statusText = "Отменен";
        auctionsTable->setItem(row, 4, new QTableWidgetItem(statusText));
        auctionsTable->setItem(row, 5, new QTableWidgetItem(QString::number(auction->getBids().size())));

        QString auctionId = QString::fromStdString(auction->getId());
        QWidget *actionsWidget =
            createActionButtons(auctionsTable, auctionId, [this]() { viewAuction(); }, [this]() { deleteAuction(); });
        auctionsTable->setCellWidget(row, 6, actionsWidget);
    }
}

void AuctionsWidget::addAuction()
{
    QStringList propertyIds;
    auto properties = agency->getPropertyManager().getAllProperties();

    if (properties.empty())
    {
        QMessageBox::information(this, "Информация", "Нет доступной недвижимости для создания аукциона");
        return;
    }

    for (Property *p : properties)
    {
        if (!p)
            continue;

        std::string propertyId = p->getId();

        if (hasActiveTransactions(propertyId))
            continue;

        auto auctions = agency->getAuctionManager().getAuctionsByProperty(propertyId);
        bool isInActiveAuction = false;
        for (Auction *auction : auctions)
        {
            if (auction && auction->getStatus() == "active")
            {
                isInActiveAuction = true;
                break;
            }
        }

        if (!isInActiveAuction)
            propertyIds << QString::fromStdString(propertyId + " - " + p->getAddress());
    }

    if (propertyIds.isEmpty())
    {
        QMessageBox::information(this, "Информация", "Нет доступной недвижимости для создания аукциона.");
        return;
    }

    AuctionDialog dialog(this, nullptr, propertyIds);
    if (dialog.exec() == QDialog::Accepted)
    {
        try
        {
            QString id = dialog.getId();
            QString propertyId = dialog.getPropertyId();
            double startingPrice = dialog.getStartingPrice();

            Property *prop = agency->getPropertyManager().findProperty(propertyId.toStdString());
            if (!prop)
            {
                QMessageBox::warning(this, "Ошибка", "Недвижимость не найдена");
                return;
            }

            auto auction = std::make_shared<Auction>(id.toStdString(), propertyId.toStdString(), prop->getAddress(),
                                                     startingPrice);

            agency->getAuctionManager().addAuction(auction);

            refresh();
            emit dataChanged();
        }
        catch (const std::invalid_argument &e)
        {
            QMessageBox::warning(this, "Ошибка", QString("Ошибка создания аукциона: %1").arg(e.what()));
        }
        catch (const AuctionManagerException &e)
        {
            QMessageBox::warning(this, "Ошибка", QString("Ошибка создания аукциона: %1").arg(e.what()));
        }
    }
}

void AuctionsWidget::viewAuction()
{
    if (!checkTableSelection(auctionsTable, "Выберите аукцион для просмотра"))
        return;

    QString id = getSelectedIdFromTable(auctionsTable);
    Auction *auction = agency->getAuctionManager().findAuction(id.toStdString());
    if (!auction)
        return;

    QStringList propertyIds;
    auto properties = agency->getPropertyManager().getAllProperties();
    for (Property *p : properties)
        propertyIds << QString::fromStdString(p->getId() + " - " + p->getAddress());

    AuctionDialog dialog(this, auction, propertyIds);
    dialog.exec();

    refresh();
    emit dataChanged();
}

void AuctionsWidget::deleteAuction()
{
    if (!checkTableSelection(auctionsTable, "Выберите аукцион для удаления"))
        return;

    QString id = getSelectedIdFromTable(auctionsTable);
    int ret = QMessageBox::question(this, "Подтверждение", "Удалить аукцион?", QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes)
    {
        agency->getAuctionManager().removeAuction(id.toStdString());
        refresh();
        emit dataChanged();
    }
}

void AuctionsWidget::searchAuctions()
{
    if (!auctionsTable || !searchAuctionEdit)
        return;

    QString searchText = searchAuctionEdit->text();
    if (searchText.isEmpty())
    {
        refresh();
        return;
    }

    auctionsTable->setRowCount(0);

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
        Auction *auction = agency->getAuctionManager().findAuction(searchText.toStdString());
        if (auction)
        {
            int row = auctionsTable->rowCount();
            auctionsTable->insertRow(row);

            double currentBid = auction->getCurrentHighestBid();

            auctionsTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(auction->getId())));
            auctionsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(auction->getPropertyAddress())));
            auctionsTable->setItem(
                row, 2, new QTableWidgetItem(QString::number(auction->getStartingPrice(), 'f', 2) + " руб."));

            QString currentBidText = (currentBid > 0) ? QString::number(currentBid, 'f', 2) + " руб." : "Нет ставок";
            auctionsTable->setItem(row, 3, new QTableWidgetItem(currentBidText));

            QString statusText;
            if (auction->getStatus() == "active")
                statusText = "Активен";
            else if (auction->getStatus() == "completed")
                statusText = "Завершен";
            else
                statusText = "Отменен";
            auctionsTable->setItem(row, 4, new QTableWidgetItem(statusText));
            auctionsTable->setItem(row, 5, new QTableWidgetItem(QString::number(auction->getBids().size())));

            QString auctionId = QString::fromStdString(auction->getId());
            QWidget *actionsWidget = createActionButtons(
                auctionsTable, auctionId, [this]() { viewAuction(); }, [this]() { deleteAuction(); });
            auctionsTable->setCellWidget(row, 6, actionsWidget);
        }
    }
}

void AuctionsWidget::auctionSelectionChanged()
{
    if (!auctionsTable)
        return;

    int row = auctionsTable->currentRow();
    if (row >= 0 && row < auctionsTable->rowCount())
    {
        QTableWidgetItem *item = auctionsTable->item(row, 0);
        if (item)
        {
            QString id = item->text();
            Auction *auction = agency->getAuctionManager().findAuction(id.toStdString());
            if (auction)
                showAuctionDetails(auction);
        }
    }
}

void AuctionsWidget::showAuctionDetails(Auction *auction)
{
    if (!auction)
        return;

    Property *prop = agency->getPropertyManager().findProperty(auction->getPropertyId());

    QString html;
    html += "<html><body style='font-family: Arial, sans-serif;'>";
    html += "<h2 style='font-weight: bold; margin-bottom: 15px;'>ДЕТАЛИ АУКЦИОНА</h2>";

    html += "<p><b>ID аукциона:</b> " + QString::fromStdString(auction->getId()) + "</p>";
    html += "<p><b>Начальная цена:</b> " + QString::number(auction->getStartingPrice(), 'f', 2) + " руб.</p>";
    html += "<p><b>Цена автоматической покупки:</b> " + QString::number(auction->getBuyoutPrice(), 'f', 2) +
            " руб. (+70%)</p>";
    html += "<p><b>Дата создания:</b> " + QString::fromStdString(auction->getCreatedAt()) + "</p>";

    QString statusText = TableHelper::getAuctionStatusText(auction->getStatus());
    html += "<p><b>Статус:</b> " + statusText + "</p>";

    if (!auction->getCompletedAt().empty())
        html += "<p><b>Дата завершения:</b> " + QString::fromStdString(auction->getCompletedAt()) + "</p>";

    double currentBid = auction->getCurrentHighestBid();
    if (currentBid > 0)
    {
        Bid *highest = auction->getHighestBid();
        html += "<p><b>Текущая максимальная ставка:</b> " + QString::number(currentBid, 'f', 2) + " руб.</p>";
        if (highest)
            html += "<p><b>Победитель:</b> " + QString::fromStdString(highest->getClientName()) +
                    " (ID: " + QString::fromStdString(highest->getClientId()) + ")</p>";
    }
    else
        html += "<p><b>Текущая максимальная ставка:</b> Нет ставок</p>";

    html += "<p><b>Количество ставок:</b> " + QString::number(auction->getBids().size()) + "</p>";

    if (auction->wasBuyout())
        html += "<p style='margin-top: 10px;'><b>Аукцион завершен автоматической покупкой.</b></p>";

    html += "<h3 style='font-weight: bold; margin-top: 20px; margin-bottom: 10px;'>НЕДВИЖИМОСТЬ</h3>";
    if (prop)
    {
        html += "<p><b>ID:</b> " + QString::fromStdString(prop->getId()) + "</p>";
        html += "<p><b>Тип:</b> " + TableHelper::getPropertyTypeText(prop->getType()) + "</p>";
        html += "<p><b>Адрес:</b> " + QString::fromStdString(prop->getAddress()) + "</p>";
        html += "<p><b>Цена:</b> " + QString::number(prop->getPrice(), 'f', 2) + " руб.</p>";
        html += "<p><b>Площадь:</b> " + QString::number(prop->getArea(), 'f', 2) + " м²</p>";
    }
    else
        html += "<p>Недвижимость не найдена.</p>";

    html += "<h3 style='font-weight: bold; margin-top: 20px; margin-bottom: 10px;'>СТАВКИ</h3>";
    auto bids = auction->getBids();
    if (bids.empty())
        html += "<p>Ставок пока нет.</p>";
    else
    {
        int num = 1;
        for (const auto &bid : bids)
        {
            html += "<div style='margin-bottom: 15px;'>";
            html += "<h4 style='font-weight: bold; margin-bottom: 5px;'>Ставка #" + QString::number(num) + "</h4>";
            html += "<p><b>Клиент:</b> " + QString::fromStdString(bid->getClientName()) + "</p>";
            html += "<p><b>ID клиента:</b> " + QString::fromStdString(bid->getClientId()) + "</p>";
            html += "<p><b>Сумма:</b> " + QString::number(bid->getAmount(), 'f', 2) + " руб.</p>";
            html += "<p><b>Время:</b> " + QString::fromStdString(bid->getTimestamp()) + "</p>";
            html += "</div>";
            num++;
        }
    }

    html += "</body></html>";
    auctionDetailsText->setHtml(html);
}

bool AuctionsWidget::hasActiveTransactions(const std::string &propertyId)
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

QWidget *AuctionsWidget::createActionButtons(QTableWidget *table, const QString &id,
                                             const std::function<void()> &viewAction,
                                             const std::function<void()> &deleteAction, [[maybe_unused]] bool isView)
{
    QWidget *actionsWidget = new QWidget;
    QHBoxLayout *actionsLayout = new QHBoxLayout(actionsWidget);
    actionsLayout->setContentsMargins(5, 5, 5, 5);
    actionsLayout->setSpacing(8);

    QPushButton *viewBtn = new QPushButton("Просмотр");
    viewBtn->setMinimumWidth(100);
    viewBtn->setFixedHeight(35);
    QPushButton *deleteBtn = new QPushButton("Удалить");
    deleteBtn->setMinimumWidth(90);
    deleteBtn->setFixedHeight(35);

    connect(viewBtn, &QPushButton::clicked, this,
            [this, table, id, viewAction]()
            {
                selectRowById(table, id);
                viewAction();
            });
    connect(deleteBtn, &QPushButton::clicked, this,
            [this, table, id, deleteAction]()
            {
                selectRowById(table, id);
                deleteAction();
            });

    actionsLayout->addWidget(viewBtn);
    actionsLayout->addWidget(deleteBtn);
    actionsLayout->addStretch();

    return actionsWidget;
}

void AuctionsWidget::selectRowById(QTableWidget *table, const QString &id)
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

QString AuctionsWidget::getSelectedIdFromTable(QTableWidget *table) const { return TableHelper::getSelectedId(table); }

bool AuctionsWidget::checkTableSelection(QTableWidget *table, const QString &errorMessage)
{
    if (!table || !TableHelper::hasValidSelection(table))
    {
        if (!errorMessage.isEmpty())
            QMessageBox::information(this, "Информация", errorMessage);
        return false;
    }
    return true;
}
