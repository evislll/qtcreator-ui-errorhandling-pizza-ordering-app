#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QMessageBox>
#include <QRegularExpressionValidator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateSummary();
    void validatePostalCode();
    void validateName();
    void validateAddress();
    void validateCity();
    void updateToppingAvailability();
    void updateExtraCheeseAvailability();
    void onResetClicked();
    void onCancelClicked();
    void onOrderClicked();
    void showOrderConfirmation();

private:
    Ui::MainWindow *ui;

    // Widgets
    QLineEdit *nameEdit;
    QLineEdit *addressEdit;
    QLineEdit *apartmentEdit;
    QLineEdit *cityEdit;
    QLineEdit *postalCodeEdit;
    QComboBox *sizeComboBox;
    QCheckBox *toppingHam;
    QCheckBox *toppingPepperoni;
    QCheckBox *toppingGreenPepper;
    QCheckBox *toppingMushroom;
    QCheckBox *toppingPineapple;
    QCheckBox *toppingOnion;
    QCheckBox *toppingArtichokes;
    QCheckBox *toppingOlives;
    QCheckBox *extraCheeseCheck;
    QPushButton *resetButton;
    QPushButton *cancelButton;
    QPushButton *orderButton;
    QLabel *summaryLabel;
    QLabel *errorLabel;

    // Validation
    bool validateForm();
    int countSelectedToppings();

    // Colors for validation
    QString errorStyle = "QLineEdit { border: 2px solid red; }";
    QString normalStyle = "QLineEdit { border: 1px solid gray; }";
};
#endif // MAINWINDOW_H
