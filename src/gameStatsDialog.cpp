#include "gameStatsDialog.h"
#include <QSizePolicy>
#include <QTabBar>
#include <utility>

GameStatsDialog::GameStatsDialog(GameStats stats, QWidget* parent)
    : QDialog{parent}
, m_stats{std::move(stats)}
, m_tabWidget{new QTabWidget(this)}
{
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addWidget(m_tabWidget);

	for (auto difficulty : {HighScore::beginner, HighScore::intermediate, HighScore::expert})
	{
		QWidget* tab = new QWidget();
		QGridLayout* tabLayout = new QGridLayout(tab);

		int row = 0;
		tabLayout->addWidget(new QLabel("Games Played:", tab), ++row, 0);
		tabLayout->addWidget(new QLabel(QString::number(m_stats.played(difficulty)), this), row, 1, Qt::AlignRight);
		tabLayout->addWidget(new QFrame(this), ++row, 0);
		tabLayout->addWidget(new QLabel("Wins:", this), ++row, 0);
		tabLayout->addWidget(new QLabel(QString::number(m_stats.wins(difficulty)), this), row, 1, Qt::AlignRight);
		tabLayout->addWidget(new QLabel("Win Rate:", this), ++row, 0);
		tabLayout->addWidget(new QLabel(QString::number(m_stats.winRate(difficulty), 'f', 1), this), row, 1, Qt::AlignRight);
		tabLayout->addWidget(new QLabel("%", this), row, 2);
		tabLayout->addWidget(new QLabel("Avg. Time to Win:", this), ++row, 0);
		tabLayout->addWidget(new QLabel(QString::number(m_stats.averageTimeToWin(difficulty)), this), row, 1, Qt::AlignRight);
		tabLayout->addWidget(new QLabel("seconds", this), row, 2);
		tabLayout->addWidget(new QFrame(this), ++row, 0);
		tabLayout->addWidget(new QLabel("Losses:", this), ++row, 0);
		tabLayout->addWidget(new QLabel(QString::number(m_stats.losses(difficulty)), this), row, 1, Qt::AlignRight);
		tabLayout->addWidget(new QLabel("Loss Rate:", this), ++row, 0);
		tabLayout->addWidget(new QLabel(QString::number(m_stats.lossRate(difficulty), 'f', 1), this), row, 1, Qt::AlignRight);
		tabLayout->addWidget(new QLabel("%", this), row, 2);
		tabLayout->addWidget(new QLabel("Avg. Time to Loss:", this), ++row, 0);
		tabLayout->addWidget(new QLabel(QString::number(m_stats.averageTimeToLoss(difficulty)), this), row, 1, Qt::AlignRight);
		tabLayout->addWidget(new QLabel("seconds", this), row, 2);
		tabLayout->addWidget(new QFrame(this), ++row, 0);
		tabLayout->addWidget(new QLabel("Forfeits:", this), ++row, 0);
		tabLayout->addWidget(new QLabel(QString::number(m_stats.forfeits(difficulty)), this), row, 1, Qt::AlignRight);
		tabLayout->addWidget(new QLabel("Forfeit Rate:", this), ++row, 0);
		tabLayout->addWidget(new QLabel(QString::number(m_stats.forfeitRate(difficulty), 'f', 1), this), row, 1, Qt::AlignRight);
		tabLayout->addWidget(new QLabel("%", this), row, 2);
		tabLayout->addWidget(new QLabel("Avg. Time to Forfeit:", this), ++row, 0);
		tabLayout->addWidget(new QLabel(QString::number(m_stats.averageTimeToForfeit(difficulty)), this), row, 1, Qt::AlignRight);
		tabLayout->addWidget(new QLabel("seconds", this), row, 2);

		m_tabWidget->addTab(tab, difficulty == HighScore::beginner ? "Beginner" :
				   difficulty == HighScore::intermediate ? "Intermediate" : "Expert");
	    }

	m_tabWidget->tabBar()->setUsesScrollButtons(false);
	this->setWindowTitle("Statistics");
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	this->adjustSize();
}

void GameStatsDialog::setActiveTab(const QString& activeTab) const
{
	for (int index = 0; index < m_tabWidget->count(); index++)
	{
		if (m_tabWidget->tabText(index) == activeTab)
		{
			m_tabWidget->setCurrentIndex(index);
			return;
		}
	}
}