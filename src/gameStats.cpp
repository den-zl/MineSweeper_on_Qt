#include "gameStats.h"


void GameStats::addStat(HighScore::Difficulty difficulty, GameType type, quint64 time)
{
	switch (type)
	{
	case Forfeit:
		stats[difficulty].forfeits.insert(time);
		break;
	case Loss:
		stats[difficulty].losses.insert(time);
		break;
	case Win:
		stats[difficulty].wins.insert(time);
		break;
	}
	stats[difficulty].gamesPlayed.insert(time);
}

QDataStream& operator<<(QDataStream& stream, const GameStats& stats)
{
	stream << (quint64)stats.stats.size();
	for (auto it = stats.stats.begin(); it != stats.stats.end(); ++it)
	{
		stream << it->first << it->second.wins << it->second.losses << it->second.forfeits << it->second.gamesPlayed;
	}
	return stream;
}

QDataStream& operator>>(QDataStream& stream, GameStats& stats)
{
	if (stream.status() != QDataStream::Ok)
		return stream;

	stats.stats.clear();
	quint64 size;
	stream >> size;

	for (quint64 i = 0; i < size; ++i)
	{
		HighScore::Difficulty	 difficulty;
		GameStats::GameStatsData data;

		stream >> difficulty;
		if (stream.status() != QDataStream::Ok)
			return stream;

		stream >> data.wins >> data.losses >> data.forfeits >> data.gamesPlayed;
		if (stream.status() != QDataStream::Ok)
			return stream;

		stats.stats[difficulty] = data;
	}

	return stream;
}

quint64 GameStats::played(HighScore::Difficulty difficulty) noexcept { return this->stats[difficulty].gamesPlayed.count(); }

quint64 GameStats::wins(HighScore::Difficulty difficulty) noexcept { return this->stats[difficulty].wins.count(); }

quint64 GameStats::losses(HighScore::Difficulty difficulty) noexcept { return this->stats[difficulty].losses.count(); }

quint64 GameStats::forfeits(HighScore::Difficulty difficulty) noexcept { return this->stats[difficulty].forfeits.count(); }

double GameStats::winRate(HighScore::Difficulty difficulty) noexcept { return 100.0 * (double)wins(difficulty) / (double)played(difficulty); }

double GameStats::lossRate(HighScore::Difficulty difficulty) noexcept { return 100.0 * (double)losses(difficulty) / (double)played(difficulty); }

double GameStats::forfeitRate(HighScore::Difficulty difficulty) noexcept { return 100.0 * (double)forfeits(difficulty) / (double)played(difficulty); }

quint64 GameStats::averageTimeToWin(HighScore::Difficulty difficulty) noexcept { return this->stats[difficulty].wins.mean(); }

quint64 GameStats::averageTimeToLoss(HighScore::Difficulty difficulty) noexcept { return this->stats.at(difficulty).losses.mean(); }

quint64 GameStats::averageTimeToForfeit(HighScore::Difficulty difficulty) noexcept { return this->stats.at(difficulty).forfeits.mean(); }