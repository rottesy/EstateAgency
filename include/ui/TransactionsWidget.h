#ifndef TRANSACTIONSWIDGET_H
#define TRANSACTIONSWIDGET_H

#include "../core/EstateAgency.h"
#include "../entities/Transaction.h"
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
    void showTransactionDetails(const Transaction *trans);
    bool validateTransaction(std::string_view propertyId, std::string_view clientId, std::string_view status,
                             std::string_view excludeId = "");
    bool hasActiveTransactions(const std::string &propertyId);
    QWidget *createActionButtons(QTableWidget *table, const QString &id, const std::function<void()> &editAction,
                                 const std::function<void()> &deleteAction);
    void selectRowById(QTableWidget *table, const QString &id) const;
    QString getSelectedIdFromTable(const QTableWidget *table) const;
    bool checkTableSelection(const QTableWidget *table, const QString &errorMessage);

    EstateAgency *agency;
    QTableWidget *transactionsTable;
    QPushButton *addTransactionBtn;
    QPushButton *refreshTransactionBtn;
    QPushButton *searchTransactionBtn;
    QLineEdit *searchTransactionEdit;
    QTextEdit *transactionDetailsText;
};

#endif
