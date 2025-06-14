#include "mainwindow.h"
#include "appinfo.h"
#include "gameboard.h"
#include "highScoreDialog.h"
#include "highScoreModel.h"
#include "mineCounter.h"
#include "minetimer.h"
#include "gameStatsDialog.h"

#include <QFrame>
#include <QGuiApplication>
#include <QInputDialog>
#include <QMenuBar>
#include <QSettings>
#include <QSignalTransition>
#include <QStatusBar>
#include <QStyleHints>
#include <QTimer>
#include <qfile.h>
#include <qstyle.h>
#include <QActionGroup>
#include <QAction>


MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, mainFrame(nullptr)
{
	this->setWindowIcon(QIcon(":/mine"));
	setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
	setupStateMachine();
	setupMenus();
	loadSettings();

	connect(this, &MainWindow::defeat, this,
			[this]()
			{
				newGame->setIcon(QIcon(":/emoji/injured"));
				gameStats.addStat(this->difficulty, GameStats::Loss, mineTimer->time());
			});
	connect(this, &MainWindow::victory, this,
			[this]()
			{
				newGame->setIcon(QIcon(":/emoji/sunglasses"));
				gameStats.addStat(this->difficulty, GameStats::Win, mineTimer->time());
			});


	this->layout()->setSizeConstraint(QLayout::SetFixedSize);
}

void MainWindow::setDifficulty(HighScore::Difficulty difficulty)
{
	this->difficulty = difficulty;

	switch (difficulty)
	{
	case HighScore::beginner:
		numRows	 = 9;
		numCols	 = 9;
		numMines = 10;
		beginnerAction->setChecked(true);
		break;
	case HighScore::intermediate:
		numRows	 = 16;
		numCols	 = 16;
		numMines = 40;
		intermediateAction->setChecked(true);
		break;
	case HighScore::expert:
		numRows	 = 16;
		numCols	 = 30;
		numMines = 99;
		expertAction->setChecked(true);
		break;
	case HighScore::custom:
		break;
	default:
		break;
	}

	initialize();
	adjustSize();
}

void MainWindow::initialize()
{
	QFrame* newMainFrame	= new QFrame(this);
	auto	mainFrameLayout = new QVBoxLayout;
	auto	infoLayout		= new QHBoxLayout;
	gameBoard				= new GameBoard(numRows, numCols, numMines, newMainFrame);
	mineCounter				= new MineCounter(newMainFrame);
	mineTimer				= new MineTimer(newMainFrame);
	newGame					= new QPushButton(newMainFrame);
	gameClock				= new QTimer(this);

	mineCounter->setNumMines(numMines);

	connect(gameBoard, &GameBoard::initialized, this, &MainWindow::startGame, Qt::UniqueConnection);
	connect(gameBoard, &GameBoard::flagCountChanged, mineCounter, &MineCounter::setFlagCount, Qt::UniqueConnection);
	connect(gameBoard, &GameBoard::victory, this, &MainWindow::victory, Qt::UniqueConnection);
	connect(gameBoard, &GameBoard::defeat, this, &MainWindow::defeat, Qt::UniqueConnection);

	newGame->setMinimumSize(35, 35);
	newGame->setIconSize(QSize(30, 30));
	newGame->setIcon(QIcon(":/emoji/smile"));
	connect(newGame, &QPushButton::clicked, this, &MainWindow::startNewGame, Qt::UniqueConnection);

	gameClock->setInterval(1000);
	connect(gameClock, &QTimer::timeout, mineTimer, &MineTimer::incrementTime, Qt::UniqueConnection);

	infoLayout->addWidget(mineCounter);
	infoLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding));
	infoLayout->addWidget(newGame);
	infoLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding));
	infoLayout->addWidget(mineTimer);

	mainFrameLayout->addLayout(infoLayout);
	mainFrameLayout->addWidget(gameBoard);

	newMainFrame->setLayout(mainFrameLayout);

	this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	this->setCentralWidget(newMainFrame);

	std::swap(mainFrame, newMainFrame);
	delete newMainFrame;
}

void MainWindow::setupStateMachine()
{
	m_machine = new QStateMachine;

	unstartedState	= new QState;
	inProgressState = new QState;
	victoryState	= new QState;
	defeatState		= new QState;

	unstartedState->addTransition(this, &MainWindow::startGame, inProgressState);

	inProgressState->addTransition(this, &MainWindow::victory, victoryState);
	inProgressState->addTransition(this, &MainWindow::defeat, defeatState);
	QSignalTransition* forfeitTransition = inProgressState->addTransition(this, &MainWindow::startNewGame, unstartedState);

	victoryState->addTransition(this, &MainWindow::startNewGame, unstartedState);

	defeatState->addTransition(this, &MainWindow::startNewGame, unstartedState);

	connect(unstartedState, &QState::entered, [this]() { initialize(); });

	connect(inProgressState, &QState::entered, [this]() { gameClock->start(); });

	connect(forfeitTransition, &QSignalTransition::triggered, [this]() { gameStats.addStat(this->difficulty, GameStats::Forfeit, mineTimer->time()); });

	connect(victoryState, &QState::entered,
			[this]()
			{
				gameClock->stop();
				onVictory();
			});

	connect(defeatState, &QState::entered, [this]() { gameClock->stop(); });

	m_machine->addState(unstartedState);
	m_machine->addState(inProgressState);
	m_machine->addState(victoryState);
	m_machine->addState(defeatState);

	m_machine->setInitialState(unstartedState);
	m_machine->start();
}

void MainWindow::onVictory()
{
	if (!m_highScores.contains(difficulty))
	{
		m_highScores.insert(difficulty, HighScoreModel(difficulty));
	}

	if (m_highScores[difficulty].isHighScore(mineTimer->time()))
	{
		auto name = QInputDialog::getText(this, tr("Congratulations!"), tr("You've earned a high score!<br>Please enter your name:"));
		m_highScores[difficulty].addHighScore(HighScore(name, difficulty, mineTimer->time(), QDateTime::currentDateTime()));
		highScoreAction->trigger();
	}
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	if (m_machine->configuration().contains(inProgressState))
	{
		gameStats.addStat(this->difficulty, GameStats::Forfeit, mineTimer->time());
	}
	saveSettings();
}

void MainWindow::setupMenus()
{
	gameMenu = new QMenu(tr("Game"));

	newGameAction = new QAction(tr("New Game"));
	newGameAction->setShortcut(QKeySequence(Qt::Key_F2));
	connect(newGameAction, &QAction::triggered, this, &MainWindow::startNewGame);

	difficultyMenu		  = new QMenu(tr("Difficulty"));
	difficultyActionGroup = new QActionGroup(difficultyMenu);

	beginnerAction = new QAction(tr("Beginner"), difficultyActionGroup);
	beginnerAction->setCheckable(true);
	QObject::connect(beginnerAction, &QAction::triggered, [this]() { setDifficulty(HighScore::beginner); });

	intermediateAction = new QAction(tr("Intermediate"), difficultyActionGroup);
	intermediateAction->setCheckable(true);
	connect(intermediateAction, &QAction::triggered, [this]() { setDifficulty(HighScore::intermediate); });

	expertAction = new QAction(tr("Expert"), difficultyActionGroup);
	expertAction->setCheckable(true);
	connect(expertAction, &QAction::triggered, [this]() { setDifficulty(HighScore::expert); });

	difficultyMenu->addAction(beginnerAction);
	difficultyMenu->addAction(intermediateAction);
	difficultyMenu->addAction(expertAction);

	highScoreAction = new QAction(tr("High Scores..."));
	connect(
		highScoreAction, &QAction::triggered, this,
		[this]()
		{
			QString difficulty = QVariant::fromValue(this->difficulty).toString();
			QString tabName	   = difficulty[0].toUpper() + difficulty.mid(1).toLower();

			auto* dialog = new HighScoreDialog(m_highScores, this);
			dialog->setActiveTab(tabName);
			dialog->exec();
			dialog->deleteLater();
		},
		Qt::QueuedConnection);

	statisticsAction = new QAction(tr("Statistics..."));
	connect(
		statisticsAction, &QAction::triggered, this,
		[this]()
		{
			QString difficulty = QVariant::fromValue(this->difficulty).toString();
			QString tabName	   = difficulty[0].toUpper() + difficulty.mid(1).toLower();

			auto* dialog = new GameStatsDialog(gameStats, this);
			dialog->setActiveTab(tabName);
			dialog->exec();
			dialog->deleteLater();
		},
		Qt::QueuedConnection);

	exitAction = new QAction(tr("Exit"));
	connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

	gameMenu->addAction(newGameAction);
	gameMenu->addSeparator();
	gameMenu->addMenu(difficultyMenu);
	gameMenu->addAction(highScoreAction);
	gameMenu->addAction(statisticsAction);
	gameMenu->addSeparator();
	gameMenu->addAction(exitAction);

	helpMenu = new QMenu(tr("Help"));

	aboutAction		   = new QAction(tr("About..."));
	aboutQtAction	   = new QAction(tr("About Qt..."));

	helpMenu->addAction(aboutAction);
	helpMenu->addAction(aboutQtAction);
	helpMenu->addSeparator();

	aboutAction->setIcon(QIcon(":/mine"));
	aboutQtAction->setIcon(this->style()->standardIcon(QStyle::SP_TitleBarMenuButton));

	this->menuBar()->addMenu(gameMenu);
}

void MainWindow::saveSettings()
{
	QSettings settings(APPINFO::organization, APPINFO::name);
	settings.setValue("difficulty", QVariant::fromValue(difficulty).toString());

	settings.beginWriteArray("High Scores", static_cast<int>(m_highScores.size()));

	int i = 0;
	for (const auto& model : m_highScores)
	{
		QByteArray data;
		QDataStream stream(&data, QIODevice::WriteOnly);
		stream << model;

		settings.setArrayIndex(i++);
		settings.setValue("difficulty", model.difficulty());
		settings.setValue("model", data);
	}
	settings.endArray();

	QByteArray data;
	QDataStream stream(&data, QIODevice::WriteOnly);
	stream << gameStats;
	settings.setValue("stats", data);
}

void MainWindow::loadSettings()
{
	QSettings settings(APPINFO::organization, APPINFO::name);
	setDifficulty(settings.value("difficulty").value<HighScore::Difficulty>());

	m_highScores.insert(HighScore::beginner, HighScoreModel{HighScore::beginner});
	m_highScores.insert(HighScore::intermediate, HighScoreModel{HighScore::intermediate});
	m_highScores.insert(HighScore::expert, HighScoreModel{HighScore::expert});

	const int size = settings.beginReadArray("High Scores");
	for (int i = 0; i < size; ++i)
	{
		settings.setArrayIndex(i);

		HighScoreModel model;
		QByteArray data = settings.value("model").toByteArray();
		QDataStream stream(&data, QIODevice::ReadOnly);
		stream >> model;

		m_highScores[model.difficulty()] = std::move(model);
	}
	settings.endArray();

	QByteArray data = settings.value("stats").toByteArray();
	QDataStream stream(&data, QIODevice::ReadOnly);
	stream >> gameStats;
}

void MainWindow::changeEvent(QEvent* event)
{
	if (event->type() == QEvent::ThemeChange || event->type() == QEvent::StyleChange)
	{
		this->setTheme(QGuiApplication::styleHints()->colorScheme());
	}
	QMainWindow::changeEvent(event);
}

void MainWindow::setTheme(Qt::ColorScheme colorScheme)
{
	gameBoard->setTheme(colorScheme);
	mineCounter->setTheme(colorScheme);
	mineTimer->setTheme(colorScheme);
}