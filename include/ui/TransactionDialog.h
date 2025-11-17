#ifndef TRANSACTION_DIALOG_H
#define TRANSACTION_DIALOG_H

#include "../entities/Transaction.h"
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>

class TransactionDialog : public QDialog
{
    Q_OBJECT

  public:
    explicit TransactionDialog(QWidget *parent = nullptr, Transaction *editTransaction = nullptr,
                               const QStringList &propertyIds = QStringList(),
                               const QStringList &clientIds = QStringList());
    ~TransactionDialog();

    QString getId() const;
    QString getPropertyId() const;
    QString getClientId() const;
    double getFinalPrice() const;
    QString getStatus() const;
    QString getNotes() const;

  private slots:
    void validateAndAccept();
    void onPropertyChanged(int index);

  private:
    void setupUI();
    void loadTransactionData(Transaction *trans);
    void updatePriceFromProperty();
    void updatePriceDifference();

    QLineEdit *idEdit;
    QComboBox *propertyCombo;
    QComboBox *clientCombo;
    QDoubleSpinBox *priceSpin;
    QComboBox *statusCombo;
    QTextEdit *notesEdit;
    QDialogButtonBox *buttonBox;
    QLabel *propertyPriceLabel;
    QLabel *differenceLabel;
    QStringList propertyIds;
    QStringList clientIds;
    bool isUpdatingFromProperty = false;
};

#endif
