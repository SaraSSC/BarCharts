#include "mainwindow.h"

#include <QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QLegend>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QHorizontalStackedBarSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QHorizontalBarSeries>

#include <QHBoxLayout>
#include <QDebug>
#include <QMessageBox>


QT_USE_NAMESPACE


int main(int argc, char *argv[])
{
    // -------------First Chart---------------//
    QApplication a(argc,argv);
    QBarSet *set0 = new QBarSet("Altuve");
    QBarSet *set1 = new QBarSet("Martins");
    QBarSet *set2 = new QBarSet("Laranjeira");
    QBarSet *set3 = new QBarSet("Duarte");
    QBarSet *set4 = new QBarSet("Jerónimo");


    QValueAxis *valueAxis =  new QValueAxis();
    valueAxis->setRange(0, 100);
    valueAxis->setTickCount(11);
    valueAxis->setLabelFormat("%d");

    *set0 << 10;
    *set1 << 5;
    *set2 << 6;
    *set3 << 170;
    *set4 << 30;

    QList<QBarSet*> sets = {set0, set1, set2, set3, set4};
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

   //BarSeries *series = new QBarSeries();
    QHorizontalBarSeries *series = new QHorizontalBarSeries();
    series->append(set0);
    series->append(set1);
    series->append(set2);
    series->append(set3);
    series->append(set4);

    QChart *chart = new QChart();

    //Adding the chart
    chart->addSeries(series);

    //Title and Animaitions
    chart->setTitle("Battling Avg of the Year");
    chart->setAnimationOptions(QChart::AllAnimations);

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

    //Bar Chart Axis
    chart->setAxisY(axis, series);
    chart->setAxisX(valueAxis, series);

    QPalette pal = qApp->palette();
    pal.setColor(QPalette::Window,QRgb(0xffffff));
    pal.setColor(QPalette::WindowText,QRgb(0x404040));
    qApp->setPalette(pal);




    // Used to display the chart
    QChartView *chartView1= new QChartView(chart);
    chartView1->setRenderHint(QPainter::Antialiasing);


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

    QBarCategoryAxis *axis2 = new QBarCategoryAxis();
    axis2->append(categories);
    chart2->createDefaultAxes();
    chart2->legend()->setVisible(true);
    chart2->legend()->setAlignment(Qt::AlignBottom);
    series2->attachAxis(axis2);
    chart2->setAxisX(axis2, series2);

    QChartView *chartView2 = new QChartView(chart2);
    chartView2->setRenderHint(QPainter::Antialiasing);

    // Create the main app window
    QMainWindow window;



    QWidget *centralWidget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(centralWidget);
    layout->addWidget(chartView1);
    layout->addWidget(chartView2);

    // Set the main window widget
    window.setCentralWidget(centralWidget);
    window.resize(900,400);
    window.show();

    return a.exec();


}
