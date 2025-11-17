#ifndef DASHBOARDWIDGET_H
#define DASHBOARDWIDGET_H

#include "../core/EstateAgency.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class DashboardWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit DashboardWidget(EstateAgency *agency, QWidget *parent = nullptr);
    void updateStats();

  signals:
    void saveRequested();
    void loadRequested();
    void refreshRequested();

  private:
    void setupUI();

    EstateAgency *agency;
    QLabel *statsPropertiesLabel;
    QLabel *statsClientsLabel;
    QLabel *statsTransactionsLabel;
    QLabel *statsAvailableLabel;
    QLabel *statsAuctionsLabel;
    QPushButton *saveBtn;
    QPushButton *loadBtn;
    QPushButton *refreshBtn;
};

#endif
