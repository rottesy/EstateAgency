#include "../../include/ui/TransactionDialog.h"
#include "../../include/core/EstateAgency.h"
#include "../../include/entities/Transaction.h"
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <cmath>

TransactionDialog::TransactionDialog(QWidget *parent, Transaction *editTransaction, const QStringList &propertyIds,
                                     const QStringList &clientIds)
    : QDialog(parent), propertyIds(propertyIds), clientIds(clientIds), isUpdatingFromProperty(false)
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
        QDoubleSpinBox {
            background-color: #2d2d2d;
            color: #d0d0d0;
            border: 1px solid #404040;
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

    propertyCombo->addItems(propertyIds);
    clientCombo->addItems(clientIds);

    if (editTransaction)
    {
        loadTransactionData(editTransaction);
    }

    setWindowTitle(editTransaction ? "Редактировать сделку" : "Добавить сделку");
    setMinimumWidth(400);
}

TransactionDialog::~TransactionDialog() = default;

void TransactionDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    auto *formLayout = new QFormLayout;

    idEdit = new QLineEdit;
    idEdit->setPlaceholderText("123456 (6-8 цифр)");
    idEdit->setMaxLength(8);
    idEdit->setToolTip("ID должен содержать только цифры\nДлина: от 6 до 8 символов");

    propertyCombo = new QComboBox;
    propertyCombo->setToolTip("Выберите недвижимость. Цена сделки будет "
                              "автоматически подставлена из цены недвижимости.");
    connect(propertyCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &TransactionDialog::onPropertyChanged);

    clientCombo = new QComboBox;

    auto *priceWidget = new QWidget;
    auto *priceLayout = new QVBoxLayout(priceWidget);
    priceLayout->setContentsMargins(0, 0, 0, 0);
    priceLayout->setSpacing(5);

    auto *priceInputLayout = new QHBoxLayout;
    priceSpin = new QDoubleSpinBox;
    priceSpin->setMinimum(10000);
    priceSpin->setMaximum(1000000000);
    priceSpin->setValue(10000);
    priceSpin->setSuffix(" руб.");
    priceSpin->setDecimals(2);
    priceSpin->setSingleStep(10000);
    priceSpin->setToolTip("Цена сделки (может отличаться от цены недвижимости)\nМинимум: 10 000 "
                          "руб.\nМаксимум: 1 000 000 000 руб.\n💡 Подсказка: при выборе "
                          "недвижимости цена автоматически подставится");
    priceInputLayout->addWidget(priceSpin);
    priceInputLayout->addStretch();
    priceLayout->addLayout(priceInputLayout);

    propertyPriceLabel = new QLabel("Цена недвижимости: не выбрана");
    propertyPriceLabel->setStyleSheet("color: #8fa8b3; font-size: 9pt; padding-left: 5px;");
    propertyPriceLabel->setToolTip("Цена выбранной недвижимости (информация)");
    priceLayout->addWidget(propertyPriceLabel);

    differenceLabel = new QLabel("");
    differenceLabel->setStyleSheet("color: #6a9; font-size: 9pt; padding-left: 5px;");
    differenceLabel->setToolTip("Разница между ценой сделки и ценой недвижимости");
    priceLayout->addWidget(differenceLabel);

    statusCombo = new QComboBox;
    statusCombo->addItems({"pending", "completed", "cancelled"});
    statusCombo->setToolTip("pending - в ожидании\ncompleted - завершена\ncancelled - отменена");

    notesEdit = new QTextEdit;
    notesEdit->setMaximumHeight(100);
    notesEdit->setPlaceholderText("Дополнительные примечания к сделке...");

    formLayout->addRow("ID сделки:", idEdit);
    formLayout->addRow("Недвижимость:", propertyCombo);
    formLayout->addRow("Клиент:", clientCombo);
    formLayout->addRow("Цена сделки:", priceWidget);
    formLayout->addRow("Статус:", statusCombo);
    formLayout->addRow("Примечания:", notesEdit);

    connect(priceSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            [this]()
            {
                if (!isUpdatingFromProperty)
                {
                    updatePriceDifference();
                }
            });

    mainLayout->addLayout(formLayout);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    mainLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &TransactionDialog::validateAndAccept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void TransactionDialog::loadTransactionData(Transaction *trans)
{
    idEdit->setText(QString::fromStdString(trans->getId()));
    QString propertyId = QString::fromStdString(trans->getPropertyId());
    int propIndex = -1;
    for (int i = 0; i < propertyIds.size(); ++i)
    {
        if (propertyIds[i].startsWith(propertyId + " - "))
        {
            propIndex = i;
            break;
        }
    }
    if (propIndex >= 0)
        propertyCombo->setCurrentIndex(propIndex);

    QString clientId = QString::fromStdString(trans->getClientId());
    int clientIndex = -1;
    for (int i = 0; i < clientIds.size(); ++i)
    {
        if (clientIds[i].startsWith(clientId + " - "))
        {
            clientIndex = i;
            break;
        }
    }
    if (clientIndex >= 0)
        clientCombo->setCurrentIndex(clientIndex);
    priceSpin->setValue(trans->getFinalPrice());

    if (trans->getStatus() == "pending")
        statusCombo->setCurrentIndex(0);
    else if (trans->getStatus() == "completed")
        statusCombo->setCurrentIndex(1);
    else
        statusCombo->setCurrentIndex(2);

    notesEdit->setPlainText(QString::fromStdString(trans->getNotes()));
}

void TransactionDialog::validateAndAccept()
{
    QString idText = idEdit->text();
    if (idText.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "ID не может быть пустым");
        return;
    }
    if (!Transaction::validateId(idText.toStdString()))
    {
        QMessageBox::warning(this, "Ошибка валидации",
                             "ID должен содержать только цифры и быть длиной от 6 до 8 символов");
        return;
    }
    if (propertyCombo->currentIndex() < 0)
    {
        QMessageBox::warning(this, "Ошибка", "Выберите недвижимость");
        return;
    }
    if (clientCombo->currentIndex() < 0)
    {
        QMessageBox::warning(this, "Ошибка", "Выберите клиента");
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
        QMessageBox::warning(this, "Ошибка", "Цена сделки не может быть меньше 10 000 рублей");
        return;
    }
    if (price > 1000000000)
    {
        QMessageBox::warning(this, "Ошибка", "Цена сделки не может превышать 1 000 000 000 рублей");
        return;
    }

    if (propertyCombo->currentIndex() >= 0)
    {
    }

    accept();
}

void TransactionDialog::onPropertyChanged(int index)
{
    if (index >= 0 && index < propertyIds.size())
    {
        updatePriceFromProperty();
    }
    else
    {
        propertyPriceLabel->setText("Цена недвижимости: не выбрана");
        differenceLabel->setText("");
    }
}

void TransactionDialog::updatePriceFromProperty()
{
    if (propertyCombo->currentIndex() < 0 || propertyCombo->currentIndex() >= propertyIds.size())
    {
        return;
    }

    QString propIdText = propertyIds[propertyCombo->currentIndex()];
    int separatorIndex = propIdText.indexOf(" - ");
    QString propId = (separatorIndex > 0) ? propIdText.left(separatorIndex) : propIdText;

    EstateAgency *agency = EstateAgency::getInstance();
    Property *prop = agency->getPropertyManager().findProperty(propId.toStdString());

    if (prop)
    {
        double propertyPrice = prop->getPrice();
        isUpdatingFromProperty = true;
        priceSpin->setValue(propertyPrice);
        isUpdatingFromProperty = false;

        QString priceText = QString("Цена недвижимости: %1 руб.").arg(QString::number(propertyPrice, 'f', 2));
        propertyPriceLabel->setText(priceText);

        updatePriceDifference();
    }
}

void TransactionDialog::updatePriceDifference()
{
    if (propertyCombo->currentIndex() < 0 || propertyCombo->currentIndex() >= propertyIds.size())
    {
        differenceLabel->setText("");
        return;
    }

    QString propIdText = propertyIds[propertyCombo->currentIndex()];
    int separatorIndex = propIdText.indexOf(" - ");
    QString propId = (separatorIndex > 0) ? propIdText.left(separatorIndex) : propIdText;

    EstateAgency *agency = EstateAgency::getInstance();
    Property *prop = agency->getPropertyManager().findProperty(propId.toStdString());

    if (prop)
    {
        double propertyPrice = prop->getPrice();
        double transactionPrice = priceSpin->value();
        double difference = transactionPrice - propertyPrice;
        double percentDiff = (difference / propertyPrice) * 100.0;

        QString diffText;
        if (std::abs(difference) < 0.01)
        {
            diffText = "Цена сделки равна цене недвижимости";
            differenceLabel->setStyleSheet("color: #6a9; font-size: 9pt; padding-left: 5px;");
        }
        else if (difference > 0)
        {
            diffText = QString("Переплата: +%1 руб. (+%2%)")
                           .arg(QString::number(difference, 'f', 2), QString::number(percentDiff, 'f', 1));
            differenceLabel->setStyleSheet("color: #a66; font-size: 9pt; padding-left: 5px;");
        }
        else
        {
            diffText = QString("Скидка: %1 руб. (%2%)")
                           .arg(QString::number(difference, 'f', 2), QString::number(percentDiff, 'f', 1));
            differenceLabel->setStyleSheet("color: #6a9; font-size: 9pt; padding-left: 5px;");
        }
        differenceLabel->setText(diffText);
    }
}

QString TransactionDialog::getId() const { return idEdit->text(); }
QString TransactionDialog::getPropertyId() const
{
    QString text = propertyCombo->currentText();
    int separatorIndex = text.indexOf(" - ");
    if (separatorIndex > 0)
    {
        return text.left(separatorIndex);
    }
    return text;
}
QString TransactionDialog::getClientId() const
{
    QString text = clientCombo->currentText();
    int separatorIndex = text.indexOf(" - ");
    if (separatorIndex > 0)
    {
        return text.left(separatorIndex);
    }
    return text;
}
double TransactionDialog::getFinalPrice() const { return priceSpin->value(); }
QString TransactionDialog::getStatus() const { return statusCombo->currentText(); }
QString TransactionDialog::getNotes() const { return notesEdit->toPlainText(); }
