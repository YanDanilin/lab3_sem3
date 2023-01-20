#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QtMath>
#include "Graph.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void on_directedButton_clicked();

    void on_avButton_clicked();

    void on_weightButton_clicked();

    void on_dgButton_clicked();

    void on_dvButton_clicked();

    void on_aeButton_clicked();

    void on_deButton_clicked();

    void on_min_path_clicked();

    void on_cycle_preset_clicked();

    void on_euler_path_clicked();

    void on_for_min_clicked();

    void on_complete_graph_clicked();

private:
    bool is_directed = false;
    bool is_weighed = false;
    size_t count = 0;
    size_t cur_id = 0;
    UndirectedGraph<int, int> undgraph;
    DirectedGraph<int, int> dgraph;
    Ui::MainWindow *ui;
    Graph<int, int>::Path path;
    std::unordered_map<size_t, QPointF> pos;
    void draw_vertex(size_t id, int value, int len, double phi, QPointF centre, QPainter& painter, int counter);
    void draw_connections(size_t from, size_t to, int value, bool direct, QPainter& painter);

    std::unordered_map<Pair<size_t, size_t>, QColor, pair_hash<size_t>, pair_unord_equal<size_t>> colours;

};
#endif // MAINWINDOW_H
