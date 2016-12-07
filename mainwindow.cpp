#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QMessageBox>

/**
 * @brief MainWindow::MainWindow é o construtor da classe.
 * @param parent
 */

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  socket = new QTcpSocket(this);
}

/**
 * @brief MainWindow::getData Recebe os IPs conectados ao servidor
 *
*/

void MainWindow::getData(){
  QString str;
  QByteArray array;
  QStringList list;
  QDateTime datetime;

  qDebug() << "to get data...";

  if(socket->state() == QAbstractSocket::ConnectedState){
    if(socket->isOpen()){

      qDebug() << "reading...";

      socket->write("list");
      socket->waitForBytesWritten();
      socket->waitForReadyRead();

      qDebug() << socket->bytesAvailable();

      str = socket->readLine().replace("\n","").replace("\r",""); //pega o ip conectado

      ui->listWidget->addItem(str); //para imprimir no listWidget o IP conectado

    }
  }
}

/**
 * @brief MainWindow::~MainWindow é o destrutor da classe.
 */
MainWindow::~MainWindow()
{
  delete socket;
  delete ui;
}

/**
 * @brief MainWindow::on_pushButton_Connect_clicked Conecta ao servidor quando o botão é clicado
 */
void MainWindow::on_pushButton_Connect_clicked()
{
    socket->connectToHost(ui->ip->text(),1234);
    if(socket->waitForConnected(3000)){
      qDebug() << "Connected";
    }
    else{
      qDebug() << "Disconnected";
    }

    getData();
}

/**
 * @brief MainWindow::on_pushButton_Disconnect_clicked Desconecta do servidor a encerrar a aplicacao
 */
void MainWindow::on_pushButton_Disconnect_clicked()
{
    close();
}

/**
 * @brief MainWindow::on_pushButton_Start_clicked Seleciona os IPs conectados ao servidor e imprime
 */
void MainWindow::on_pushButton_Start_clicked()
{
    QString sele = ui->listWidget->currentItem()->text();
    QMessageBox::information(NULL, "Item", "Item selecionado: "+sele);
//=========================================================================================
    QString str;
    QByteArray array;
    QStringList list;
    QDateTime datetime;

    qDebug() << "to get data...";

    if(socket->state() == QAbstractSocket::ConnectedState){

        if(socket->isOpen()){
        qDebug() << "reading...";

        socket->write("get 127.0.0.1\r\n");
        socket->waitForBytesWritten();
        socket->waitForReadyRead();

        qDebug() << socket-> bytesAvailable();

        while(socket-> bytesAvailable()){
          str = socket-> readLine().replace("\n","").replace("\r","");
          list = str.split(" ");
          if(list.size() == 2){
            datetime.fromString(list.at(0),Qt::ISODate);
            str = list.at(1);
            qDebug() << datetime << ": " << str;
          }
        }
      }
    }
}
