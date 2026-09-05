#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientconnection.h"
#include <QMessageBox>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ClientConnection::getInstance(), &ClientConnection::responseReceived, this, &MainWindow::onResponse);
    
    if (ClientConnection::getInstance()->getRole() != "admin") {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->tabAdmin));
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onResponse(const QByteArray& res) {
    QString resp = QString::fromUtf8(res).trimmed();
    QStringList parts = resp.split("&");
    
    if (resp.startsWith("vigenere_res")) {
        ui->vigRes->setText("Result: " + (parts.size() > 1 ? parts[1] : ""));
    } else if (resp.startsWith("sha512_res")) {
        ui->shaRes->setText("Result: " + (parts.size() > 1 ? parts[1] : ""));
    } else if (resp.startsWith("bisection_res")) {
        ui->bisRes->setText("Result: " + (parts.size() > 1 ? parts[1] : ""));
    } else if (resp.startsWith("graph_res")) {
        ui->graphRes->setText("Result: " + (parts.size() > 1 ? parts[1] : ""));
    } else if (resp.startsWith("admin_res")) {
        ui->adminRes->setText("Status: " + (parts.size() > 1 ? parts[1] : ""));
    } else if (resp.startsWith("error")) {
        QMessageBox::warning(this, "Error", parts.size() > 1 ? parts[1] : "Unknown error");
    }
}

void MainWindow::on_vigBtn_clicked() {
    QString req = "vigenere&" + ui->vigText->text() + "&" + ui->vigKey->text();
    ClientConnection::getInstance()->sendRequest(req.toUtf8());
}

void MainWindow::on_vigDecBtn_clicked() {
    QString req = "vigenere_dec&" + ui->vigText->text() + "&" + ui->vigKey->text();
    ClientConnection::getInstance()->sendRequest(req.toUtf8());
}

void MainWindow::on_shaBtn_clicked() {
    QString req = "sha512&" + ui->shaText->text();
    ClientConnection::getInstance()->sendRequest(req.toUtf8());
}

void MainWindow::on_bisBtn_clicked() {
    QString req = "bisection&" + ui->bisTarget->text() + "&" + ui->bisA->text() + "&" + ui->bisB->text();
    ClientConnection::getInstance()->sendRequest(req.toUtf8());
}

void MainWindow::on_graphBtn_clicked() {
    QString edgesText = ui->graphEdges->text();
    edgesText.replace(",", " ");
    QStringList parts = edgesText.split(" ", Qt::SkipEmptyParts);
    
    QString req = "graph&" + ui->graphStart->text() + "&" + ui->graphEnd->text() + "&" + ui->graphV->text();
    for (const QString& part : parts) {
        req += "&" + part;
    }
    
    ClientConnection::getInstance()->sendRequest(req.toUtf8());
}

void MainWindow::on_adminBtn_clicked() {
    QString req = "admin&setrole&" + ui->adminUser->text() + "&" + ui->adminRole->text();
    ClientConnection::getInstance()->sendRequest(req.toUtf8());
}
