#ifndef GAMESTATS_H
#define GAMESTATS_H


#include <highScore.h>
#include <statistics.h>

#include <QObject>

class GameStats
{
	Q_GADGET
public:

	enum GameType
	{
		Win,
		Loss,
		Forfeit,
	};
	Q_ENUM(GameType)

	struct GameStatsData
	{
		Statistics<quint64> wins;
		Statistics<quint64> losses;
		Statistics<quint64> forfeits;
		Statistics<quint64> gamesPlayed;
	};

	GameStats() = default;

	[[nodiscard]] quint64 played(HighScore::Difficulty difficulty) noexcept;
	[[nodiscard]] quint64 wins(HighScore::Difficulty difficulty) noexcept;
	[[nodiscard]] quint64 losses(HighScore::Difficulty difficulty) noexcept;
	[[nodiscard]] quint64 forfeits(HighScore::Difficulty difficulty) noexcept;
	[[nodiscard]] double winRate(HighScore::Difficulty difficulty) noexcept;
	[[nodiscard]] double lossRate(HighScore::Difficulty difficulty) noexcept;
	[[nodiscard]] double forfeitRate(HighScore::Difficulty difficulty) noexcept;
	[[nodiscard]] quint64 averageTimeToWin(HighScore::Difficulty difficulty) noexcept;
	[[nodiscard]] quint64 averageTimeToLoss(HighScore::Difficulty difficulty) noexcept;
	[[nodiscard]] quint64 averageTimeToForfeit(HighScore::Difficulty difficulty) noexcept;

public slots:

	void addStat(HighScore::Difficulty difficulty, GameStats::GameType type, quint64 time);

	friend QDataStream& operator<<(QDataStream& stream, const GameStats& stats);
	friend QDataStream& operator>>(QDataStream& stream, GameStats& stats);

private:

	std::map<HighScore::Difficulty, GameStatsData> stats;
};

#endif // GAMESTATS_H