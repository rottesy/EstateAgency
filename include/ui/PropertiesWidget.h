#ifndef PROPERTIESWIDGET_H
#define PROPERTIESWIDGET_H

#include "../core/EstateAgency.h"
#include "../entities/Property.h"
#include <QLineEdit>
#include <QPushButton>
#include <QSplitter>
#include <QTableWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

class PropertiesWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit PropertiesWidget(EstateAgency *agency, QWidget *parent = nullptr);
    void refresh();
    void updateTable();

  signals:
    void dataChanged();

  private slots:
    void addProperty();
    void editProperty();
    void deleteProperty();
    void searchProperties();
    void propertySelectionChanged();

  private:
    void setupUI();
    void showPropertyDetails(Property *prop);
    void showPropertyTransactions(const std::string &propertyId);
    QWidget *createActionButtons(QTableWidget *table, const QString &id, const std::function<void()> &editAction,
                                 const std::function<void()> &deleteAction);
    void selectRowById(QTableWidget *table, const QString &id) const;
    QString getSelectedIdFromTable(QTableWidget *table) const;
    bool checkTableSelection(QTableWidget *table, const QString &errorMessage);
    bool isNumericId(const QString &text) const;

    EstateAgency *agency;
    QTableWidget *propertiesTable;
    QPushButton *addPropertyBtn;
    QPushButton *refreshPropertyBtn;
    QPushButton *searchPropertyBtn;
    QLineEdit *searchPropertyEdit;
    QTextEdit *propertyDetailsText;
};

#endif
