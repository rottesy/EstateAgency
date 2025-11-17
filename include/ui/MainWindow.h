#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../core/EstateAgency.h"
#include "AuctionsWidget.h"
#include "ClientsWidget.h"
#include "DashboardWidget.h"
#include "PropertiesWidget.h"
#include "TransactionsWidget.h"
#include <QListWidget>
#include <QMainWindow>
#include <QStackedWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:
    void saveAllData();
    void loadAllData();
    void refreshAllData();
    void onNavigationChanged(int index);
    void onDataChanged();

  private:
    void setupUI();
    void setupMenuBar();
    void setupNewUI();
    void applyStyles();
    void updateDashboardStats();
    void showStatusMessage(const QString &message, int timeout = 3000) const;

    EstateAgency *agency;

    QListWidget *navigationList;
    QStackedWidget *contentStack;

    DashboardWidget *dashboardWidget;
    PropertiesWidget *propertiesWidget;
    ClientsWidget *clientsWidget;
    TransactionsWidget *transactionsWidget;
    AuctionsWidget *auctionsWidget;
};

#endif
