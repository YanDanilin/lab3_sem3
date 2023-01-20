#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_directedButton_clicked()
{
    colours.clear();
    if (is_directed) {
        dgraph.clear();
    }
    else {
        undgraph.clear();
    }
    is_directed = !is_directed;
    count = 0;
    cur_id = 0;
   repaint();
}

void MainWindow::on_weightButton_clicked()
{
    colours.clear();
    if (is_weighed) {
        dgraph.clear();
    }
    else {
        undgraph.clear();
    }
    count = 0;
    cur_id = 0;
    is_weighed = !is_weighed;
    repaint();
}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setBrush(QBrush(Qt::green));
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    if (is_directed) {
        painter.drawText(QPointF(10, 111), QString("Directed"));
    }
    else {
        painter.drawText(QPointF(10, 111), QString("Undirected"));
    }
    if (is_weighed) {
        painter.drawText(QPointF(10, 126), QString("Weighed"));
    }
    else {
        painter.drawText(QPointF(10, 126), QString("Not weighed"));
    }
    QPointF centre(400, 340);
    if (count == 0) {
        return;
    }
    else if (count == 1) {
        painter.drawEllipse(centre, 12, 12);
        size_t id = 0;
        if (is_directed) {
            for (auto it = dgraph.vertexes.cbegin(); it != dgraph.vertexes.cend(); it++) {
                if (it->second.id != 0) {
                    id = it->second.id;
                    if (is_weighed) {
                        painter.drawText(centre + QPointF(0, 15),  QString::number(it->second.value));
                    }
                    break;
                }
            }
        }
        else {
            for (auto it = undgraph.vertexes.cbegin(); it != undgraph.vertexes.cend(); it++) {
                if (it->second.id != 0) {
                    id = it->second.id;
                    if (is_weighed) {
                        painter.drawText(centre + QPointF(-4, 25),  QString::number(it->second.value));
                    }
                    break;
                }
            }
        }
        painter.drawText(QPointF(397, 344), QString::number(id));
        return;
    }

    double phi = 2 * 3.141592/count;
    int len = 100;
    if (count >= 9) {
        len+=(50 * (count / 9));
    }
    if (is_directed) {
        std::unordered_map<size_t, std::vector<bool>> added_edges;
        added_edges.reserve(count);
        int counter = 0;
        for (auto it = dgraph.vertexes.cbegin(); it != dgraph.vertexes.cend(); it++) {
            draw_vertex(it->first, it->second.value, len, phi, centre, painter, counter);
            counter++;
            added_edges[it->first].resize(dgraph.incidence_list[it->first].size(), false);
        }
        for (auto it = dgraph.incidence_list.cbegin(); it != dgraph.incidence_list.cend(); it++) {
            for (size_t i = 0; i < it->second.size(); i++) {
                if (added_edges[it->first][i] == false) {
                    draw_connections(it->second[i].id_from, it->second[i].id_to, it->second[i].value, true, painter);
                    added_edges[it->first][i] = true;
                }
            }
        }
    }
    else {
        std::unordered_map<size_t, std::vector<bool>> added_edges;
        added_edges.reserve(count);
        int counter = 0;
        for (auto it = undgraph.vertexes.cbegin(); it != undgraph.vertexes.cend(); it++) {
            draw_vertex(it->first, it->second.value, len, phi, centre, painter, counter);
            counter++;
            added_edges[it->first].resize(undgraph.incidence_list[it->first].size(), false);
        }
        for (auto it = undgraph.incidence_list.cbegin(); it != undgraph.incidence_list.cend(); it++) {
            for (size_t i = 0; i < it->second.size(); i++) {
                if (added_edges[it->first][i] == false) {
                    draw_connections(it->second[i].id_from, it->second[i].id_to, it->second[i].value, false, painter);
                    added_edges[it->first][i] = true;
                }
            }
        }
    }
}

void MainWindow::draw_vertex(size_t id, int value, int len, double phi, QPointF centre, QPainter& painter, int counter)
{
    QPointF num_offset(-3, 4);
    if (id >= 10) {
        num_offset.rx() = -7;
    }
    qreal x = qSin(phi * counter) * len;
    qreal y = qCos(phi * counter) * len;
    painter.drawEllipse(QPointF(x, y) + centre, 12, 12);
    pos[id] = QPointF(x, y) + centre;
    painter.drawText(QPointF(x, y) + centre + num_offset, QString::number(id));
    if (is_weighed) {
        x *= 1.25;
        y *= 1.25;
        painter.drawText(QPointF(x, y) + centre + num_offset,  QString::number(value));
    }
}

void MainWindow::draw_connections(size_t from, size_t to, int value, bool direct, QPainter &painter)
{
    qreal x1, x2;// = pos[from].x(), x2 = pos[to].x();
    qreal y1, y2;//; = pos[from].y(), y2 = pos[to].y();
    qreal vx ;//= x2 - x1;
    qreal vy;// = y2 - y1;
    qreal length;// = qSqrt(vx * vx + vy * vy);
    qreal x_offset;// = (qAbs(vx) / length) * 12;
    qreal y_offset;// = (qAbs(vy) / length) * 12;
    short int ky = 1;
    short int kx = 1;

    auto it = colours.find(Pair{from, to});
    if (it != colours.end()) {
        painter.setPen(QPen(it->second));
        if (it->second == Qt::blue) {
            x1 = pos[it->first.first].x(); x2 = pos[it->first.second].x();
            y1 = pos[it->first.first].y(); y2 = pos[it->first.second].y();
            vx = x2 - x1;
            vy = y2 - y1;
            length = qSqrt(vx * vx + vy * vy);
            x_offset = (qAbs(vx) / length) * 12;
            y_offset = (qAbs(vy) / length) * 12;
            if (vx > 0) {
                kx = 1;
                if (vy > 0) {
                    ky = 1;
                }
                else {
                    ky = -1;
                }
            }
            else {
                kx = -1;
                if (vy > 0) {
                    ky = 1;
                }
                else {
                    ky = -1;
                }
            }
            QPointF points[3] = {
                QPointF(pos[it->first.second] + QPointF(-kx * x_offset, -ky * y_offset)),
                QPointF(pos[it->first.second] + QPointF(-kx * x_offset * 1.75, -ky * y_offset * 1.75) + QPointF(ky * y_offset * 0.25 , -kx * x_offset * 0.25)),
                QPointF(pos[it->first.second] + QPointF(-kx * x_offset * 1.75, -ky * y_offset * 1.75) + QPointF(-ky * y_offset * 0.25, kx * x_offset * 0.25))
            };
            painter.setBrush(QBrush(Qt::blue));
            painter.drawPolygon(points, 3);
            painter.setBrush(QBrush(Qt::green));
        }
    }
     x1 = pos[from].x(); x2 = pos[to].x();
     y1 = pos[from].y(); y2 = pos[to].y();
     vx = x2 - x1;
     vy = y2 - y1;
     length = qSqrt(vx * vx + vy * vy);
     x_offset = (qAbs(vx) / length) * 12;
     y_offset = (qAbs(vy) / length) * 12;
     ky = 1;
     kx = 1;
    if (vx > 0) {
        kx = 1;
        if (vy > 0) {
            ky = 1;
        }
        else {
            ky = -1;
        }
    }
    else {
        kx = -1;
        if (vy > 0) {
            ky = 1;
        }
        else {
            ky = -1;
        }
    }
    painter.drawLine(pos[from] + QPointF(kx * x_offset, ky * y_offset), pos[to] + QPointF(-kx * x_offset, -ky * y_offset));
    painter.setPen(QPen(Qt::black));
    if (direct) {
        QPointF points[3] = {
            QPointF(pos[to] + QPointF(-kx * x_offset, -ky * y_offset)),
            QPointF(pos[to] + QPointF(-kx * x_offset * 1.75, -ky * y_offset * 1.75) + QPointF(ky * y_offset * 0.25 , -kx * x_offset * 0.25)),
            QPointF(pos[to] + QPointF(-kx * x_offset * 1.75, -ky * y_offset * 1.75) + QPointF(-ky * y_offset * 0.25, kx * x_offset * 0.25))
        };
        painter.setBrush(QBrush(Qt::black));
        painter.drawPolygon(points, 3);
        painter.setBrush(QBrush(Qt::green));
    }
    if (is_weighed) {
        painter.drawText(QPointF(pos[to].x()/2 + pos[from].x()/2 , pos[to].y()/2 + pos[from].y()/2) + QPointF(ky * y_offset * 0.25 , -kx * x_offset * 0.25) + QPointF(-kx * x_offset * 2, -ky * y_offset * 2), QString::number(value));
    }
}

void MainWindow::on_dgButton_clicked()
{
    colours.clear();
    if (is_directed) {
        dgraph.clear();
    }
    else {
        undgraph.clear();
    }
    count = 0;
    cur_id = 0;
    repaint();
}

void MainWindow::on_dvButton_clicked()
{
    colours.clear();
    size_t id = 0;
    bool ok;
    id = QInputDialog::getInt(this, QString("Vertex deleting"), QString("Enter id"), 0, 1, cur_id, 1, &ok);
    if (ok) {
        if (is_directed) {
            if (dgraph.vertexes.find(id) != dgraph.vertexes.end()) {
                dgraph.erase_vertex(id);
                count--;
            }
        }
        else {
            if (undgraph.vertexes.find(id) != undgraph.vertexes.end()) {
                undgraph.erase_vertex(id);
                count--;
            }
        }
        if (count == 0) {
            cur_id = 0;
        }
        repaint();
    }
}

void MainWindow::on_avButton_clicked()
{
    colours.clear();
//    int value = 0;
//    bool ok;
//    if (is_weighed) {
//        value = QInputDialog::getInt(this, QString("Value receiving"), QString("Enter value"), 0, 1, 500, 1, &ok);
//    }
    if (is_directed) {
        dgraph.add_vertex(cur_id + 1);
    }
    else {
        undgraph.add_vertex(cur_id + 1);
    }
    count++;
    cur_id++;
    repaint();
}

void MainWindow::on_aeButton_clicked()
{
    colours.clear();
    size_t id_from = 0;
    size_t id_to = 0;
    bool ok1, ok2;
    id_from = QInputDialog::getInt(this, QString("Edge input"), QString("Enter id from"), 0, 1, cur_id, 1, &ok1);
    id_to = QInputDialog::getInt(this, QString("Edge input"), QString("Enter id to"), 0, 1, cur_id, 1, &ok2);
    int value = 0;
    bool ok;
    if (is_weighed) {
        value = QInputDialog::getInt(this, QString("Value receiving"), QString("Enter value"), 0, 1, 500, 1, &ok);
    }
    if (ok1 && ok2) {
        if (is_directed) {
            dgraph.add_edge(id_from, id_to, value);
        }
        else {
            undgraph.add_edge(id_from, id_to, value);
        }
    }
    repaint();
}

void MainWindow::on_deButton_clicked()
{
    colours.clear();
    size_t id_from = 0;
    size_t id_to = 0;
    bool ok1, ok2;
    id_from = QInputDialog::getInt(this, QString("Edge input"), QString("Enter id from"), 0, 1, cur_id, 1, &ok1);
    id_to = QInputDialog::getInt(this, QString("Edge input"), QString("Enter id to"), 0, 1, cur_id, 1, &ok2);
    if (ok1 && ok2) {
        if (is_directed) {
            dgraph.erase_edge(id_from, id_to);
        }
        else {
            undgraph.erase_edge(id_from, id_to);
        }
    }
    repaint();
}

void MainWindow::on_min_path_clicked()
{
    path.clear();
    size_t id_from = 0;
    size_t id_to = 0;
    bool ok1, ok2;
    id_from = QInputDialog::getInt(this, QString("Vertex input"), QString("Enter id from"), 0, 1, cur_id, 1, &ok1);
    id_to = QInputDialog::getInt(this, QString("Vertex input"), QString("Enter id to"), 0, 1, cur_id, 1, &ok2);
    if (!ok1 || !ok2) return;
    if (is_directed) {
        path = dgraph.min_path(id_from, id_to);
    }
    else {
        path = undgraph.min_path(id_from, id_to);
    }
    colours.clear();
    for (size_t i = 0; i < path.size(); ++i) {
        colours[Pair{path[i].id_from, path[i].id_to}] = Qt::red;
    }
    repaint();
}

void MainWindow::on_euler_path_clicked()
{
    path.clear();
    colours.clear();
    if (is_directed) {
        QMessageBox::information(this, "Euler path", "This operation is for undirected graphs");
    }
    else {
        path = undgraph.euler_path();
        for(size_t i = 0; i < path.size(); ++i) {
            colours[Pair{path[i].id_from, path[i].id_to}] = Qt::blue;
        }
    }
    repaint();
}
void MainWindow::on_cycle_preset_clicked()
{
    colours.clear();
    dgraph.clear();
    undgraph.clear();
    for (int i = 1; i < 8; ++i) {
        if (is_directed) {
            dgraph.add_edge(i, i + 1);
        }
        else {
            undgraph.add_edge(i, i + 1);
        }
    }
    if (is_directed) {
        dgraph.add_edge(8, 1);
    }
    else {
        undgraph.add_edge(8, 1);
    }
    count = 8;
    cur_id = 8;
    repaint();
}

void MainWindow::on_for_min_clicked()
{
    colours.clear();
    dgraph.clear();
    undgraph.clear();
    if (is_directed) {
        for (int i = 1; i < 7; ++i) {
            dgraph.add_vertex(i);
        }
        dgraph.add_edge(1, 2, 12);
        dgraph.add_edge(2, 3, 1);
        dgraph.add_edge(1, 4, 5);
        dgraph.add_edge(4, 3, 10);
        dgraph.add_edge(6, 5, 2);
        dgraph.add_edge(5, 2, 1);
        dgraph.add_edge(1, 6, 7);
    }
    else {
        for (int i = 1; i < 7; ++i) {
            undgraph.add_vertex(i);
        }
        undgraph.add_edge(1, 2, 12);
        undgraph.add_edge(2, 3, 1);
        undgraph.add_edge(1, 4, 5);
        undgraph.add_edge(4, 3, 10);
        undgraph.add_edge(6, 5, 2);
        undgraph.add_edge(5, 2, 1);
        undgraph.add_edge(1, 6, 7);
    }
    count = 6;
    cur_id = 6;
    repaint();
}


void MainWindow::on_complete_graph_clicked()
{
    srand(time(NULL));
    colours.clear();
    dgraph.clear();
    undgraph.clear();
    bool ok;
    count = QInputDialog::getInt(this, "Complete graph", "Enter quantaty", 1, 1, 40, 1, &ok);
    if (ok) {
        is_directed = false;
        for (size_t i = 1; i <= count; i++) {
            undgraph.add_vertex(i);
            for (size_t j = 1; j < i; j++) {
                undgraph.add_edge(j, i);
            }
        }
        cur_id = count;
    }
    repaint();
}

