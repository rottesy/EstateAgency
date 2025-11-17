#include "../../include/ui/PropertyDialog.h"
#include "../../include/entities/Apartment.h"
#include "../../include/entities/CommercialProperty.h"
#include "../../include/entities/House.h"
#include "../../include/entities/Property.h"
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QVBoxLayout>

PropertyDialog::PropertyDialog(QWidget *parent, Property *editProperty) : QDialog(parent)
{
    setStyleSheet(R"(
        QDialog {
            background-color: #2d2d2d;
        }
        QLabel {
            color: #d0d0d0;
        }
        QLineEdit, QTextEdit {
            background-color: #2d2d2d;
            color: #d0d0d0;
            border: 1px solid #404040;
        }
        QComboBox {
            background-color: #2d2d2d;
            color: #d0d0d0;
            border: 1px solid #404040;
        }
        QComboBox QAbstractItemView {
            background-color: #2d2d2d;
            color: #d0d0d0;
            selection-background-color: #5a9;
            selection-color: white;
        }
        QSpinBox, QDoubleSpinBox {
            background-color: #2d2d2d;
            color: #d0d0d0;
            border: 1px solid #404040;
        }
        QGroupBox {
            background-color: #2d2d2d;
            color: #d0d0d0;
            border: 1px solid #404040;
        }
        QCheckBox {
            color: #d0d0d0;
        }
        QDialogButtonBox QPushButton {
            background-color: #404040;
            color: white;
            min-width: 80px;
            padding: 6px;
        }
        QDialogButtonBox QPushButton:hover {
            background-color: #505050;
        }
    )");

    setupUI();

    if (editProperty)
    {
        loadPropertyData(editProperty);
    }

    setWindowTitle(editProperty ? "Редактировать недвижимость" : "Добавить недвижимость");
    setMinimumWidth(500);
}

PropertyDialog::~PropertyDialog() = default;

void PropertyDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);

    auto *typeLayout = new QHBoxLayout;
    typeLayout->addWidget(new QLabel("Тип:"));
    typeCombo = new QComboBox;
    typeCombo->addItems({"Квартира", "Дом", "Коммерческая"});
    typeLayout->addWidget(typeCombo);
    mainLayout->addLayout(typeLayout);

    auto *commonLayout = new QFormLayout;
    idEdit = new QLineEdit;
    idEdit->setPlaceholderText("123456 (6-8 цифр)");
    idEdit->setMaxLength(8);
    idEdit->setToolTip("ID должен содержать только цифры\nДлина: от 6 до 8 символов");
    cityEdit = new QLineEdit;
    cityEdit->setPlaceholderText("г. Минск");
    cityEdit->setToolTip("Название города");
    streetEdit = new QLineEdit;
    streetEdit->setPlaceholderText("ул. Ленина");
    streetEdit->setToolTip("Название улицы");
    houseEdit = new QLineEdit;
    houseEdit->setPlaceholderText("д. 10");
    houseEdit->setToolTip("Номер дома");
    priceSpin = new QDoubleSpinBox;
    priceSpin->setMinimum(10000);
    priceSpin->setMaximum(1000000000);
    priceSpin->setValue(10000);
    priceSpin->setSuffix(" руб.");
    priceSpin->setToolTip("Минимальная цена: 10 000 руб.\nМаксимальная цена: 1 000 000 000 руб.");
    areaSpin = new QDoubleSpinBox;
    areaSpin->setMaximum(100000);
    areaSpin->setSuffix(" м²");
    descriptionEdit = new QTextEdit;
    descriptionEdit->setMaximumHeight(100);
    availableCheck = new QCheckBox("Доступна");
    availableCheck->setChecked(true);

    commonLayout->addRow("ID:", idEdit);
    commonLayout->addRow("Город:", cityEdit);
    commonLayout->addRow("Улица:", streetEdit);
    commonLayout->addRow("Дом:", houseEdit);
    commonLayout->addRow("Цена:", priceSpin);
    commonLayout->addRow("Площадь:", areaSpin);
    commonLayout->addRow("Описание:", descriptionEdit);
    commonLayout->addRow(availableCheck);

    auto *commonGroup = new QGroupBox("Общая информация");
    commonGroup->setLayout(commonLayout);
    mainLayout->addWidget(commonGroup);

    apartmentGroup = new QGroupBox("Параметры квартиры");
    auto *aptLayout = new QFormLayout;
    roomsSpin = new QSpinBox;
    roomsSpin->setRange(1, 10);
    floorSpin = new QSpinBox;
    floorSpin->setRange(1, 100);
    balconyCheck = new QCheckBox;
    elevatorCheck = new QCheckBox;

    aptLayout->addRow("Комнат:", roomsSpin);
    aptLayout->addRow("Этаж:", floorSpin);
    aptLayout->addRow("Балкон:", balconyCheck);
    aptLayout->addRow("Лифт:", elevatorCheck);
    apartmentGroup->setLayout(aptLayout);
    mainLayout->addWidget(apartmentGroup);

    houseGroup = new QGroupBox("Параметры дома");
    auto *houseLayout = new QFormLayout;
    floorsSpin = new QSpinBox;
    floorsSpin->setRange(1, 10);
    houseRoomsSpin = new QSpinBox;
    houseRoomsSpin->setRange(1, 50);
    landAreaSpin = new QDoubleSpinBox;
    landAreaSpin->setMaximum(10000);
    landAreaSpin->setSuffix(" м²");
    garageCheck = new QCheckBox;
    gardenCheck = new QCheckBox;

    houseLayout->addRow("Этажей:", floorsSpin);
    houseLayout->addRow("Комнат:", houseRoomsSpin);
    houseLayout->addRow("Участок:", landAreaSpin);
    houseLayout->addRow("Гараж:", garageCheck);
    houseLayout->addRow("Сад:", gardenCheck);
    houseGroup->setLayout(houseLayout);
    houseGroup->setVisible(false);
    mainLayout->addWidget(houseGroup);

    commercialGroup = new QGroupBox("Параметры коммерческой");
    auto *commLayout = new QFormLayout;
    businessTypeEdit = new QLineEdit;
    parkingCheck = new QCheckBox;
    parkingSpacesSpin = new QSpinBox;
    parkingSpacesSpin->setRange(0, 1000);
    visibleFromStreetCheck = new QCheckBox;

    commLayout->addRow("Тип бизнеса:", businessTypeEdit);
    commLayout->addRow("Парковка:", parkingCheck);
    commLayout->addRow("Мест парковки:", parkingSpacesSpin);
    commLayout->addRow("Видимость с улицы:", visibleFromStreetCheck);
    commercialGroup->setLayout(commLayout);
    commercialGroup->setVisible(false);
    mainLayout->addWidget(commercialGroup);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    mainLayout->addWidget(buttonBox);

    connect(typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PropertyDialog::propertyTypeChanged);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &PropertyDialog::validateAndAccept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void PropertyDialog::propertyTypeChanged(int index)
{
    apartmentGroup->setVisible(index == 0);
    houseGroup->setVisible(index == 1);
    commercialGroup->setVisible(index == 2);
}

void PropertyDialog::loadPropertyData(Property *prop)
{
    idEdit->setText(QString::fromStdString(prop->getId()));
    cityEdit->setText(QString::fromStdString(prop->getCity()));
    streetEdit->setText(QString::fromStdString(prop->getStreet()));
    houseEdit->setText(QString::fromStdString(prop->getHouse()));
    priceSpin->setValue(prop->getPrice());
    areaSpin->setValue(prop->getArea());
    descriptionEdit->setPlainText(QString::fromStdString(prop->getDescription()));
    availableCheck->setChecked(prop->getIsAvailable());

    if (const auto *apt = dynamic_cast<const Apartment *>(prop))
    {
        typeCombo->setCurrentIndex(0);
        roomsSpin->setValue(apt->getRooms());
        floorSpin->setValue(apt->getFloor());
        balconyCheck->setChecked(apt->getHasBalcony());
        elevatorCheck->setChecked(apt->getHasElevator());
    }
    else if (const auto *house = dynamic_cast<const House *>(prop))
    {
        typeCombo->setCurrentIndex(1);
        floorsSpin->setValue(house->getFloors());
        houseRoomsSpin->setValue(house->getRooms());
        landAreaSpin->setValue(house->getLandArea());
        garageCheck->setChecked(house->getHasGarage());
        gardenCheck->setChecked(house->getHasGarden());
    }
    else if (const auto *comm = dynamic_cast<const CommercialProperty *>(prop))
    {
        typeCombo->setCurrentIndex(2);
        businessTypeEdit->setText(QString::fromStdString(comm->getBusinessType()));
        parkingCheck->setChecked(comm->getHasParking());
        parkingSpacesSpin->setValue(comm->getParkingSpaces());
        visibleFromStreetCheck->setChecked(comm->getIsVisibleFromStreet());
    }

    updateTypeSpecificFields();
}

void PropertyDialog::updateTypeSpecificFields() { propertyTypeChanged(typeCombo->currentIndex()); }

void PropertyDialog::validateAndAccept()
{
    QString idText = idEdit->text();
    if (idText.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "ID не может быть пустым");
        return;
    }
    if (!Property::validateId(idText.toStdString()))
    {
        QMessageBox::warning(this, "Ошибка валидации",
                             "ID должен содержать только цифры и быть длиной от 6 до 8 символов");
        return;
    }

    QString cityText = cityEdit->text();
    QString streetText = streetEdit->text();
    QString houseText = houseEdit->text();
    if (cityText.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Город не может быть пустым");
        return;
    }
    if (streetText.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Улица не может быть пустой");
        return;
    }
    if (houseText.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Дом не может быть пустым");
        return;
    }
    if (!Property::validateAddressPart(cityText.toStdString()))
    {
        QMessageBox::warning(this, "Ошибка валидации", "Неверный формат города");
        return;
    }
    if (!Property::validateAddressPart(streetText.toStdString()))
    {
        QMessageBox::warning(this, "Ошибка валидации", "Неверный формат улицы");
        return;
    }
    if (!Property::validateAddressPart(houseText.toStdString()))
    {
        QMessageBox::warning(this, "Ошибка валидации", "Неверный формат номера дома");
        return;
    }
    double price = priceSpin->value();
    if (price <= 0)
    {
        QMessageBox::warning(this, "Ошибка", "Цена должна быть положительной");
        return;
    }
    if (price < 10000)
    {
        QMessageBox::warning(this, "Ошибка", "Цена недвижимости не может быть меньше 10 000 рублей");
        return;
    }
    if (price > 1000000000)
    {
        QMessageBox::warning(this, "Ошибка", "Цена недвижимости не может превышать 1 000 000 000 рублей");
        return;
    }
    if (areaSpin->value() <= 0)
    {
        QMessageBox::warning(this, "Ошибка", "Площадь должна быть положительной");
        return;
    }

    if (int type = typeCombo->currentIndex(); type == 2 && businessTypeEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Тип бизнеса не может быть пустым");
        return;
    }

    accept();
}

PropertyDialog::PropertyType PropertyDialog::getPropertyType() const
{
    return static_cast<PropertyType>(typeCombo->currentIndex());
}

QString PropertyDialog::getId() const { return idEdit->text(); }
QString PropertyDialog::getCity() const { return cityEdit->text(); }
QString PropertyDialog::getStreet() const { return streetEdit->text(); }
QString PropertyDialog::getHouse() const { return houseEdit->text(); }
double PropertyDialog::getPrice() const { return priceSpin->value(); }
double PropertyDialog::getArea() const { return areaSpin->value(); }
QString PropertyDialog::getDescription() const { return descriptionEdit->toPlainText(); }
bool PropertyDialog::getIsAvailable() const { return availableCheck->isChecked(); }

int PropertyDialog::getRooms() const { return roomsSpin->value(); }
int PropertyDialog::getFloor() const { return floorSpin->value(); }
bool PropertyDialog::getHasBalcony() const { return balconyCheck->isChecked(); }
bool PropertyDialog::getHasElevator() const { return elevatorCheck->isChecked(); }

int PropertyDialog::getFloors() const { return floorsSpin->value(); }
double PropertyDialog::getLandArea() const { return landAreaSpin->value(); }
bool PropertyDialog::getHasGarage() const { return garageCheck->isChecked(); }
bool PropertyDialog::getHasGarden() const { return gardenCheck->isChecked(); }

QString PropertyDialog::getBusinessType() const { return businessTypeEdit->text(); }
int PropertyDialog::getParkingSpaces() const { return parkingSpacesSpin->value(); }
bool PropertyDialog::getIsVisibleFromStreet() const { return visibleFromStreetCheck->isChecked(); }
bool PropertyDialog::getHasParking() const { return parkingCheck->isChecked(); }
