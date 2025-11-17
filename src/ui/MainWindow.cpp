#include "../../include/ui/MainWindow.h"
#include "../../include/core/Constants.h"
#include "../../include/services/AuctionManager.h"
#include "../../include/services/ClientManager.h"
#include "../../include/services/FileManager.h"
#include "../../include/services/PropertyManager.h"
#include "../../include/services/TransactionManager.h"
#include <QFrame>
#include <QLabel>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QStatusBar>
#include <QTimer>
#include <filesystem>
#include <stdexcept>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    try
    {
        agency = EstateAgency::getInstance();
        agency->loadAllData();

        applyStyles();
        setupUI();
        setupMenuBar();

        setWindowTitle("Информационная система продажи недвижимости");
        setMinimumSize(1200, 700);
        resize(1400, 800);

        QTimer::singleShot(100, this,
                           [this]()
                           {
                               if (propertiesWidget)
                                   propertiesWidget->refresh();
                               if (clientsWidget)
                                   clientsWidget->refresh();
                               if (transactionsWidget)
                                   transactionsWidget->refresh();
                               if (auctionsWidget)
                                   auctionsWidget->refresh();
                               updateDashboardStats();
                           });
    }
    catch (const FileManagerException &e)
    {
        QMessageBox::critical(nullptr, "Ошибка инициализации", QString("Ошибка файлового менеджера: %1").arg(e.what()));
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        QMessageBox::critical(nullptr, "Ошибка инициализации", QString("Ошибка файловой системы: %1").arg(e.what()));
    }
    catch (const PropertyManagerException &e)
    {
        QMessageBox::critical(nullptr, "Ошибка инициализации",
                              QString("Ошибка менеджера недвижимости: %1").arg(e.what()));
    }
    catch (const ClientManagerException &e)
    {
        QMessageBox::critical(nullptr, "Ошибка инициализации", QString("Ошибка менеджера клиентов: %1").arg(e.what()));
    }
    catch (const TransactionManagerException &e)
    {
        QMessageBox::critical(nullptr, "Ошибка инициализации", QString("Ошибка менеджера сделок: %1").arg(e.what()));
    }
    catch (const AuctionManagerException &e)
    {
        QMessageBox::critical(nullptr, "Ошибка инициализации", QString("Ошибка менеджера аукционов: %1").arg(e.what()));
    }
    catch (const std::runtime_error &e)
    {
        QMessageBox::critical(nullptr, "Ошибка инициализации", QString("Ошибка выполнения: %1").arg(e.what()));
    }
    catch (const std::bad_alloc &e)
    {
        QMessageBox::critical(nullptr, "Ошибка инициализации", QString("Недостаточно памяти: %1").arg(e.what()));
    }
}

MainWindow::~MainWindow() { EstateAgency::destroyInstance(); }

void MainWindow::applyStyles()
{
    setStyleSheet(R"(
        QMainWindow {
            background-color: #1e1e1e;
        }
        
        QTabWidget::pane {
            border: 1px solid #3d3d3d;
            background-color: #2d2d2d;
            top: -1px;
        }
        
        QTabBar::tab {
            background-color: #2d2d2d;
            color: #d0d0d0;
            padding: 8px 20px;
            margin-right: 2px;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
            font-weight: 500;
            font-size: 11pt;
            min-width: 120px;
        }
        
        QTabBar::tab:selected {
            background-color: #3d3d3d;
            color: #ffffff;
            border-bottom: 2px solid #5a9;
        }
        
        QTabBar::tab:hover {
            background-color: #353535;
        }
        
        QPushButton {
            background-color: #404040;
            color: white;
            border: none;
            padding: 6px 14px;
            border-radius: 3px;
            font-weight: 500;
            font-size: 9pt;
            min-width: 80px;
        }
        
        QPushButton:hover {
            background-color: #505050;
        }
        
        QPushButton:pressed {
            background-color: #303030;
        }
        
        QPushButton[text="Добавить"] {
            background-color: #2d5a3d;
        }
        
        QPushButton[text="Добавить"]:hover {
            background-color: #3d6a4d;
        }
        
        QPushButton[text="Удалить"] {
            background-color: #5a2d2d;
        }
        
        QPushButton[text="Удалить"]:hover {
            background-color: #6a3d3d;
        }
        
        QPushButton[text="Редактировать"] {
            background-color: #2d3d5a;
        }
        
        QPushButton[text="Редактировать"]:hover {
            background-color: #3d4d6a;
        }
        
        QPushButton[text="Обновить"], QPushButton[text="Найти"] {
            background-color: #4a4a4a;
        }
        
        QPushButton[text="Обновить"]:hover, QPushButton[text="Найти"]:hover {
            background-color: #5a5a5a;
        }
        
        QTableWidget {
            background-color: #2d2d2d;
            alternate-background-color: #252525;
            border: 1px solid #3d3d3d;
            gridline-color: #404040;
            color: #d0d0d0;
        }
        
        QTableWidget::item {
            padding: 6px;
            color: #d0d0d0;
        }
        
        QTableWidget::item:selected {
            background-color: #5a9;
            color: white;
        }
        
        QHeaderView::section {
            background-color: #3d3d3d;
            color: #ffffff;
            padding: 8px;
            border: none;
            font-weight: 600;
            font-size: 9pt;
        }
        
        QLineEdit, QTextEdit {
            border: 1px solid #404040;
            border-radius: 3px;
            padding: 6px;
            background-color: #2d2d2d;
            color: #d0d0d0;
            font-size: 10pt;
        }
        
        QLineEdit:focus, QTextEdit:focus {
            border: 1px solid #5a9;
            outline: none;
        }
        
        QLabel {
            color: #d0d0d0;
            font-size: 10pt;
        }
        
        QGroupBox {
            border: 1px solid #404040;
            border-radius: 4px;
            margin-top: 12px;
            font-weight: 600;
            font-size: 10pt;
            color: #d0d0d0;
            background-color: #2d2d2d;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 2px 8px;
            background-color: #3d3d3d;
            color: #ffffff;
            border-radius: 2px;
        }
        
        QComboBox {
            border: 1px solid #404040;
            border-radius: 3px;
            padding: 6px;
            background-color: #2d2d2d;
            color: #d0d0d0;
            font-size: 10pt;
        }
        
        QComboBox:focus {
            border: 1px solid #5a9;
        }
        
        QComboBox::drop-down {
            border: none;
        }
        
        QComboBox QAbstractItemView {
            background-color: #2d2d2d;
            color: #d0d0d0;
            selection-background-color: #5a9;
            selection-color: white;
            border: 1px solid #404040;
        }
        
        QSpinBox, QDoubleSpinBox {
            border: 1px solid #404040;
            border-radius: 3px;
            padding: 6px;
            background-color: #2d2d2d;
            color: #d0d0d0;
            font-size: 10pt;
        }
        
        QSpinBox:focus, QDoubleSpinBox:focus {
            border: 1px solid #5a9;
        }
        
        QCheckBox {
            color: #d0d0d0;
            font-size: 10pt;
        }
        
        QCheckBox::indicator {
            width: 18px;
            height: 18px;
            border: 1px solid #404040;
            border-radius: 3px;
            background-color: #2d2d2d;
        }
        
        QCheckBox::indicator:checked {
            background-color: #5a9;
            border-color: #5a9;
        }
        
        QDialog {
            background-color: #2d2d2d;
        }
        
        QDialog QLabel {
            color: #d0d0d0;
        }
        
        QDialog QLineEdit, QDialog QTextEdit, QDialog QComboBox,
        QDialog QSpinBox, QDialog QDoubleSpinBox {
            background-color: #2d2d2d;
            color: #d0d0d0;
            border: 1px solid #404040;
        }
        
        QDialog QGroupBox {
            background-color: #2d2d2d;
            color: #d0d0d0;
        }
        
        QMenuBar {
            background-color: #252525;
            color: #d0d0d0;
            border-bottom: 1px solid #3d3d3d;
        }
        
        QMenuBar::item {
            padding: 6px 12px;
        }
        
        QMenuBar::item:selected {
            background-color: #3d3d3d;
        }
        
        QMenu {
            background-color: #2d2d2d;
            color: #d0d0d0;
            border: 1px solid #404040;
        }
        
        QMenu::item:selected {
            background-color: #5a9;
            color: white;
        }
        
        QStatusBar {
            background-color: #252525;
            color: #d0d0d0;
            border-top: 1px solid #3d3d3d;
        }
        
        QMessageBox {
            background-color: #2d2d2d;
        }
        
        QMessageBox QLabel {
            color: #d0d0d0;
        }
        
        /* Новые стили для кардинально другого интерфейса */
        QListWidget {
            background-color: #1a1a1a;
            border: none;
            outline: none;
            color: #d0d0d0;
            font-size: 11pt;
            padding: 10px;
        }
        
        QListWidget::item {
            padding: 15px 20px;
            border-radius: 8px;
            margin: 3px;
            background-color: #252525;
            border-left: 4px solid transparent;
        }
        
        QListWidget::item:hover {
            background-color: #2d2d2d;
            border-left-color: #5a9;
        }
        
        QListWidget::item:selected {
            background-color: #353535;
            border-left-color: #7bb;
            color: #ffffff;
            font-weight: 600;
        }
        
        QStackedWidget {
            background-color: #1e1e1e;
        }
        
        QFrame {
            background-color: #252525;
            border: 1px solid #3d3d3d;
            border-radius: 8px;
            padding: 15px;
        }
        
        QScrollArea {
            border: none;
            background-color: #1e1e1e;
        }
        
        QScrollArea QWidget {
            background-color: #1e1e1e;
        }
    )");
}

void MainWindow::setupUI() { setupNewUI(); }

void MainWindow::setupMenuBar()
{
    QMenu *fileMenu = menuBar()->addMenu("Файл");
    fileMenu->addAction("Сохранить все", QKeySequence::Save, this, &MainWindow::saveAllData);
    fileMenu->addAction("Загрузить все", QKeySequence::Open, this, &MainWindow::loadAllData);
    fileMenu->addSeparator();
    fileMenu->addAction("Выход", QKeySequence::Quit, this, &QWidget::close);

    QMenu *viewMenu = menuBar()->addMenu("Вид");
    viewMenu->addAction("Обновить все", this, &MainWindow::refreshAllData);

    showStatusMessage(Constants::Messages::READY);
}

void MainWindow::setupNewUI()
{
    auto *centralWidget = new QWidget(this);
    auto *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    auto *sidebar = new QFrame;
    sidebar->setFixedWidth(250);
    sidebar->setStyleSheet("background-color: #1a1a1a; border-right: 1px solid #3d3d3d;");

    auto *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(0, 20, 0, 20);
    sidebarLayout->setSpacing(10);

    auto *titleLabel = new QLabel("Недвижимость");
    titleLabel->setStyleSheet("color: #ffffff; font-size: 18pt; font-weight: "
                              "bold; padding: 10px 20px;");
    sidebarLayout->addWidget(titleLabel);

    sidebarLayout->addSpacing(20);

    navigationList = new QListWidget;
    navigationList->addItem("Панель управления");
    navigationList->addItem("Недвижимость");
    navigationList->addItem("Клиенты");
    navigationList->addItem("Сделки");
    navigationList->addItem("Аукционы");
    navigationList->setCurrentRow(0);
    sidebarLayout->addWidget(navigationList);

    sidebarLayout->addStretch();

    auto *saveBtn = new QPushButton("Сохранить");
    auto *loadBtn = new QPushButton("Загрузить");
    saveBtn->setStyleSheet("QPushButton { background-color: #2d5a3d; padding: 10px; border-radius: "
                           "5px; } QPushButton:hover { background-color: #3d6a4d; }");
    loadBtn->setStyleSheet("QPushButton { background-color: #2d3d5a; padding: 10px; border-radius: "
                           "5px; } QPushButton:hover { background-color: #3d4d6a; }");
    connect(saveBtn, &QPushButton::clicked, this, &MainWindow::saveAllData);
    connect(loadBtn, &QPushButton::clicked, this, &MainWindow::loadAllData);
    sidebarLayout->addWidget(saveBtn);
    sidebarLayout->addWidget(loadBtn);

    contentStack = new QStackedWidget;

    dashboardWidget = new DashboardWidget(agency, this);
    propertiesWidget = new PropertiesWidget(agency, this);
    clientsWidget = new ClientsWidget(agency, this);
    transactionsWidget = new TransactionsWidget(agency, this);
    auctionsWidget = new AuctionsWidget(agency, this);

    contentStack->addWidget(dashboardWidget);
    contentStack->addWidget(propertiesWidget);
    contentStack->addWidget(clientsWidget);
    contentStack->addWidget(transactionsWidget);
    contentStack->addWidget(auctionsWidget);

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(contentStack, 1);

    setCentralWidget(centralWidget);

    connect(navigationList, &QListWidget::currentRowChanged, this, &MainWindow::onNavigationChanged);
    connect(navigationList, &QListWidget::currentRowChanged, contentStack, &QStackedWidget::setCurrentIndex);

    connect(propertiesWidget, &PropertiesWidget::dataChanged, this, &MainWindow::onDataChanged);
    connect(clientsWidget, &ClientsWidget::dataChanged, this, &MainWindow::onDataChanged);
    connect(transactionsWidget, &TransactionsWidget::dataChanged, this, &MainWindow::onDataChanged);
    connect(auctionsWidget, &AuctionsWidget::dataChanged, this, &MainWindow::onDataChanged);
    connect(dashboardWidget, &DashboardWidget::saveRequested, this, &MainWindow::saveAllData);
    connect(dashboardWidget, &DashboardWidget::loadRequested, this, &MainWindow::loadAllData);
    connect(dashboardWidget, &DashboardWidget::refreshRequested, this, &MainWindow::refreshAllData);
}

void MainWindow::updateDashboardStats()
{
    if (dashboardWidget)
    {
        dashboardWidget->updateStats();
    }
}

void MainWindow::showStatusMessage(const QString &message, int timeout) const
{
    if (statusBar())
    {
        statusBar()->showMessage(message, timeout);
    }
}

void MainWindow::refreshAllData()
{
    if (propertiesWidget)
        propertiesWidget->refresh();
    if (clientsWidget)
        clientsWidget->refresh();
    if (transactionsWidget)
        transactionsWidget->refresh();
    if (auctionsWidget)
        auctionsWidget->refresh();
    updateDashboardStats();
    showStatusMessage("Все данные обновлены", 2000);
}

void MainWindow::onNavigationChanged(int index)
{
    if (index == 0)
    {
        updateDashboardStats();
    }
}

void MainWindow::onDataChanged() { updateDashboardStats(); }

void MainWindow::saveAllData()
{
    if (!agency)
    {
        QMessageBox::warning(this, "Ошибка", "Система не инициализирована");
        return;
    }

    try
    {
        agency->saveAllData();
        refreshAllData();
        showStatusMessage("Данные сохранены", 3000);
        QMessageBox::information(this, "Успех", "Все данные успешно сохранены");
    }
    catch (const FileManagerException &e)
    {
        QMessageBox::warning(this, "Ошибка", QString("Ошибка файлового менеджера: %1").arg(e.what()));
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        QMessageBox::warning(this, "Ошибка", QString("Ошибка файловой системы: %1").arg(e.what()));
    }
    catch (const PropertyManagerException &e)
    {
        QMessageBox::warning(this, "Ошибка", QString("Ошибка менеджера недвижимости: %1").arg(e.what()));
    }
    catch (const ClientManagerException &e)
    {
        QMessageBox::warning(this, "Ошибка", QString("Ошибка менеджера клиентов: %1").arg(e.what()));
    }
    catch (const TransactionManagerException &e)
    {
        QMessageBox::warning(this, "Ошибка", QString("Ошибка менеджера сделок: %1").arg(e.what()));
    }
    catch (const AuctionManagerException &e)
    {
        QMessageBox::warning(this, "Ошибка", QString("Ошибка менеджера аукционов: %1").arg(e.what()));
    }
    catch (const std::runtime_error &e)
    {
        QMessageBox::warning(this, "Ошибка", QString("Ошибка выполнения: %1").arg(e.what()));
    }
    catch (const std::bad_alloc &e)
    {
        QMessageBox::warning(this, "Ошибка", QString("Недостаточно памяти: %1").arg(e.what()));
    }
}

void MainWindow::loadAllData()
{
    if (!agency)
    {
        QMessageBox::warning(this, "Ошибка", "Система не инициализирована");
        return;
    }

    try
    {
        agency->loadAllData();
        refreshAllData();
        showStatusMessage("Данные загружены", 3000);
        QMessageBox::information(this, "Успех", "Данные успешно загружены");
    }
    catch (const FileManagerException &e)
    {
        QMessageBox::warning(this, "Ошибка", QString("Ошибка файлового менеджера: %1").arg(e.what()));
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        QMessageBox::warning(this, "Ошибка", QString("Ошибка файловой системы: %1").arg(e.what()));
    }
    catch (const std::exception &e)
    {
        QMessageBox::warning(this, "Ошибка", QString("Ошибка при загрузке: %1").arg(e.what()));
    }
}
