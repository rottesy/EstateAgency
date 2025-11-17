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

PropertyDialog::PropertyDialog(QWidget *parent, const Property *editProperty) : QDialog(parent)
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
    common.typeCombo = new QComboBox;
    common.typeCombo->addItems({"Квартира", "Дом", "Коммерческая"});
    typeLayout->addWidget(common.typeCombo);
    mainLayout->addLayout(typeLayout);

    auto *commonLayout = new QFormLayout;
    common.idEdit = new QLineEdit;
    common.idEdit->setPlaceholderText("123456 (6-8 цифр)");
    common.idEdit->setMaxLength(8);
    common.idEdit->setToolTip("ID должен содержать только цифры\nДлина: от 6 до 8 символов");
    common.cityEdit = new QLineEdit;
    common.cityEdit->setPlaceholderText("г. Минск");
    common.cityEdit->setToolTip("Название города");
    common.streetEdit = new QLineEdit;
    common.streetEdit->setPlaceholderText("ул. Ленина");
    common.streetEdit->setToolTip("Название улицы");
    common.houseEdit = new QLineEdit;
    common.houseEdit->setPlaceholderText("д. 10");
    common.houseEdit->setToolTip("Номер дома");
    common.priceSpin = new QDoubleSpinBox;
    common.priceSpin->setMinimum(10000);
    common.priceSpin->setMaximum(1000000000);
    common.priceSpin->setValue(10000);
    common.priceSpin->setSuffix(" руб.");
    common.priceSpin->setToolTip("Минимальная цена: 10 000 руб.\nМаксимальная цена: 1 000 000 000 руб.");
    common.areaSpin = new QDoubleSpinBox;
    common.areaSpin->setMaximum(100000);
    common.areaSpin->setSuffix(" м²");
    common.descriptionEdit = new QTextEdit;
    common.descriptionEdit->setMaximumHeight(100);
    common.availableCheck = new QCheckBox("Доступна");
    common.availableCheck->setChecked(true);

    commonLayout->addRow("ID:", common.idEdit);
    commonLayout->addRow("Город:", common.cityEdit);
    commonLayout->addRow("Улица:", common.streetEdit);
    commonLayout->addRow("Дом:", common.houseEdit);
    commonLayout->addRow("Цена:", common.priceSpin);
    commonLayout->addRow("Площадь:", common.areaSpin);
    commonLayout->addRow("Описание:", common.descriptionEdit);
    commonLayout->addRow(common.availableCheck);

    auto *commonGroup = new QGroupBox("Общая информация");
    commonGroup->setLayout(commonLayout);
    mainLayout->addWidget(commonGroup);

    apartment.group = new QGroupBox("Параметры квартиры");
    auto *aptLayout = new QFormLayout;
    apartment.roomsSpin = new QSpinBox;
    apartment.roomsSpin->setRange(1, 10);
    apartment.floorSpin = new QSpinBox;
    apartment.floorSpin->setRange(1, 100);
    apartment.balconyCheck = new QCheckBox;
    apartment.elevatorCheck = new QCheckBox;

    aptLayout->addRow("Комнат:", apartment.roomsSpin);
    aptLayout->addRow("Этаж:", apartment.floorSpin);
    aptLayout->addRow("Балкон:", apartment.balconyCheck);
    aptLayout->addRow("Лифт:", apartment.elevatorCheck);
    apartment.group->setLayout(aptLayout);
    mainLayout->addWidget(apartment.group);

    house.group = new QGroupBox("Параметры дома");
    auto *houseLayout = new QFormLayout;
    house.floorsSpin = new QSpinBox;
    house.floorsSpin->setRange(1, 10);
    house.roomsSpin = new QSpinBox;
    house.roomsSpin->setRange(1, 50);
    house.landAreaSpin = new QDoubleSpinBox;
    house.landAreaSpin->setMaximum(10000);
    house.landAreaSpin->setSuffix(" м²");
    house.garageCheck = new QCheckBox;
    house.gardenCheck = new QCheckBox;

    houseLayout->addRow("Этажей:", house.floorsSpin);
    houseLayout->addRow("Комнат:", house.roomsSpin);
    houseLayout->addRow("Участок:", house.landAreaSpin);
    houseLayout->addRow("Гараж:", house.garageCheck);
    houseLayout->addRow("Сад:", house.gardenCheck);
    house.group->setLayout(houseLayout);
    house.group->setVisible(false);
    mainLayout->addWidget(house.group);

    commercial.group = new QGroupBox("Параметры коммерческой");
    auto *commLayout = new QFormLayout;
    commercial.businessTypeEdit = new QLineEdit;
    commercial.parkingCheck = new QCheckBox;
    commercial.parkingSpacesSpin = new QSpinBox;
    commercial.parkingSpacesSpin->setRange(0, 1000);
    commercial.visibleFromStreetCheck = new QCheckBox;

    commLayout->addRow("Тип бизнеса:", commercial.businessTypeEdit);
    commLayout->addRow("Парковка:", commercial.parkingCheck);
    commLayout->addRow("Мест парковки:", commercial.parkingSpacesSpin);
    commLayout->addRow("Видимость с улицы:", commercial.visibleFromStreetCheck);
    commercial.group->setLayout(commLayout);
    commercial.group->setVisible(false);
    mainLayout->addWidget(commercial.group);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    mainLayout->addWidget(buttonBox);

    connect(common.typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &PropertyDialog::propertyTypeChanged);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &PropertyDialog::validateAndAccept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void PropertyDialog::propertyTypeChanged(int index)
{
    apartment.group->setVisible(index == 0);
    house.group->setVisible(index == 1);
    commercial.group->setVisible(index == 2);
}

void PropertyDialog::loadPropertyData(const Property *prop)
{
    common.idEdit->setText(QString::fromStdString(prop->getId()));
    common.cityEdit->setText(QString::fromStdString(prop->getCity()));
    common.streetEdit->setText(QString::fromStdString(prop->getStreet()));
    common.houseEdit->setText(QString::fromStdString(prop->getHouse()));
    common.priceSpin->setValue(prop->getPrice());
    common.areaSpin->setValue(prop->getArea());
    common.descriptionEdit->setPlainText(QString::fromStdString(prop->getDescription()));
    common.availableCheck->setChecked(prop->getIsAvailable());

    if (const auto *apt = dynamic_cast<const Apartment *>(prop))
    {
        common.typeCombo->setCurrentIndex(0);
        apartment.roomsSpin->setValue(apt->getRooms());
        apartment.floorSpin->setValue(apt->getFloor());
        apartment.balconyCheck->setChecked(apt->getHasBalcony());
        apartment.elevatorCheck->setChecked(apt->getHasElevator());
    }
    else if (const auto *houseObj = dynamic_cast<const House *>(prop))
    {
        common.typeCombo->setCurrentIndex(1);
        house.floorsSpin->setValue(houseObj->getFloors());
        house.roomsSpin->setValue(houseObj->getRooms());
        house.landAreaSpin->setValue(houseObj->getLandArea());
        house.garageCheck->setChecked(houseObj->getHasGarage());
        house.gardenCheck->setChecked(houseObj->getHasGarden());
    }
    else if (const auto *comm = dynamic_cast<const CommercialProperty *>(prop))
    {
        common.typeCombo->setCurrentIndex(2);
        commercial.businessTypeEdit->setText(QString::fromStdString(comm->getBusinessType()));
        commercial.parkingCheck->setChecked(comm->getHasParking());
        commercial.parkingSpacesSpin->setValue(comm->getParkingSpaces());
        commercial.visibleFromStreetCheck->setChecked(comm->getIsVisibleFromStreet());
    }

    updateTypeSpecificFields();
}

void PropertyDialog::updateTypeSpecificFields() { propertyTypeChanged(common.typeCombo->currentIndex()); }

void PropertyDialog::validateAndAccept()
{
    QString idText = common.idEdit->text();
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

    QString cityText = common.cityEdit->text();
    QString streetText = common.streetEdit->text();
    QString houseText = common.houseEdit->text();
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
    double price = common.priceSpin->value();
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
    if (common.areaSpin->value() <= 0)
    {
        QMessageBox::warning(this, "Ошибка", "Площадь должна быть положительной");
        return;
    }

    if (int type = common.typeCombo->currentIndex(); type == 2 && commercial.businessTypeEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Тип бизнеса не может быть пустым");
        return;
    }

    accept();
}

PropertyDialog::PropertyType PropertyDialog::getPropertyType() const
{
    return static_cast<PropertyType>(common.typeCombo->currentIndex());
}

QString PropertyDialog::getId() const { return common.idEdit->text(); }
QString PropertyDialog::getCity() const { return common.cityEdit->text(); }
QString PropertyDialog::getStreet() const { return common.streetEdit->text(); }
QString PropertyDialog::getHouse() const { return common.houseEdit->text(); }
double PropertyDialog::getPrice() const { return common.priceSpin->value(); }
double PropertyDialog::getArea() const { return common.areaSpin->value(); }
QString PropertyDialog::getDescription() const { return common.descriptionEdit->toPlainText(); }
bool PropertyDialog::getIsAvailable() const { return common.availableCheck->isChecked(); }

int PropertyDialog::getRooms() const { return apartment.roomsSpin->value(); }
int PropertyDialog::getFloor() const { return apartment.floorSpin->value(); }
bool PropertyDialog::getHasBalcony() const { return apartment.balconyCheck->isChecked(); }
bool PropertyDialog::getHasElevator() const { return apartment.elevatorCheck->isChecked(); }

int PropertyDialog::getFloors() const { return house.floorsSpin->value(); }
double PropertyDialog::getLandArea() const { return house.landAreaSpin->value(); }
bool PropertyDialog::getHasGarage() const { return house.garageCheck->isChecked(); }
bool PropertyDialog::getHasGarden() const { return house.gardenCheck->isChecked(); }

QString PropertyDialog::getBusinessType() const { return commercial.businessTypeEdit->text(); }
int PropertyDialog::getParkingSpaces() const { return commercial.parkingSpacesSpin->value(); }
bool PropertyDialog::getIsVisibleFromStreet() const { return commercial.visibleFromStreetCheck->isChecked(); }
bool PropertyDialog::getHasParking() const { return commercial.parkingCheck->isChecked(); }
