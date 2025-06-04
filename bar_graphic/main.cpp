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


QT_USE_NAMESPACE


int main(int argc, char *argv[])
{
    QApplication a(argc,argv);
    QBarSet *set0 = new QBarSet("Altuve");
    QBarSet *set1 = new QBarSet("Martins");
    QBarSet *set2 = new QBarSet("Laranjeira");
    QBarSet *set3 = new QBarSet("Duarte");
    QBarSet *set4 = new QBarSet("Jerónimo");


    *set0 << 326; // Example: average or any single value
    *set1 << 298;
    *set2 << 290;
    *set3 << 271;
    *set4 << 307;

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

    //Bar chart
   //hart->setAxisX(axis, series);

    //Stacked Bar Chart
    chart->setAxisY(axis, series);

    QPalette pal = qApp->palette();
    pal.setColor(QPalette::Window,QRgb(0xffffff));
    pal.setColor(QPalette::WindowText,QRgb(0x404040));
    qApp->setPalette(pal);




    // Used to display the chart
    QChartView *chartView1= new QChartView(chart);
    chartView1->setRenderHint(QPainter::Antialiasing);


    // ----------------------------//

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
