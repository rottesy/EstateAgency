#include "../../include/ui/ClientDialog.h"
#include "../../include/entities/Client.h"
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QVBoxLayout>

ClientDialog::ClientDialog(QWidget *parent, Client *editClient) : QDialog(parent)
{
    setStyleSheet(R"(
        QDialog {
            background-color: #2d2d2d;
        }
        QLabel {
            color: #d0d0d0;
        }
        QLineEdit {
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

    if (editClient)
    {
        loadClientData(editClient);
    }

    setWindowTitle(editClient ? "Редактировать клиента" : "Добавить клиента");
    setMinimumWidth(400);
}

ClientDialog::~ClientDialog() = default;

void ClientDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    auto *formLayout = new QFormLayout;

    idEdit = new QLineEdit;
    idEdit->setPlaceholderText("123456 (6-8 цифр)");
    idEdit->setMaxLength(8);
    idEdit->setToolTip("ID должен содержать только цифры\nДлина: от 6 до 8 символов");
    nameEdit = new QLineEdit;
    phoneEdit = new QLineEdit;
    phoneEdit->setPlaceholderText("+375XXXXXXXXX");
    emailEdit = new QLineEdit;
    emailEdit->setPlaceholderText("example@email.com");

    formLayout->addRow("ID:", idEdit);
    formLayout->addRow("Имя:", nameEdit);
    formLayout->addRow("Телефон:", phoneEdit);
    formLayout->addRow("Email:", emailEdit);

    mainLayout->addLayout(formLayout);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    mainLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &ClientDialog::validateAndAccept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void ClientDialog::loadClientData(Client *client)
{
    idEdit->setText(QString::fromStdString(client->getId()));
    nameEdit->setText(QString::fromStdString(client->getName()));
    phoneEdit->setText(QString::fromStdString(client->getPhone()));
    emailEdit->setText(QString::fromStdString(client->getEmail()));
}

void ClientDialog::validateAndAccept()
{
    QString idText = idEdit->text();
    if (idText.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "ID не может быть пустым");
        return;
    }
    if (!Client::validateId(idText.toStdString()))
    {
        QMessageBox::warning(this, "Ошибка валидации",
                             "ID должен содержать только цифры и быть длиной от 6 до 8 символов");
        return;
    }
    if (nameEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Имя не может быть пустым");
        return;
    }
    if (!Client::validatePhone(phoneEdit->text().toStdString()))
    {
        QMessageBox::warning(this, "Ошибка", "Неверный формат телефона (+375XXXXXXXXX)");
        return;
    }
    if (!Client::validateEmail(emailEdit->text().toStdString()))
    {
        QMessageBox::warning(this, "Ошибка", "Неверный формат email");
        return;
    }

    accept();
}

QString ClientDialog::getId() const { return idEdit->text(); }
QString ClientDialog::getName() const { return nameEdit->text(); }
QString ClientDialog::getPhone() const { return phoneEdit->text(); }
QString ClientDialog::getEmail() const { return emailEdit->text(); }
