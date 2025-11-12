#ifndef PROPERTY_DIALOG_H
#define PROPERTY_DIALOG_H

#include "../include/Property.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QTextEdit>

class PropertyDialog : public QDialog
{
    Q_OBJECT

  public:
    enum PropertyType
    {
        TypeApartment,
        TypeHouse,
        TypeCommercial
    };

    PropertyDialog(QWidget *parent = nullptr, Property *editProperty = nullptr);
    ~PropertyDialog();

    PropertyType getPropertyType() const;

    QString getId() const;
    QString getCity() const;
    QString getStreet() const;
    QString getHouse() const;
    double getPrice() const;
    double getArea() const;
    QString getDescription() const;
    bool getIsAvailable() const;

    int getRooms() const;
    int getFloor() const;
    bool getHasBalcony() const;
    bool getHasElevator() const;

    int getFloors() const;
    double getLandArea() const;
    bool getHasGarage() const;
    bool getHasGarden() const;

    QString getBusinessType() const;
    int getParkingSpaces() const;
    bool getIsVisibleFromStreet() const;
    bool getHasParking() const;

  private slots:
    void propertyTypeChanged(int index);
    void validateAndAccept();

  private:
    void setupUI();
    void loadPropertyData(Property *prop);
    void updateTypeSpecificFields();

    QComboBox *typeCombo;
    QLineEdit *idEdit;
    QLineEdit *cityEdit;
    QLineEdit *streetEdit;
    QLineEdit *houseEdit;
    QDoubleSpinBox *priceSpin;
    QDoubleSpinBox *areaSpin;
    QTextEdit *descriptionEdit;
    QCheckBox *availableCheck;

    QGroupBox *apartmentGroup;
    QSpinBox *roomsSpin;
    QSpinBox *floorSpin;
    QCheckBox *balconyCheck;
    QCheckBox *elevatorCheck;

    QGroupBox *houseGroup;
    QSpinBox *floorsSpin;
    QSpinBox *houseRoomsSpin;
    QDoubleSpinBox *landAreaSpin;
    QCheckBox *garageCheck;
    QCheckBox *gardenCheck;

    QGroupBox *commercialGroup;
    QLineEdit *businessTypeEdit;
    QCheckBox *parkingCheck;
    QSpinBox *parkingSpacesSpin;
    QCheckBox *visibleFromStreetCheck;

    QDialogButtonBox *buttonBox;
};

#endif // PROPERTY_DIALOG_H
