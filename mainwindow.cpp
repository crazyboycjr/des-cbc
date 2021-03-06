#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	srand(unsigned(time(0)));

    ui->setupUi(this);
    this->setFixedSize(this->size());
    ui->plainTextEdit->document()->setDefaultFont(QFont("monospace"));
    ui->plainTextEdit_2->document()->setDefaultFont(QFont("monospace"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QDir dir;
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), dir.currentPath());
    qDebug() << fileName;
    //this->encryptFile = fileName; // not needed
    ui->lineEdit->setText(fileName);
    showText(fileName, ui->plainTextEdit);
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    qDebug() << "encryptFile" << arg1;
    this->encryptFile = arg1;
}

void MainWindow::on_pushButton_2_clicked()
{
    QDir dir;
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), dir.currentPath());
    qDebug() << fileName;
    ui->lineEdit_2->setText(fileName);
    showText(fileName, ui->plainTextEdit_2);
}

void MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{
    qDebug() << "decryptFile" << arg1;
    this->encryptFile = arg1;
}

void MainWindow::on_radioButton_2_clicked()
{
    this->keyIsHex = true;
}

void MainWindow::on_radioButton_clicked()
{
    this->keyIsHex = false;
}

bool MainWindow::testFile(QString fileName, QPlainTextEdit *plainTextWidget) {
	char errMsg[] = "filename invalid or not writeable.";
	qDebug() << QFile::exists(fileName);

	if (!(QFile::exists(fileName)
			&& (QFile::permissions(fileName) | QFileDevice::WriteUser))) {
		qDebug() << QString(errMsg);

		if (plainTextWidget != nullptr) {
			plainTextWidget->document()->setPlainText(errMsg);
		}
		return false;
	}
	return true;
}

void MainWindow::showText(QString fileName, QPlainTextEdit *plainTextWidget) {
	if (!QFile::exists(fileName)) {
		plainTextWidget->document()->setPlainText("");
		return;
	}

    FILE *fin = fopen(fileName.toStdString().c_str(), "rb");
    if (fin == nullptr) {
        qDebug() << "fin is null";
        return;
    }
    plainTextWidget->document()->setPlainText("");
    int bound = 128, count = 0;
    char str[16];
    char buf[16 * 128];

	fseek(fin, 0, SEEK_END);
	long sz = ftell(fin);
	fseek(fin, 0, SEEK_SET);

    while (fread(str, 8, 1, fin), count++ < bound) {
        int len = 0;
        for (int i = 0; i < 8; i++)
            len += sprintf(buf + len, "%.2hhx ", str[i]);
        len += sprintf(buf + len, "|");
        for (int i = 0; i < 8; i++)
            len += sprintf(buf + len, "%c", 33 < str[i] && str[i] < 127 ? str[i] : '.');
        len += sprintf(buf + len, "|\n");
        plainTextWidget->document()->setPlainText(
                    plainTextWidget->toPlainText() + QString(buf));
		if ((sz -= 8) <= 0) break;
    }
}

void MainWindow::on_lineEdit_returnPressed()
{
    QString fileName = ui->lineEdit->text();
    qDebug() << fileName;
    showText(fileName, ui->plainTextEdit);
}

void MainWindow::on_lineEdit_2_returnPressed()
{
    QString fileName = ui->lineEdit_2->text();
    qDebug() << fileName;
    showText(fileName, ui->plainTextEdit_2);
}

void MainWindow::on_lineEdit_3_textEdited(const QString &arg1)
{
    QString tempStr = "";
    if (this->keyIsHex) {
        int cnt = 0;
        for (int i = 0; i < arg1.length(); i++) {
            QChar c = arg1[i];
            if (!c.isSpace()) {
                cnt ^= 1;
                tempStr.push_back(c);
                if (cnt == 0 && i != arg1.length() - 1) tempStr.push_back(' ');
            }
        }
    ui->lineEdit_3->setText(tempStr);
    }
}

uint64_t MainWindow::getKey() {
    char key_str[8] = {0};
    uint64_t key = 0;

    QString keyQStr = ui->lineEdit_3->text();

    if (this->keyIsHex) {

        keyQStr.replace(" ", "");
        keyQStr.truncate(16);

        qDebug() << keyQStr.length();
        for (int i = 0; i < keyQStr.length(); i++) {
            QChar ch = keyQStr[i];
            int num = ch.isDigit() ? ch.unicode() - 48 : ch.toLower().unicode() - 'a' + 10;
            if (i & 1) key_str[i / 2] |= num;
            else key_str[i / 2] |= num << 4;
        }

        key = *(uint64_t *)key_str;
        key = __builtin_bswap64(key);
    } else {
        keyQStr.truncate(8);
        for (int i = 0; i < keyQStr.length(); i++) {
            key_str[i] = keyQStr[i].unicode();
        }
        key = *(uint64_t *)key_str;
    }

    qDebug() << keyQStr;
    qDebug() << key;
    return key;
}

void MainWindow::on_pushButton_4_clicked()
{
    uint64_t key = getKey();
    QString inFile = ui->lineEdit->text();
    QString outFile = ui->lineEdit_2->text();

    qDebug() << "ready to encrypt" << inFile;
    qDebug() << "output file is set to " << outFile;

	if (!testFile(inFile, ui->plainTextEdit))
		return;
	if (outFile.length() == 0)
		return;

	/* Generate initial vector */
	uint64_t iv = (uint64_t)rand() << 32 | rand();

    encrypt(inFile.toStdString(), outFile.toStdString(), key, iv);
	showText(outFile, ui->plainTextEdit_2);
}

void MainWindow::on_pushButton_5_clicked()
{
    uint64_t key = getKey();
    QString inFile = ui->lineEdit_2->text();
    QString outFile = ui->lineEdit->text();

    qDebug() << "ready to decrypt" << inFile;
    qDebug() << "output file is set to " << outFile;
	
	if (!testFile(inFile, ui->plainTextEdit))
		return;
	if (outFile.length() == 0)
		return;

    decrypt(inFile.toStdString(), outFile.toStdString(), key);
	showText(outFile, ui->plainTextEdit);
}
