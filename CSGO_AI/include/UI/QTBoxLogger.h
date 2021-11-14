#pragma once
#include <string>
#include <qtextedit.h>
#include <qmutex.h>
#include "Utility/Logger.h"

class QTBoxLogger : public Logger 
{
public:
	QTBoxLogger(QTextEdit* textEdit1, QTextEdit* textEdit2);
	void log(const std::string& str) override;
	void log_error(const std::string& str) override;
	void log_success(const std::string& str) override;

private:
	void log(QTextEdit* textEdit, Qt::GlobalColor color, const std::string& string);
	void log(QTextEdit* textEdit, Qt::GlobalColor color, const QString& string);

	QTextEdit* textEdit1 = nullptr;
	QTextEdit* textEdit2 = nullptr;
	QMutex mutex;
};