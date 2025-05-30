#ifndef GAMESTATSDIALOG_H
#define GAMESTATSDIALOG_H

#include "gameStats.h"

#include <QDialog>
#include <QLabel>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QGroupBox>

class GameStatsDialog : public QDialog
{
	Q_OBJECT
public:

	explicit GameStatsDialog(GameStats stats, QWidget *parent = nullptr);

	void setActiveTab(const QString& activeTab) const;

private:

	GameStats m_stats;
	QTabWidget *m_tabWidget;
};

#endif //GAMESTATSDIALOG_H