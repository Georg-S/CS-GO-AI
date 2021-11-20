#include "UI/QTBoxLogger.h"

QTBoxLogger::QTBoxLogger(std::initializer_list<QTextEdit*> text_edits)
{
	boxes = text_edits;
}

void QTBoxLogger::log(const std::string& str)
{
	for(auto box : boxes)
		log(box, Qt::black, str);
}

void QTBoxLogger::log_error(const std::string& str)
{
	for (auto box : boxes)
		log(box, Qt::red, str);
}

void QTBoxLogger::log_success(const std::string& str)
{
	for (auto box : boxes)
		log(box, Qt::green, str);
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
