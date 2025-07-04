#pragma once

#include "tile.h"
#include "gameboard.h"
#include "mineCounter.h"
#include "minetimer.h"
#include "highScoreModel.h"
#include "gameStats.h"

#include <QAction>
#include <QMenu>
#include <QFrame>
#include <QPushButton>
#include <QMainWindow>
#include <QStateMachine>


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:

	MainWindow(QWidget* parent = nullptr);

	void setupMenus();

signals:

	void startGame();
	void victory();
	void defeat();
	void startNewGame();

protected slots:

	void onVictory();

protected:

	void closeEvent(QCloseEvent* event) override;

private:

	void setDifficulty(HighScore::Difficulty difficulty);
	void initialize();
	void setupStateMachine();
	void saveSettings();
	void loadSettings();

protected:

	void changeEvent(QEvent*) override;
	void setTheme(Qt::ColorScheme colorScheme);

private:

	QFrame*      mainFrame;
	GameBoard*   gameBoard;
	MineCounter* mineCounter;
	MineTimer*   mineTimer;
	QPushButton* newGame;

	QMenu*        gameMenu;
	QAction*      newGameAction;
	QMenu*        difficultyMenu;
	QActionGroup* difficultyActionGroup;
	QAction*      beginnerAction;
	QAction*      intermediateAction;
	QAction*      expertAction;
	QAction*      customAction;
	QAction*      highScoreAction;
	QAction*      statisticsAction;
	QAction*      exitAction;

	QMenu*   helpMenu;
	QAction* aboutAction;
	QAction* aboutQtAction;

	QTimer* gameClock;

	QStateMachine* m_machine;
	QState*        unstartedState;
	QState*        inProgressState;
	QState*        victoryState;
	QState*        defeatState;

	Tile* firstClicked;

	quint32 numRows;
	quint32 numCols;
	quint32 numMines;

	HighScore::Difficulty difficulty;
	GameStats             gameStats;

	QMap<HighScore::Difficulty, HighScoreModel> m_highScores;

};