#ifndef CLIENTSWIDGET_H
#define CLIENTSWIDGET_H

#include "../core/EstateAgency.h"
#include "../entities/Client.h"
#include <QLineEdit>
#include <QPushButton>
#include <QSplitter>
#include <QTableWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

class ClientsWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit ClientsWidget(EstateAgency *agency, QWidget *parent = nullptr);
    void refresh();
    void updateTable();

  signals:
    void dataChanged();

  private slots:
    void addClient();
    void editClient();
    void deleteClient();
    void searchClients();
    void clientSelectionChanged();

  private:
    void setupUI();
    void showClientDetails(const Client *client);
    void showClientTransactions(const std::string &clientId);
    void addClientToTable(const Client *client);
    QWidget *createActionButtons(QTableWidget *table, const QString &id, const std::function<void()> &editAction,
                                 const std::function<void()> &deleteAction);
    void selectRowById(QTableWidget *table, const QString &id) const;
    QString getSelectedIdFromTable(const QTableWidget *table) const;
    bool checkTableSelection(const QTableWidget *table, const QString &errorMessage);

    EstateAgency *agency;
    QTableWidget *clientsTable;
    QPushButton *addClientBtn;
    QPushButton *refreshClientBtn;
    QPushButton *searchClientBtn;
    QLineEdit *searchClientEdit;
    QTextEdit *clientDetailsText;
};

#endif
