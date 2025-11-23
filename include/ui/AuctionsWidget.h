#ifndef AUCTIONSWIDGET_H
#define AUCTIONSWIDGET_H

#include "../core/EstateAgency.h"
#include "../entities/Auction.h"
#include <QLineEdit>
#include <QPushButton>
#include <QSplitter>
#include <QTableWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

class AuctionsWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit AuctionsWidget(EstateAgency *agency, QWidget *parent = nullptr);
    void refresh();
    void updateTable();

  signals:
    void dataChanged();

  private slots:
    void addAuction();
    void viewAuction();
    void deleteAuction();
    void searchAuctions();
    void auctionSelectionChanged();

  private:
    void setupUI();
    void showAuctionDetails(const Auction *auction);
    void addAuctionToTable(const Auction *auction);
    bool hasActiveTransactions(const std::string &propertyId);
    QWidget *createActionButtons(QTableWidget *table, const QString &id, const std::function<void()> &viewAction,
                                 const std::function<void()> &deleteAction, [[maybe_unused]] bool isView = false);
    void selectRowById(QTableWidget *table, const QString &id) const;
    QString getSelectedIdFromTable(const QTableWidget *table) const;
    bool checkTableSelection(const QTableWidget *table, const QString &errorMessage) const;

    EstateAgency *agency;
    QTableWidget *auctionsTable;
    QPushButton *addAuctionBtn;
    QPushButton *refreshAuctionBtn;
    QPushButton *searchAuctionBtn;
    QLineEdit *searchAuctionEdit;
    QTextEdit *auctionDetailsText;
};

#endif
