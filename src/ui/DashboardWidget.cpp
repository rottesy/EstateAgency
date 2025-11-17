#include "../../include/ui/DashboardWidget.h"
#include "../../include/core/EstateAgency.h"
#include "../../include/entities/Property.h"
#include <QFrame>
#include <QHBoxLayout>

DashboardWidget::DashboardWidget(EstateAgency *agency, QWidget *parent) : QWidget(parent), agency(agency) { setupUI(); }

void DashboardWidget::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(50, 50, 50, 50);
    mainLayout->setSpacing(40);

    auto *header = new QLabel("Панель управления");
    header->setStyleSheet("color: #ffffff; "
                          "font-size: 48pt; "
                          "font-weight: 300; "
                          "letter-spacing: 3px; "
                          "margin-bottom: 10px;");
    mainLayout->addWidget(header);

    auto createMetricWidget = [](const QString &label, const QString &color, QLabel **valuePtr)
    {
        auto *widget = new QWidget;
        widget->setFixedHeight(160);

        auto *layout = new QVBoxLayout(widget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(12);

        auto *labelWidget = new QLabel(label);
        labelWidget->setStyleSheet(QString("color: %1; "
                                           "font-size: 11pt; "
                                           "font-weight: 500; "
                                           "letter-spacing: 2px;")
                                       .arg(color));
        layout->addWidget(labelWidget);

        auto *valueLabel = new QLabel("0");
        valueLabel->setStyleSheet(QString("color: %1; "
                                          "font-size: 50pt; "
                                          "font-weight: 700; "
                                          "background: transparent;")
                                      .arg(color));
        valueLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        valueLabel->setMinimumHeight(90);
        valueLabel->setMinimumWidth(120);
        layout->addWidget(valueLabel);
        layout->addStretch();

        if (valuePtr)
        {
            *valuePtr = valueLabel;
        }

        return widget;
    };

    QWidget *propMetric = createMetricWidget("Недвижимость", "#00D4AA", &statsPropertiesLabel);
    QWidget *clientMetric = createMetricWidget("Клиенты", "#5B9FFF", &statsClientsLabel);
    QWidget *transMetric = createMetricWidget("Сделки", "#FFB020", &statsTransactionsLabel);
    QWidget *availMetric = createMetricWidget("Доступно", "#B794F6", &statsAvailableLabel);
    QWidget *auctionMetric = createMetricWidget("Аукционы", "#FF6B6B", &statsAuctionsLabel);

    auto *metricsLayout = new QHBoxLayout;
    metricsLayout->setSpacing(60);
    metricsLayout->setContentsMargins(0, 0, 0, 0);

    metricsLayout->addWidget(propMetric);
    metricsLayout->addWidget(clientMetric);
    metricsLayout->addWidget(transMetric);
    metricsLayout->addWidget(availMetric);
    metricsLayout->addWidget(auctionMetric);
    metricsLayout->addStretch();

    mainLayout->addLayout(metricsLayout);

    auto *divider = new QFrame;
    divider->setFixedHeight(1);
    divider->setStyleSheet("background-color: #333333;");
    mainLayout->addWidget(divider);

    auto *actionsWidget = new QWidget;
    auto *actionsLayout = new QVBoxLayout(actionsWidget);
    actionsLayout->setContentsMargins(0, 0, 0, 0);
    actionsLayout->setSpacing(25);

    auto *actionsTitle = new QLabel("Действия");
    actionsTitle->setStyleSheet("color: #888888; "
                                "font-size: 11pt; "
                                "font-weight: 500; "
                                "letter-spacing: 2px;");
    actionsLayout->addWidget(actionsTitle);

    auto *buttonsLayout = new QHBoxLayout;
    buttonsLayout->setSpacing(20);
    buttonsLayout->setContentsMargins(0, 0, 0, 0);

    auto createModernButton = [](const QString &text, const QString &color)
    {
        auto *btn = new QPushButton(text);
        btn->setStyleSheet(QString("QPushButton {"
                                   "    background-color: transparent;"
                                   "    color: %1;"
                                   "    border: 1px solid %1;"
                                   "    border-radius: 0px;"
                                   "    padding: 14px 40px;"
                                   "    font-size: 12pt;"
                                   "    font-weight: 400;"
                                   "    letter-spacing: 1px;"
                                   "    min-width: 180px;"
                                   "}"
                                   "QPushButton:hover {"
                                   "    background-color: %1;"
                                   "    color: #1e1e1e;"
                                   "}"
                                   "QPushButton:pressed {"
                                   "    background-color: %2;"
                                   "}")
                               .arg(color, color));
        return btn;
    };

    saveBtn = createModernButton("Сохранить", "#00D4AA");
    loadBtn = createModernButton("Загрузить", "#5B9FFF");
    refreshBtn = createModernButton("Обновить", "#FFB020");

    connect(saveBtn, &QPushButton::clicked, this, &DashboardWidget::saveRequested);
    connect(loadBtn, &QPushButton::clicked, this, &DashboardWidget::loadRequested);
    connect(refreshBtn, &QPushButton::clicked, this, &DashboardWidget::refreshRequested);

    buttonsLayout->addWidget(saveBtn);
    buttonsLayout->addWidget(loadBtn);
    buttonsLayout->addWidget(refreshBtn);
    buttonsLayout->addStretch();

    actionsLayout->addLayout(buttonsLayout);
    mainLayout->addWidget(actionsWidget);
    mainLayout->addStretch();
}

void DashboardWidget::updateStats()
{
    if (!agency)
        return;

    int totalProperties = agency->getPropertyManager().getCount();
    int totalClients = agency->getClientManager().getCount();
    int totalTransactions = agency->getTransactionManager().getCount();
    int totalAuctions = agency->getAuctionManager().getCount();

    int availableProperties = 0;
    auto properties = agency->getPropertyManager().getAllProperties();
    for (const Property *prop : properties)
    {
        if (prop && prop->getIsAvailable())
        {
            availableProperties++;
        }
    }

    if (statsPropertiesLabel)
        statsPropertiesLabel->setText(QString::number(totalProperties));
    if (statsClientsLabel)
        statsClientsLabel->setText(QString::number(totalClients));
    if (statsTransactionsLabel)
        statsTransactionsLabel->setText(QString::number(totalTransactions));
    if (statsAvailableLabel)
        statsAvailableLabel->setText(QString::number(availableProperties));
    if (statsAuctionsLabel)
        statsAuctionsLabel->setText(QString::number(totalAuctions));
}
