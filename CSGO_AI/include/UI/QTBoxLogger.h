#pragma once
#include <string>
#include <qtextedit.h>
#include <qmutex.h>
#include "Utility/Logger.h"

struct LogMessage 
{
	std::string message;
	Qt::GlobalColor color;
};

class QTBoxLogger : public Logger 
{
public:
	QTBoxLogger(std::initializer_list<QTextEdit*> text_edits);
	void log(const std::string& str) override;
	void log_error(const std::string& str) override;
	void log_success(const std::string& str) override;
	void update();

private:
	void push_message(const LogMessage& message);
	void log(QTextEdit* textEdit, Qt::GlobalColor color, const std::string& string);
	void log(QTextEdit* textEdit, Qt::GlobalColor color, const QString& string);

	QMutex mutex;
	std::vector<LogMessage> messages;
	std::vector<QTextEdit*> boxes;
};