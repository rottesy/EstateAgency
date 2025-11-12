#ifndef TRANSACTIONSWIDGET_H
#define TRANSACTIONSWIDGET_H

#include "../include/EstateAgency.h"
#include "../include/Transaction.h"
#include <QLineEdit>
#include <QPushButton>
#include <QSplitter>
#include <QTableWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

class TransactionsWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit TransactionsWidget(EstateAgency *agency, QWidget *parent = nullptr);
    void refresh();
    void updateTable();

  signals:
    void dataChanged();

  private slots:
    void addTransaction();
    void editTransaction();
    void deleteTransaction();
    void searchTransactions();
    void transactionSelectionChanged();

  private:
    void setupUI();
    void showTransactionDetails(Transaction *trans);
    bool validateTransaction(const std::string &propertyId, const std::string &clientId, const std::string &status,
                             const std::string &excludeTransactionId = "");
    bool hasActiveTransactions(const std::string &propertyId);
    QWidget *createActionButtons(QTableWidget *table, const QString &id, std::function<void()> editAction,
                                 std::function<void()> deleteAction);
    void selectRowById(QTableWidget *table, const QString &id);
    QString getSelectedIdFromTable(QTableWidget *table);
    bool checkTableSelection(QTableWidget *table, const QString &errorMessage);

    EstateAgency *agency;
    QTableWidget *transactionsTable;
    QPushButton *addTransactionBtn;
    QPushButton *refreshTransactionBtn;
    QPushButton *searchTransactionBtn;
    QLineEdit *searchTransactionEdit;
    QTextEdit *transactionDetailsText;
};

#endif // TRANSACTIONSWIDGET_H

