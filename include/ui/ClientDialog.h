#ifndef CLIENT_DIALOG_H
#define CLIENT_DIALOG_H

#include "../entities/Client.h"
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>

class ClientDialog : public QDialog
{
    Q_OBJECT

  public:
    ClientDialog(QWidget *parent = nullptr, Client *editClient = nullptr);
    ~ClientDialog();

    QString getId() const;
    QString getName() const;
    QString getPhone() const;
    QString getEmail() const;

  private slots:
    void validateAndAccept();

  private:
    void setupUI();
    void loadClientData(Client *client);

    QLineEdit *idEdit;
    QLineEdit *nameEdit;
    QLineEdit *phoneEdit;
    QLineEdit *emailEdit;
    QDialogButtonBox *buttonBox;
};

#endif
