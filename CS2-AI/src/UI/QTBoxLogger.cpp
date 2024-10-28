#include "UI/QTBoxLogger.h"

QTBoxLogger::QTBoxLogger(std::initializer_list<QTextEdit*> text_edits)
{
	boxes = text_edits;
}

void QTBoxLogger::log(const std::string& str)
{
	push_message({ str, Qt::black });
}

void QTBoxLogger::log_error(const std::string& str)
{
	push_message({ str, Qt::red });
}

void QTBoxLogger::log_success(const std::string& str)
{
	push_message({ str, Qt::green });
}

void QTBoxLogger::push_message(const LogMessage& message)
{
	QMutexLocker lock(&mutex);
	messages.push_back(message);
}

void QTBoxLogger::update() // Should only be called from the owning thread
{
	QMutexLocker lock(&mutex);
	for (const auto& message : messages) 
	{
		for (auto textEdit : boxes) 
		{
			log(textEdit, message.color, message.message);
		}
	}
	messages.clear();
}

void QTBoxLogger::log(QTextEdit* textEdit, Qt::GlobalColor color, const std::string& string)
{
	log(textEdit, color, QString::fromStdString(string));
}

void QTBoxLogger::log(QTextEdit* textEdit, Qt::GlobalColor color, const QString& string)
{
	textEdit->setTextColor(color);
	textEdit->append(string);
}
