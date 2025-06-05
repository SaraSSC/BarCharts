#include "mainwindow.h"

#include <QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QLegend>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QHorizontalBarSeries>

#include <QHBoxLayout>
#include <QDebug>
#include <QMessageBox>

#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

#include <QSettings>
#include <QComboBox>

QT_USE_NAMESPACE


int main(int argc, char *argv[])
{
    QBarSet* selectedSet = nullptr;
    int selectedIndex = -1;


    // -------------First Chart---------------//
    QApplication a(argc,argv);
    QBarSet *set0 = new QBarSet("Altuve");
    QBarSet *set1 = new QBarSet("Martins");

    QBarSet *set2 = new QBarSet("Laranjeira");




    QValueAxis *valueAxis =  new QValueAxis();
    valueAxis->setRange(0, 100); //def the max and min values on the chart's legend
    valueAxis->setTickCount(11);
    valueAxis->setLabelFormat("%d");

    //Adding the values to each bar
    *set0 << 0;
    *set1 << 0;
    *set2 << 100;





    //def the max and min values for them and user warning
    QList<QBarSet*> sets = {set0, set1, set2};
    QString message;
    for(QBarSet* set : sets){
        for (int i= 0; i < set->count();i++){
            qreal value = (*set)[i];
            if (value > 100){
                message += QString("%1 value is greater than 100: %2\n")
                .arg(set->label()).arg(value);
            } else if (value < 0) {
                message += QString("%1 value is less than 0: %2\n")
                .arg(set->label()).arg(value);
            }
        }
    }

    // Show message if any out-of-range values found
    if (!message.isEmpty()) {
        QMessageBox::information(nullptr, "Out of Range Values", message);
    }


    QHorizontalBarSeries *series = new QHorizontalBarSeries();
    series->append(set0);
    series->append(set1);

    series->append(set2);



    series->setLabelsVisible(true);

    QChart *chart = new QChart();

    //Adding the chart
    chart->addSeries(series);


    //Placing the category titles
    QStringList categories;
    categories << "2013";



    //Ads it to the axis
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    series->attachAxis(axis);
    series->attachAxis(valueAxis);

    //Calculate the total of the bars
    qreal total = 0;
    for (QBarSet* set : sets){
        for (int i = 0; i < set->count(); i++){
            total += (*set)[i];
        };
    };


    //Check if total is out range
    bool totalOutOfRange = (total < 0 || total > 100);

    //Bar Chart Axis
    chart->setAxisY(axis, series);
    chart->setAxisX(valueAxis, series);


    //Title and Animaitions
    chart->setTitle("Battling Avg of the Year");
    chart->setAnimationOptions(QChart::AllAnimations);
    QFont labelFont;
    labelFont.setBold(true);

    QLabel *totalLabel = new QLabel(QString("Total value: %1")
                                        .arg(total));
    if (totalOutOfRange){
        totalLabel->setStyleSheet("color: red; font-weight: bold");
        totalLabel->setText(totalLabel->text() + "(Out of range!)");
    }


    QPalette pal = qApp->palette();
    pal.setColor(QPalette::Window,QRgb(0xffffff));
    pal.setColor(QPalette::WindowText,QRgb(0x404040));
    qApp->setPalette(pal);







    // -------------Second Chart---------------//

    QBarSet *setA = new QBarSet("Player A");
    QBarSet *setB = new QBarSet("Player B");

    *setA << 200 << 220 << 210 << 250 << 270 << 260;
    *setB << 180 << 210 << 230 << 240 << 260 << 250;

    QBarSeries *series2 = new QBarSeries();
    series2->append(setA);
    series2->append(setB);

    QChart *chart2 = new QChart();
    chart2->addSeries(series2);
    chart2->setTitle("Play Vertical Bar Chart");
    chart2->setAnimationOptions(QChart::AllAnimations);

    // Used to display the second chart
    QChartView *chartView2 = new QChartView(chart2);
    chartView2->setRenderHint(QPainter::Antialiasing);

    QBarCategoryAxis *axis2 = new QBarCategoryAxis();
    axis2->append(categories);
    chart2->createDefaultAxes();
    chart2->legend()->setVisible(true);
    chart2->legend()->setAlignment(Qt::AlignBottom);
    series2->attachAxis(axis2);
    chart2->setAxisX(axis2, series2);
    chartView2->setRenderHint(QPainter::Antialiasing);



    // -------------Layout---------------//
    // Create the main app window
    QMainWindow window;

    //Used to display the first chart
    QChartView *chartView1 = new QChartView(chart);
    chartView1->setRenderHint(QPainter::Antialiasing);

    // Layout for first chart and its total label (vertical)
    QVBoxLayout *firstChartLayout = new QVBoxLayout();
    firstChartLayout->addWidget(chartView1);
    firstChartLayout->addWidget(totalLabel);

    QWidget *firstChartWidget = new QWidget();
    firstChartWidget->setLayout(firstChartLayout);



    // Main layout: horizontal for both charts
    QWidget *centralWidget = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->addWidget(firstChartWidget);
    mainLayout->addWidget(chartView2);



    // Create + and - buttons
    QPushButton *plusButton = new QPushButton("+");
    QPushButton *minusButton = new QPushButton("-");

    for (QBarSet* set : sets){
        QObject::connect(set, &QBarSet::clicked, [&](int index){
            selectedSet = set;
            selectedIndex = index;
        });
    }

    // Highlight function
    auto highlightSelectedBar = [&]() {
        for (QBarSet* set : sets) {
            set->setColor(Qt::blue); // Default color
        }
        if (selectedSet) {
            selectedSet->setColor(Qt::red); // Highlight selected
        }
    };

    // Update total label function
    auto updateTotalLabel = [&]() {
        qreal total = 0;
        for (QBarSet* set : sets){
            for (int i = 0; i < set->count(); i++){
                total += set->at(i);
            }
        }
        bool totalOutOfRange = (total < 0 || total > 100);
        totalLabel->setText(QString("Total value: %1%2 %")
                                .arg(total)
                                .arg(totalOutOfRange ? " (Out of range!)" : ""));
        totalLabel->setStyleSheet(totalOutOfRange ? "color: red; font-weight: bold" : "");


    };
    // Check if we can increment without exceeding 100
    if (total < 100) {
        qreal value = selectedSet->at(selectedIndex);
        qreal increment = qMin(1.0, 100.0 - total); // Don't exceed 100
        selectedSet->replace(selectedIndex, value + increment);
        series->setLabelsVisible(true);
        chartView1->update();
        updateTotalLabel();
    } else {
        // Optional: show message that total is at maximum
        QMessageBox::information(nullptr, "Maximum Reached",
                                 "Total value is already at maximum (100).");
    }

    // Connect bar clicks
    for (QBarSet* set : sets){
        QObject::connect(set, &QBarSet::clicked, [&, set](int index){
            selectedSet = set;
            selectedIndex = index;
            highlightSelectedBar();
            updateTotalLabel();
        });
    }


    //---------- Bars single bar movement -----------//
    /*
    // Connect plus button
    QObject::connect(plusButton, &QPushButton::clicked, [&](){
        if (selectedSet && selectedIndex >= 0) {
            // Calculate current total
            qreal total = 0;
            for (QBarSet* set : sets) {
                for (int i = 0; i < set->count(); i++) {
                    total += set->at(i);
                }
            }

            // Check if we can increment without exceeding 100
            if (total < 100) {
                qreal value = selectedSet->at(selectedIndex);
                qreal increment = qMin(1.0, 100.0 - total); // Don't exceed 100
                selectedSet->replace(selectedIndex, value + increment);
                series->setLabelsVisible(true);
                chartView1->update();
                updateTotalLabel();
            } else {
                // Optional: show message that total is at maximum
                QMessageBox::information(nullptr, "Maximum Reached",
                                         "Total value is already at maximum (100).");
            }
        }
    });

    // Connect minus button
    QObject::connect(minusButton, &QPushButton::clicked, [&](){
        if (selectedSet && selectedIndex >= 0) {
            qreal value = selectedSet->at(selectedIndex);
            if (value > 0) { // Don't go below 0
                selectedSet->replace(selectedIndex, value - 1);
                series->setLabelsVisible(true);
                chartView1->update();
                updateTotalLabel();
            }
        }
    });

*/

    // Value to be always at 100

    // -------------Dynamic Bars---------------//
    // Connect plus button

    QObject::connect(plusButton, &QPushButton::clicked, [&](){
        if (selectedSet && selectedIndex >= 0) {
            // Calculate current total FIRST
            qreal currentTotal = 0;
            for (QBarSet* set : sets) {
                for (int i = 0; i < set->count(); i++) {
                    currentTotal += set->at(i);
                }
            }

            // Ensure the total is 100 before redistributing
            if (currentTotal == 100) {
                // Get current value of the selected bar
                qreal value = selectedSet->at(selectedIndex);
                qreal increment = 1.0; // Increment by 1 unit

                // Check if we can increment this value
                if (value + increment <= 100) {
                    // Increase the selected bar
                    selectedSet->replace(selectedIndex, value + increment);

                    // Calculate the total decrement needed
                    qreal decrementNeeded = increment;

                    // Redistribute the decrement proportionally among other bars
                    for (QBarSet* set : sets) {
                        for (int i = 0; i < set->count(); i++) {
                            if (set != selectedSet || i != selectedIndex) {
                                qreal currentValue = set->at(i);
                                qreal decrement = (currentValue / (currentTotal - value)) * decrementNeeded;
                                set->replace(i, qMax(0.0, currentValue - decrement));
                            }
                        }
                    }

                    // Update the chart and total label
                    series->setLabelsVisible(true);
                    chartView1->update();
                    updateTotalLabel();
                }
            } else {
                // Show message if total is not 100
                QMessageBox::information(nullptr, "Invalid Operation",
                                         "Total value must be 100 to redistribute values.");
            }
        }
    });

    // Connect minus button
    QObject::connect(minusButton, &QPushButton::clicked, [&](){
        if (selectedSet && selectedIndex >= 0) {
            // Get current value
            qreal value = selectedSet->at(selectedIndex);
            qreal decrement = 1.0;

            // Check if we can decrement this value
            if (value - decrement >= 0) {
                // Decrease selected bar
                selectedSet->replace(selectedIndex, value - decrement);

                // Calculate how much to increase per other bar
                int otherBarsCount = 0;
                for (QBarSet* set : sets) {
                    for (int i = 0; i < set->count(); i++) {
                        if (set != selectedSet || i != selectedIndex) {
                            otherBarsCount++;
                        }
                    }
                }

                qreal increasePerBar = decrement / otherBarsCount;

                // Increase other bars proportionally
                for (QBarSet* set : sets) {
                    for (int i = 0; i < set->count(); i++) {
                        if (set != selectedSet || i != selectedIndex) {
                            qreal currentValue = set->at(i);
                            qreal newValue = qMin(100.0, currentValue + increasePerBar);
                            set->replace(i, newValue);
                        }
                    }
                }

                series->setLabelsVisible(true);
                chartView1->update();
                updateTotalLabel();
            }
        }
    });


    // -------------Layout---------------//

    // Set the main window widget
    window.setCentralWidget(centralWidget);
    window.resize(1000, 500);
    window.show();

    // Add buttons below the total label in firstChartLayout
    firstChartLayout->addWidget(plusButton);
    firstChartLayout->addWidget(minusButton);

    plusButton->animateClick();
    minusButton->animateClick();

    plusButton->setStyleSheet(
        "QPushButton:hover { background-color: #cccccc; }"
        );
    minusButton->setStyleSheet(
        "QPushButton:hover { background-color: #cccccc; }"
        );

    // Save bar values before exit
    QObject::connect(&a, &QApplication::aboutToQuit, [&](){
        QSettings settings("YourCompany", "YourApp");
        for (int i = 0; i < sets.size(); ++i) {
            settings.beginGroup(QString("BarSet%1").arg(i));
            QBarSet* set = sets[i];
            settings.setValue("label", set->label());
            QVariantList values;
            for (int j = 0; j < set->count(); ++j) {
                values << set->at(j);
            }
            settings.setValue("values", values);
            settings.endGroup();
        }
    });

    // Restore bar values on startup
    QSettings settings("YourCompany", "YourApp");
    for (int i = 0; i < sets.size(); ++i) {
        settings.beginGroup(QString("BarSet%1").arg(i));
        QVariantList values = settings.value("values").toList();
        if (!values.isEmpty()) {
            sets[i]->remove(0, sets[i]->count()); // Clear old values
            for (const QVariant& v : values) {
                *(sets[i]) << v.toReal();
            }
        }
        settings.endGroup();
    }

    // Create the reset button
    QPushButton *resetButton = new QPushButton("Reset");
    resetButton->setStyleSheet(
        "QPushButton:hover { background-color: #cccccc; }"
        );

    // Connect reset button to restore default values
    QObject::connect(resetButton, &QPushButton::clicked, [&](){
        // Reset values to 100, 0, 0
        set0->replace(0, 0);
        set1->replace(0, 0);
        set2->replace(0, 100);


        // Reset colors
        for (QBarSet* set : sets) {
            set->setColor(Qt::blue);
        }

        // Reset selection
        selectedSet = nullptr;
        selectedIndex = -1;

        // Update the chart and total label
        series->setLabelsVisible(true);
        chartView1->update();
        updateTotalLabel();

        // Inform the user
        QMessageBox::information(nullptr, "Values Reset",
                                 "All values have been reset to their defaults.");
    });

    // Add the reset button to the layout
    firstChartLayout->addWidget(resetButton);


    /*TODO: adding the relative and absolute
            bars to incremente and decrement the relative value
            label to each bar with the relative value
             */

    QComboBox *barSelector = new QComboBox();
    barSelector->addItem("Altuve");
    barSelector->addItem("Martins");
    barSelector->addItem("Laranjeira");


    barSelector->setCurrentIndex(0);

    //Connect the combobox selection to highlight the bar we are selectiing
    QObject::connect(barSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
                     [&](int index){
                         if (index >= 0 && index < sets.size()){
                            selectedSet = sets[index];
                            selectedIndex = 0;
                            highlightSelectedBar;
                         };

    });

    // Add a label to the combobox in the layout
    QLabel *selectedLabel = new QLabel("Select a bar:");
    QHBoxLayout *selectorLayout = new QHBoxLayout();
    selectorLayout->addWidget(selectedLabel);
    selectorLayout->addWidget(barSelector);

    QWidget *selectorWidget = new QWidget();
    selectorWidget->setLayout(selectorLayout);

    //Rearange the layout
    firstChartLayout->addWidget(selectorWidget);
    firstChartLayout->addWidget(plusButton);
    firstChartLayout->addWidget(minusButton);




    updateTotalLabel();
    return a.exec();


}
