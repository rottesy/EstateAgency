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
    void showAuctionDetails(Auction *auction);
    bool hasActiveTransactions(const std::string &propertyId);
    QWidget *createActionButtons(QTableWidget *table, const QString &id, std::function<void()> viewAction,
                                 std::function<void()> deleteAction, bool isView = false);
    void selectRowById(QTableWidget *table, const QString &id);
    QString getSelectedIdFromTable(QTableWidget *table);
    bool checkTableSelection(QTableWidget *table, const QString &errorMessage);

    EstateAgency *agency;
    QTableWidget *auctionsTable;
    QPushButton *addAuctionBtn;
    QPushButton *refreshAuctionBtn;
    QPushButton *searchAuctionBtn;
    QLineEdit *searchAuctionEdit;
    QTextEdit *auctionDetailsText;
};

#endif
