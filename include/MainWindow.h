#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../include/AuctionsWidget.h"
#include "../include/ClientsWidget.h"
#include "../include/DashboardWidget.h"
#include "../include/EstateAgency.h"
#include "../include/PropertiesWidget.h"
#include "../include/TransactionsWidget.h"
#include <QListWidget>
#include <QMainWindow>
#include <QStackedWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
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
    void showStatusMessage(const QString &message, int timeout = 3000);

    EstateAgency *agency;

    QListWidget *navigationList;
    QStackedWidget *contentStack;

    DashboardWidget *dashboardWidget;
    PropertiesWidget *propertiesWidget;
    ClientsWidget *clientsWidget;
    TransactionsWidget *transactionsWidget;
    AuctionsWidget *auctionsWidget;
};

#endif // MAINWINDOW_H
