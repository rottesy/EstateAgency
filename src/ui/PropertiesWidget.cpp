#include "../../include/ui/PropertiesWidget.h"
#include "../../include/core/Constants.h"
#include "../../include/core/Utils.h"
#include "../../include/entities/Apartment.h"
#include "../../include/entities/Client.h"
#include "../../include/entities/CommercialProperty.h"
#include "../../include/entities/House.h"
#include "../../include/entities/PropertyParams.h"
#include "../../include/entities/Transaction.h"
#include "../../include/services/PropertyManager.h"
#include "../../include/ui/PropertyDialog.h"
#include "../../include/ui/TableHelper.h"
#include <QAbstractItemView>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <functional>
#include <ranges>

PropertiesWidget::PropertiesWidget(EstateAgency *agency, QWidget *parent) : QWidget(parent), agency(agency)
{
    setupUI();
}

void PropertiesWidget::setupUI()
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    auto *headerLayout = new QHBoxLayout;
    auto *title = new QLabel("Недвижимость");
    title->setStyleSheet("color: #ffffff; font-size: 22pt; font-weight: bold;");
    headerLayout->addWidget(title);
    headerLayout->addStretch();

    searchPropertyEdit = new QLineEdit;
    searchPropertyEdit->setPlaceholderText("🔍 Поиск по ID или адресу");
    searchPropertyEdit->setFixedWidth(300);
    searchPropertyBtn = new QPushButton("Найти");
    headerLayout->addWidget(searchPropertyEdit);
    headerLayout->addWidget(searchPropertyBtn);

    addPropertyBtn = new QPushButton("➕ Добавить");
    refreshPropertyBtn = new QPushButton("🔄 Обновить");
    headerLayout->addWidget(addPropertyBtn);
    headerLayout->addWidget(refreshPropertyBtn);
    layout->addLayout(headerLayout);

    auto *splitter = new QSplitter(Qt::Horizontal);

    propertiesTable = new QTableWidget;
    propertiesTable->setColumnCount(7);
    propertiesTable->setHorizontalHeaderLabels({"ID", "Тип", "Адрес", "Цена", "Площадь", "Доступность", "Действия"});
    propertiesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    propertiesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    propertiesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    propertiesTable->verticalHeader()->setDefaultSectionSize(60);
    propertiesTable->setColumnWidth(0, 100);
    propertiesTable->setColumnWidth(1, 120);
    propertiesTable->setColumnWidth(2, 300);
    propertiesTable->setColumnWidth(3, 150);
    propertiesTable->setColumnWidth(4, 100);
    propertiesTable->setColumnWidth(5, 120);
    propertiesTable->setColumnWidth(6, 300);
    propertiesTable->horizontalHeader()->setStretchLastSection(false);

    auto *detailsFrame = new QFrame;
    detailsFrame->setFixedWidth(400);
    detailsFrame->setStyleSheet("background-color: #252525; border-radius: 8px; padding: 15px;");
    auto *detailsLayout = new QVBoxLayout(detailsFrame);
    auto *detailsTitle = new QLabel("Информация");
    detailsTitle->setStyleSheet("color: #ffffff; font-size: 14pt; font-weight: "
                                "bold; padding-bottom: 10px;");
    detailsLayout->addWidget(detailsTitle);
    propertyDetailsText = new QTextEdit;
    propertyDetailsText->setReadOnly(true);
    propertyDetailsText->setStyleSheet("background-color: #2d2d2d; color: #d0d0d0; border: none;");
    detailsLayout->addWidget(propertyDetailsText);

    splitter->addWidget(propertiesTable);
    splitter->addWidget(detailsFrame);
    splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 1);

    layout->addWidget(splitter);

    connect(addPropertyBtn, &QPushButton::clicked, this, &PropertiesWidget::addProperty);
    connect(refreshPropertyBtn, &QPushButton::clicked, this, &PropertiesWidget::refresh);
    connect(searchPropertyBtn, &QPushButton::clicked, this, &PropertiesWidget::searchProperties);
    connect(propertiesTable, &QTableWidget::itemSelectionChanged, this, &PropertiesWidget::propertySelectionChanged);
}

void PropertiesWidget::refresh() { updateTable(); }

void PropertiesWidget::updateTable()
{
    if (!propertiesTable)
    {
        return;
    }

    TableHelper::clearTable(propertiesTable);
    auto properties = agency->getPropertyManager().getAllProperties();

    for (const Property *prop : properties)
    {
        if (!prop)
            continue;
        addPropertyToTable(prop);
    }
}

void PropertiesWidget::addProperty()
{
    PropertyDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        try
        {
            QString id = dialog.getId();
            QString city = dialog.getCity();
            QString street = dialog.getStreet();
            QString house = dialog.getHouse();
            double price = dialog.getPrice();
            double area = dialog.getArea();
            QString description = dialog.getDescription();

            if (dialog.getPropertyType() == PropertyDialog::PropertyType::TypeApartment)
            {
                ApartmentParams params{{Utils::toString(id), Utils::toString(city), Utils::toString(street),
                                        Utils::toString(house), price, area, Utils::toString(description)},
                                       dialog.getRooms(),
                                       dialog.getFloor(),
                                       dialog.getHasBalcony(),
                                       dialog.getHasElevator()};
                agency->getPropertyManager().addApartment(params);
            }
            else if (dialog.getPropertyType() == PropertyDialog::PropertyType::TypeHouse)
            {
                HouseParams params{{Utils::toString(id), Utils::toString(city), Utils::toString(street),
                                    Utils::toString(house), price, area, Utils::toString(description)},
                                   dialog.getFloors(),
                                   dialog.getRooms(),
                                   dialog.getLandArea(),
                                   dialog.getHasGarage(),
                                   dialog.getHasGarden()};
                agency->getPropertyManager().addHouse(params);
            }
            else
            {
                CommercialPropertyParams params{{Utils::toString(id), Utils::toString(city), Utils::toString(street),
                                                 Utils::toString(house), price, area, Utils::toString(description)},
                                                Utils::toString(dialog.getBusinessType()),
                                                dialog.getHasParking(),
                                                dialog.getParkingSpaces(),
                                                dialog.getIsVisibleFromStreet()};
                agency->getPropertyManager().addCommercialProperty(params);
            }

            refresh();
            emit dataChanged();
        }
        catch (const PropertyManagerException &e)
        {
            QMessageBox::warning(this, Constants::Messages::ERROR,
                                 QString("%1: %2").arg(Constants::ErrorMessages::ADD_ERROR, e.what()));
        }
        catch (const std::invalid_argument &e)
        {
            QMessageBox::warning(this, Constants::Messages::ERROR,
                                 QString("%1: %2").arg(Constants::ErrorMessages::ADD_ERROR, e.what()));
        }
    }
}

void PropertiesWidget::editProperty()
{
    if (!checkTableSelection(propertiesTable, Constants::SelectionMessages::SELECT_PROPERTY_TO_EDIT))
    {
        return;
    }

    QString id = getSelectedIdFromTable(propertiesTable);
    const Property *prop = agency->getPropertyManager().findProperty(Utils::toString(id));
    if (!prop)
        return;

    PropertyDialog dialog(this, prop);
    if (dialog.exec() == QDialog::Accepted)
    {
        try
        {
            agency->getPropertyManager().removeProperty(id.toStdString());

            QString newId = dialog.getId();
            QString city = dialog.getCity();
            QString street = dialog.getStreet();
            QString house = dialog.getHouse();
            double price = dialog.getPrice();
            double area = dialog.getArea();
            QString description = dialog.getDescription();

            if (dialog.getPropertyType() == PropertyDialog::PropertyType::TypeApartment)
            {
                ApartmentParams params{{Utils::toString(newId), Utils::toString(city), Utils::toString(street),
                                        Utils::toString(house), price, area, Utils::toString(description)},
                                       dialog.getRooms(),
                                       dialog.getFloor(),
                                       dialog.getHasBalcony(),
                                       dialog.getHasElevator()};
                agency->getPropertyManager().addApartment(params);
            }
            else if (dialog.getPropertyType() == PropertyDialog::PropertyType::TypeHouse)
            {
                HouseParams params{{Utils::toString(newId), Utils::toString(city), Utils::toString(street),
                                    Utils::toString(house), price, area, Utils::toString(description)},
                                   dialog.getFloors(),
                                   dialog.getRooms(),
                                   dialog.getLandArea(),
                                   dialog.getHasGarage(),
                                   dialog.getHasGarden()};
                agency->getPropertyManager().addHouse(params);
            }
            else
            {
                CommercialPropertyParams params{{Utils::toString(newId), Utils::toString(city), Utils::toString(street),
                                                 Utils::toString(house), price, area, Utils::toString(description)},
                                                Utils::toString(dialog.getBusinessType()),
                                                dialog.getHasParking(),
                                                dialog.getParkingSpaces(),
                                                dialog.getIsVisibleFromStreet()};
                agency->getPropertyManager().addCommercialProperty(params);
            }

            refresh();
            emit dataChanged();
        }
        catch (const PropertyManagerException &e)
        {
            QMessageBox::warning(this, Constants::Messages::ERROR,
                                 QString("%1: %2").arg(Constants::ErrorMessages::EDIT_ERROR, e.what()));
        }
        catch (const std::invalid_argument &e)
        {
            QMessageBox::warning(this, Constants::Messages::ERROR,
                                 QString("%1: %2").arg(Constants::ErrorMessages::EDIT_ERROR, e.what()));
        }
    }
}

void PropertiesWidget::deleteProperty()
{
    if (!checkTableSelection(propertiesTable, Constants::SelectionMessages::SELECT_PROPERTY_TO_DELETE))
    {
        return;
    }

    QString id = getSelectedIdFromTable(propertiesTable);
    int ret =
        QMessageBox::question(this, Constants::Messages::CONFIRMATION, Constants::ConfirmationMessages::DELETE_PROPERTY,
                              QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes)
    {
        agency->getPropertyManager().removeProperty(Utils::toString(id));
        refresh();
        emit dataChanged();
    }
}

void PropertiesWidget::searchProperties()
{
    if (!propertiesTable || !searchPropertyEdit)
    {
        return;
    }

    QString searchText = searchPropertyEdit->text();
    if (searchText.isEmpty())
    {
        refresh();
        return;
    }

    propertiesTable->setRowCount(0);

    bool isNumericOnly = true;
    for (QChar ch : searchText)
    {
        if (!ch.isDigit())
        {
            isNumericOnly = false;
            break;
        }
    }

    std::vector<Property *> properties;
    if (isNumericOnly && searchText.length() >= 6 && searchText.length() <= 8)
    {
        Property *prop = agency->getPropertyManager().findProperty(searchText.toStdString());
        if (prop)
        {
            properties.push_back(prop);
        }
    }
    else
    {
        properties = agency->getPropertyManager().searchByAddress(searchText.toStdString(), "", "");
    }

    for (const Property *prop : properties)
    {
        if (!prop)
            continue;
        addPropertyToTable(prop);
    }
}

void PropertiesWidget::propertySelectionChanged()
{
    if (!propertiesTable)
    {
        return;
    }

    int row = propertiesTable->currentRow();
    if (row >= 0 && row < propertiesTable->rowCount())
    {
        const QTableWidgetItem *item = propertiesTable->item(row, 0);
        if (item)
        {
            QString id = item->text();
            if (const Property *prop = agency->getPropertyManager().findProperty(id.toStdString()); prop)
            {
                showPropertyDetails(prop);
                showPropertyTransactions(id.toStdString());
            }
        }
    }
}

void PropertiesWidget::showPropertyDetails(const Property *prop)
{
    if (!prop)
        return;

    QString html;
    html += "<html><body style='font-family: Arial, sans-serif;'>";
    html += "<h2 style='font-weight: bold; margin-bottom: 15px;'>ИНФОРМАЦИЯ О "
            "НЕДВИЖИМОСТИ</h2>";

    html += "<p><b>ID:</b> " + QString::fromStdString(prop->getId()) + "</p>";
    html += "<p><b>Тип:</b> " + TableHelper::getPropertyTypeText(prop->getType()) + "</p>";
    html += "<p><b>Город:</b> " + QString::fromStdString(prop->getCity()) + "</p>";
    html += "<p><b>Улица:</b> " + QString::fromStdString(prop->getStreet()) + "</p>";
    html += "<p><b>Дом:</b> " + QString::fromStdString(prop->getHouse()) + "</p>";
    html += "<p><b>Адрес:</b> " + QString::fromStdString(prop->getAddress()) + "</p>";
    html += "<p><b>Цена:</b> " + QString::number(prop->getPrice(), 'f', 2) + " руб.</p>";
    html += "<p><b>Площадь:</b> " + QString::number(prop->getArea(), 'f', 2) + " м²</p>";
    html += "<p><b>Доступность:</b> " + QString(prop->getIsAvailable() ? "Доступна" : "Недоступна") + "</p>";
    if (!prop->getDescription().empty())
    {
        html += "<p><b>Описание:</b> " + QString::fromStdString(prop->getDescription()) + "</p>";
    }

    html += "<h3 style='font-weight: bold; margin-top: 20px; margin-bottom: "
            "10px;'>ДОПОЛНИТЕЛЬНЫЕ ХАРАКТЕРИСТИКИ</h3>";

    if (const auto *apt = dynamic_cast<const Apartment *>(prop))
    {
        html += "<p><b>Комнат:</b> " + QString::number(apt->getRooms()) + "</p>";
        html += "<p><b>Этаж:</b> " + QString::number(apt->getFloor()) + "</p>";
        html += "<p><b>Балкон:</b> " + QString(apt->getHasBalcony() ? "Да" : "Нет") + "</p>";
        html += "<p><b>Лифт:</b> " + QString(apt->getHasElevator() ? "Да" : "Нет") + "</p>";
    }
    else if (const auto *house = dynamic_cast<const House *>(prop))
    {
        html += "<p><b>Этажей:</b> " + QString::number(house->getFloors()) + "</p>";
        html += "<p><b>Комнат:</b> " + QString::number(house->getRooms()) + "</p>";
        html += "<p><b>Участок:</b> " + QString::number(house->getLandArea(), 'f', 2) + " м²</p>";
        html += "<p><b>Гараж:</b> " + QString(house->getHasGarage() ? "Да" : "Нет") + "</p>";
        html += "<p><b>Сад:</b> " + QString(house->getHasGarden() ? "Да" : "Нет") + "</p>";
    }
    else if (const auto *comm = dynamic_cast<const CommercialProperty *>(prop))
    {
        html += "<p><b>Тип бизнеса:</b> " + QString::fromStdString(comm->getBusinessType()) + "</p>";
        html += "<p><b>Парковка:</b> " + QString(comm->getHasParking() ? "Да" : "Нет") + "</p>";
        html += "<p><b>Мест парковки:</b> " + QString::number(comm->getParkingSpaces()) + "</p>";
        html += "<p><b>Видимость с улицы:</b> " + QString(comm->getIsVisibleFromStreet() ? "Да" : "Нет") + "</p>";
    }

    html += "</body></html>";
    propertyDetailsText->setHtml(html);
}

void PropertiesWidget::showPropertyTransactions(const std::string &propertyId)
{
    auto transactions = agency->getTransactionManager().getTransactionsByProperty(propertyId);

    QString html;
    if (QString existingHtml = propertyDetailsText->toHtml();
        !existingHtml.isEmpty() && !existingHtml.contains("<body>"))
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
            "10px;'>СВЯЗАННЫЕ СДЕЛКИ</h3>";

    if (transactions.empty())
    {
        html += "<p>У недвижимости пока нет связанных сделок.</p>";
    }
    else
    {
        int num = 1;
        for (const Transaction *trans : transactions)
        {
            if (!trans)
                continue;

            const Client *client = agency->getClientManager().findClient(trans->getClientId());
            html += "<div style='margin-bottom: 15px;'>";
            html += "<h4 style='font-weight: bold; margin-bottom: 5px;'>Сделка #" + QString::number(num) + "</h4>";
            html += "<p><b>ID сделки:</b> " + QString::fromStdString(trans->getId()) + "</p>";
            html += "<p><b>Клиент:</b> " + QString::fromStdString(client ? client->getName() : "Не указан") + "</p>";
            html += "<p><b>Цена:</b> " + QString::number(trans->getFinalPrice(), 'f', 2) + " руб.</p>";
            html += "<p><b>Статус:</b> " + TableHelper::getTransactionStatusText(trans->getStatus()) + "</p>";
            html += "<p><b>Дата:</b> " + QString::fromStdString(trans->getDate()) + "</p>";
            html += "</div>";
            num++;
        }
    }

    html += "</body></html>";
    propertyDetailsText->setHtml(html);
}

QWidget *PropertiesWidget::createActionButtons(QTableWidget *table, const QString &id,
                                               const std::function<void()> &editAction,
                                               const std::function<void()> &deleteAction)
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

void PropertiesWidget::selectRowById(QTableWidget *table, const QString &id) const
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

QString PropertiesWidget::getSelectedIdFromTable(const QTableWidget *table) const
{
    return TableHelper::getSelectedId(table);
}

bool PropertiesWidget::checkTableSelection(const QTableWidget *table, const QString &errorMessage)
{
    if (!table || !TableHelper::hasValidSelection(table))
    {
        if (!errorMessage.isEmpty())
        {
            QMessageBox::information(this, QString("Информация"), errorMessage);
        }
        return false;
    }
    return true;
}

bool PropertiesWidget::isNumericId(const QString &text) const
{
    return std::ranges::all_of(text, [](const QChar &ch) { return ch.isDigit(); });
}

void PropertiesWidget::addPropertyToTable(const Property *prop)
{
    if (!prop || !propertiesTable)
        return;

    int row = propertiesTable->rowCount();
    propertiesTable->insertRow(row);

    propertiesTable->setItem(row, 0, new QTableWidgetItem(Utils::toQString(prop->getId())));
    propertiesTable->setItem(row, 1, new QTableWidgetItem(TableHelper::getPropertyTypeText(prop->getType())));
    propertiesTable->setItem(row, 2, new QTableWidgetItem(Utils::toQString(prop->getAddress())));
    propertiesTable->setItem(row, 3, new QTableWidgetItem(Utils::formatNumber(prop->getPrice())));
    propertiesTable->setItem(row, 4, new QTableWidgetItem(Utils::formatNumber(prop->getArea())));
    propertiesTable->setItem(row, 5, new QTableWidgetItem(prop->getIsAvailable() ? "Да" : "Нет"));

    QString propId = Utils::toQString(prop->getId());
    QWidget *actionsWidget =
        createActionButtons(propertiesTable, propId, [this]() { editProperty(); }, [this]() { deleteProperty(); });
    propertiesTable->setCellWidget(row, 6, actionsWidget);
}
