#include "QTBoxLogger.h"

QTBoxLogger::QTBoxLogger(QTextEdit* textEdit1, QTextEdit* textEdit2) : textEdit1(textEdit1), textEdit2(textEdit2)
{
}

void QTBoxLogger::log(const std::string& str)
{
	log(textEdit1, Qt::black, str);
	log(textEdit2, Qt::black, str);
}

void QTBoxLogger::log_error(const std::string& str)
{
	log(textEdit1, Qt::red, str);
	log(textEdit2, Qt::red, str);
}

void QTBoxLogger::log_success(const std::string& str)
{
	log(textEdit1, Qt::green, str);
	log(textEdit2, Qt::green, str);
}

void QTBoxLogger::log(QTextEdit* textEdit, Qt::GlobalColor color, const std::string& string)
{
	log(textEdit, color, QString::fromStdString(string));
}

void QTBoxLogger::log(QTextEdit* textEdit, Qt::GlobalColor color, const QString& string)
{
	mutex.lock();
	textEdit->setTextColor(color);
	textEdit->append(string);
	mutex.unlock();
}
