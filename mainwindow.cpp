#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)

{
    setWindowTitle("Pizza Ordering System");

    // Main widget and layout
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // ========== 1. Delivery Information Group ==========
    QGroupBox *deliveryGroup = new QGroupBox("🏠 Delivery Information", centralWidget);
    deliveryGroup->setStyleSheet("QGroupBox { font-weight: bold; font-size: 14px; }");

    QFormLayout *formLayout = new QFormLayout(deliveryGroup);

    nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("Enter your name");
    formLayout->addRow("Name:", nameEdit);

    addressEdit = new QLineEdit();
    addressEdit->setPlaceholderText("Street address");
    formLayout->addRow("Street Address:", addressEdit);

    apartmentEdit = new QLineEdit();
    apartmentEdit->setPlaceholderText("Optional");
    formLayout->addRow("Apartment #:", apartmentEdit);

    cityEdit = new QLineEdit();
    cityEdit->setPlaceholderText("City");
    formLayout->addRow("City:", cityEdit);

    postalCodeEdit = new QLineEdit();
    postalCodeEdit->setPlaceholderText("A1A 1A1");
    formLayout->addRow("Postal Code:", postalCodeEdit);

    // Postal code validator (Canadian format)
    QRegularExpression postalRegex("[A-Za-z]\\d[A-Za-z] \\d[A-Za-z]\\d");
    QRegularExpressionValidator *postalValidator = new QRegularExpressionValidator(postalRegex, this);
    postalCodeEdit->setValidator(postalValidator);

    mainLayout->addWidget(deliveryGroup);

    // ========== 2. Pizza Configuration Group ==========
    QGroupBox *pizzaGroup = new QGroupBox("🍕 Pizza Configuration", centralWidget);
    pizzaGroup->setStyleSheet("QGroupBox { font-weight: bold; font-size: 14px; }");

    QVBoxLayout *pizzaLayout = new QVBoxLayout(pizzaGroup);

    // Size selection
    QHBoxLayout *sizeLayout = new QHBoxLayout();
    sizeLayout->addWidget(new QLabel("Size:"));
    sizeComboBox = new QComboBox();
    sizeComboBox->addItems({"Small", "Medium", "Large"});
    sizeComboBox->setCurrentIndex(1); // Default to Medium
    sizeLayout->addWidget(sizeComboBox);
    sizeLayout->addStretch();
    pizzaLayout->addLayout(sizeLayout);

    // Toppings selection
    QGroupBox *toppingsGroup = new QGroupBox("Toppings (max 3)");
    QGridLayout *toppingsLayout = new QGridLayout(toppingsGroup);

    toppingHam = new QCheckBox("Ham");
    toppingPepperoni = new QCheckBox("Pepperoni");
    toppingGreenPepper = new QCheckBox("Green Pepper");
    toppingMushroom = new QCheckBox("Mushroom");
    toppingPineapple = new QCheckBox("Pineapple");
    toppingOnion = new QCheckBox("Onion");
    toppingArtichokes = new QCheckBox("Artichokes");
    toppingOlives = new QCheckBox("Black Olives");

    toppingsLayout->addWidget(toppingHam, 0, 0);
    toppingsLayout->addWidget(toppingPepperoni, 0, 1);
    toppingsLayout->addWidget(toppingGreenPepper, 1, 0);
    toppingsLayout->addWidget(toppingMushroom, 1, 1);
    toppingsLayout->addWidget(toppingPineapple, 2, 0);
    toppingsLayout->addWidget(toppingOnion, 2, 1);
    toppingsLayout->addWidget(toppingArtichokes, 3, 0);
    toppingsLayout->addWidget(toppingOlives, 3, 1);

    pizzaLayout->addWidget(toppingsGroup);

    // Extra cheese option
    extraCheeseCheck = new QCheckBox("Extra Cheese (+$1.50)");
    pizzaLayout->addWidget(extraCheeseCheck);

    mainLayout->addWidget(pizzaGroup);

    // ========== 3. Order Summary Group ==========
    QGroupBox *summaryGroup = new QGroupBox("🛒 Order Summary", centralWidget);
    summaryGroup->setStyleSheet("QGroupBox { font-weight: bold; font-size: 14px; background-color: #f0f0f0; }");

    QVBoxLayout *summaryLayout = new QVBoxLayout(summaryGroup);
    summaryLabel = new QLabel("No order yet. Configure your pizza above.");
    summaryLabel->setWordWrap(true);
    summaryLabel->setStyleSheet("QLabel { padding: 10px; }");
    summaryLayout->addWidget(summaryLabel);

    mainLayout->addWidget(summaryGroup);

    // ========== 4. Error Display ==========
    errorLabel = new QLabel();
    errorLabel->setStyleSheet("QLabel { color: red; font-weight: bold; padding: 5px; }");
    errorLabel->setWordWrap(true);
    mainLayout->addWidget(errorLabel);

    // ========== 5. Control Buttons ==========
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    resetButton = new QPushButton("Reset Pizza");
    cancelButton = new QPushButton("Cancel Order");
    orderButton = new QPushButton("Order Pizza!");

    orderButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; padding: 10px; }");
    cancelButton->setStyleSheet("QPushButton { background-color: #f44336; color: white; padding: 10px; }");
    resetButton->setStyleSheet("QPushButton { background-color: #2196F3; color: white; padding: 10px; }");

    buttonLayout->addWidget(resetButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(orderButton);

    mainLayout->addLayout(buttonLayout);

    setCentralWidget(centralWidget);

    // ========== Connect Signals and Slots ==========
    // Real-time validation
    connect(nameEdit, &QLineEdit::textChanged, this, &MainWindow::validateName);
    connect(addressEdit, &QLineEdit::textChanged, this, &MainWindow::validateAddress);
    connect(cityEdit, &QLineEdit::textChanged, this, &MainWindow::validateCity);
    connect(postalCodeEdit, &QLineEdit::textChanged, this, &MainWindow::validatePostalCode);

    // Pizza configuration updates
    connect(sizeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::updateExtraCheeseAvailability);

    // Toppings selection
    QList<QCheckBox*> toppings = {toppingHam, toppingPepperoni, toppingGreenPepper,
                                   toppingMushroom, toppingPineapple, toppingOnion,
                                   toppingArtichokes, toppingOlives};
    for (QCheckBox *topping : toppings) {
        connect(topping, &QCheckBox::stateChanged, this, &MainWindow::updateToppingAvailability);
        connect(topping, &QCheckBox::stateChanged, this, &MainWindow::updateSummary);
    }

    connect(extraCheeseCheck, &QCheckBox::stateChanged, this, &MainWindow::updateSummary);
    connect(sizeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::updateSummary);

    // Button actions
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);
    connect(cancelButton, &QPushButton::clicked, this, &MainWindow::onCancelClicked);
    connect(orderButton, &QPushButton::clicked, this, &MainWindow::onOrderClicked);

    // Initial updates
    updateExtraCheeseAvailability();
    updateSummary();
}

MainWindow::~MainWindow()
{
    //delete ui;
}

// ========== Slot Implementations ==========

void MainWindow::updateSummary()
{
    QString summary = "<h3>Your Order Summary</h3>";

    // Delivery info
    if (!nameEdit->text().isEmpty()) {
        summary += QString("<b>Name:</b> %1<br>").arg(nameEdit->text());
    }
    if (!addressEdit->text().isEmpty()) {
        summary += QString("<b>Address:</b> %1").arg(addressEdit->text());
        if (!apartmentEdit->text().isEmpty()) {
            summary += QString(", Apt %1").arg(apartmentEdit->text());
        }
        summary += "<br>";
    }
    if (!cityEdit->text().isEmpty() || !postalCodeEdit->text().isEmpty()) {
        summary += QString("<b>City:</b> %1, <b>Postal Code:</b> %2<br>")
        .arg(cityEdit->text(), postalCodeEdit->text());
    }

    // Pizza info
    QString size = sizeComboBox->currentText();
    summary += QString("<br><b>Pizza Size:</b> %1<br>").arg(size);

    QStringList toppings;
    if (toppingHam->isChecked()) toppings << "Ham";
    if (toppingPepperoni->isChecked()) toppings << "Pepperoni";
    if (toppingGreenPepper->isChecked()) toppings << "Green Pepper";
    if (toppingMushroom->isChecked()) toppings << "Mushroom";
    if (toppingPineapple->isChecked()) toppings << "Pineapple";
    if (toppingOnion->isChecked()) toppings << "Onion";
    if (toppingArtichokes->isChecked()) toppings << "Artichokes";
    if (toppingOlives->isChecked()) toppings << "Black Olives";

    summary += QString("<b>Toppings:</b> %1<br>").arg(toppings.isEmpty() ? "None" : toppings.join(", "));

    if (extraCheeseCheck->isChecked() && extraCheeseCheck->isEnabled()) {
        summary += "<b>Extra Cheese:</b> Yes (+$1.50)<br>";
    }

    // Price calculation
    double price = 0.0;
    if (size == "Small") price = 8.99;
    else if (size == "Medium") price = 10.99;
    else if (size == "Large") price = 12.99;

    price += toppings.count() * 1.25;
    if (extraCheeseCheck->isChecked() && extraCheeseCheck->isEnabled()) price += 1.50;

    summary += QString("<br><b style='color: green; font-size: 16px;'>Total Price: $%1</b>")
                   .arg(QString::number(price, 'f', 2));

    summaryLabel->setText(summary);
}

void MainWindow::validatePostalCode()
{
    QString text = postalCodeEdit->text();
    QRegularExpression regex("[A-Za-z]\\d[A-Za-z] \\d[A-Za-z]\\d");

    if (text.isEmpty()) {
        postalCodeEdit->setStyleSheet(normalStyle);
        errorLabel->clear();
    } else if (regex.match(text).hasMatch()) {
        postalCodeEdit->setStyleSheet("QLineEdit { border: 2px solid green; }");
        errorLabel->clear();
    } else {
        postalCodeEdit->setStyleSheet(errorStyle);
        errorLabel->setText("⚠ Postal code should be in format: A1A 1A1");
    }
}

void MainWindow::validateName()
{
    if (nameEdit->text().isEmpty()) {
        nameEdit->setStyleSheet(errorStyle);
    } else {
        nameEdit->setStyleSheet(normalStyle);
    }
}

void MainWindow::validateAddress()
{
    if (addressEdit->text().isEmpty()) {
        addressEdit->setStyleSheet(errorStyle);
    } else {
        addressEdit->setStyleSheet(normalStyle);
    }
}

void MainWindow::validateCity()
{
    if (cityEdit->text().isEmpty()) {
        cityEdit->setStyleSheet(errorStyle);
    } else {
        cityEdit->setStyleSheet(normalStyle);
    }
}

void MainWindow::updateToppingAvailability()
{
    int selected = countSelectedToppings();

    // Disable all unchecked toppings if 3 are selected
    bool disableUnchecked = (selected >= 3);

    QList<QCheckBox*> toppings = {toppingHam, toppingPepperoni, toppingGreenPepper,
                                   toppingMushroom, toppingPineapple, toppingOnion,
                                   toppingArtichokes, toppingOlives};

    for (QCheckBox *topping : toppings) {
        if (!topping->isChecked()) {
            topping->setEnabled(!disableUnchecked);
        }
    }

    // Update error label
    if (selected > 3) {
        errorLabel->setText("⚠ Maximum 3 toppings allowed!");
    } else if (selected == 3) {
        errorLabel->setText("✓ Maximum toppings selected");
    } else {
        errorLabel->clear();
    }
}

int MainWindow::countSelectedToppings()
{
    QList<QCheckBox*> toppings = {toppingHam, toppingPepperoni, toppingGreenPepper,
                                   toppingMushroom, toppingPineapple, toppingOnion,
                                   toppingArtichokes, toppingOlives};

    int count = 0;
    for (QCheckBox *topping : toppings) {
        if (topping->isChecked()) count++;
    }
    return count;
}

void MainWindow::updateExtraCheeseAvailability()
{
    QString size = sizeComboBox->currentText();
    if (size == "Large") {
        extraCheeseCheck->setEnabled(false);
        extraCheeseCheck->setChecked(false);
        extraCheeseCheck->setToolTip("Extra cheese not available for Large pizzas");
    } else {
        extraCheeseCheck->setEnabled(true);
        extraCheeseCheck->setToolTip("Add extra cheese for $1.50");
    }
}

void MainWindow::onResetClicked()
{
    // Reset pizza selections only (not address)
    sizeComboBox->setCurrentIndex(1); // Medium

    QList<QCheckBox*> toppings = {toppingHam, toppingPepperoni, toppingGreenPepper,
                                   toppingMushroom, toppingPineapple, toppingOnion,
                                   toppingArtichokes, toppingOlives};
    for (QCheckBox *topping : toppings) {
        topping->setChecked(false);
    }

    extraCheeseCheck->setChecked(false);
    updateExtraCheeseAvailability();
    updateToppingAvailability();
    updateSummary();

    errorLabel->setText("✓ Pizza configuration reset");
}

void MainWindow::onCancelClicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Cancel Order",
                                  "Are you sure you want to cancel the order?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        //qApp->quit();
    }
}

bool MainWindow::validateForm()
{
    QStringList errors;

    if (nameEdit->text().isEmpty()) errors << "Name is required";
    if (addressEdit->text().isEmpty()) errors << "Street address is required";
    if (cityEdit->text().isEmpty()) errors << "City is required";

    QString postal = postalCodeEdit->text();
    QRegularExpression regex("[A-Za-z]\\d[A-Za-z] \\d[A-Za-z]\\d");
    if (!postal.isEmpty() && !regex.match(postal).hasMatch()) {
        errors << "Postal code format: A1A 1A1";
    }

    if (countSelectedToppings() > 3) {
        errors << "Maximum 3 toppings allowed";
    }

    if (!errors.isEmpty()) {
        errorLabel->setText("⚠ " + errors.join(" | "));
        return false;
    }

    errorLabel->setText("✓ All fields are valid!");
    errorLabel->setStyleSheet("QLabel { color: green; font-weight: bold; }");
    return true;
}

void MainWindow::onOrderClicked()
{
    if (validateForm()) {
        showOrderConfirmation();
    }
}

void MainWindow::showOrderConfirmation()
{
    QString confirmation = "<h2>Confirm Your Order</h2>";
    confirmation += summaryLabel->text().replace("<h3>", "<h4>").replace("</h3>", "</h4>");

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Order Confirmation");
    msgBox.setText(confirmation);
    msgBox.setIcon(QMessageBox::Question);

    QPushButton *confirmButton = msgBox.addButton("Confirm Order", QMessageBox::AcceptRole);
    QPushButton *modifyButton = msgBox.addButton("Modify Order", QMessageBox::RejectRole);

    msgBox.exec();

    if (msgBox.clickedButton() == confirmButton) {
        QMessageBox::information(this, "Order Placed",
                                 "Thank you! Your pizza order has been placed.\n"
                                 "The application will now close.");
        //qApp->quit();
    }
    // If Modify button clicked, just return to the application
}
