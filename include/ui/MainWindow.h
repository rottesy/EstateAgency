#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../core/EstateAgency.h"
#include "../services/AuctionManager.h"
#include "../services/ClientManager.h"
#include "../services/FileManager.h"
#include "../services/PropertyManager.h"
#include "../services/TransactionManager.h"
#include "AuctionsWidget.h"
#include "ClientsWidget.h"
#include "DashboardWidget.h"
#include "PropertiesWidget.h"
#include "TransactionsWidget.h"
#include <QListWidget>
#include <QMainWindow>
#include <QStackedWidget>
#include <filesystem>
#include <stdexcept>

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

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
    void handleException(const std::exception &e);

    template <typename Func> void executeWithExceptionHandling(Func &&operation)
    {
        try
        {
            operation();
        }
        catch (const FileManagerException &e)
        {
            handleException(e);
        }
        catch (const std::filesystem::filesystem_error &e)
        {
            handleException(e);
        }
        catch (const PropertyManagerException &e)
        {
            handleException(e);
        }
        catch (const ClientManagerException &e)
        {
            handleException(e);
        }
        catch (const TransactionManagerException &e)
        {
            handleException(e);
        }
        catch (const AuctionManagerException &e)
        {
            handleException(e);
        }
        catch (const std::bad_alloc &e)
        {
            handleException(e);
        }
        catch (const std::invalid_argument &e)
        {
            handleException(e);
        }
        catch (const std::domain_error &e)
        {
            handleException(e);
        }
        catch (const std::length_error &e)
        {
            handleException(e);
        }
        catch (const std::out_of_range &e)
        {
            handleException(e);
        }
    }

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
