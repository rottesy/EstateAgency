#ifndef AUCTION_DIALOG_H
#define AUCTION_DIALOG_H

#include "../core/EstateAgency.h"
#include "../entities/Auction.h"
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTextEdit>
#include <QVBoxLayout>

class AuctionDialog : public QDialog
{
    Q_OBJECT

  public:
    explicit AuctionDialog(QWidget *parent = nullptr, Auction *editAuction = nullptr,
                           const QStringList &propertyIds = QStringList());
    ~AuctionDialog() override;

    QString getId() const;
    QString getPropertyId() const;
    double getStartingPrice() const;

    void refreshBids();
    void updateAuctionInfo();

  private slots:
    void validateAndAccept();
    void onPropertyChanged(int index);
    void addBid();
    void completeAuction();
    void refreshAuctionInfo();

  private:
    void setupUI();
    void loadAuctionData(Auction *auction);
    void updatePropertyInfo();
    void createTransactionFromAuction();

    bool isViewMode;
    Auction *currentAuction;
    EstateAgency *agency = EstateAgency::getInstance();

    QLineEdit *idEdit;
    QComboBox *propertyCombo;
    QDoubleSpinBox *priceSpin;
    QLabel *propertyPriceLabel;
    QLabel *buyoutPriceLabel;
    QLabel *currentHighestBidLabel;
    QLabel *statusLabel;

    QTableWidget *bidsTable;
    QPushButton *addBidBtn;
    QPushButton *completeAuctionBtn;
    QComboBox *clientCombo;
    QDoubleSpinBox *bidAmountSpin;

    QDialogButtonBox *buttonBox;
    QStringList propertyIds;
};

#endif
