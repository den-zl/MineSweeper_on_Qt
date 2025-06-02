
#pragma once

#include <QAbstractItemModel>
#include <QDataStream>
#include <QVector>
#include "highScore.h"


class HighScoreModel : public QAbstractItemModel
{
public:

	enum Column
	{
		Name  = 0,
		Score = 1,
		Date  = 2,
	};

	Q_ENUM(Column);


	HighScoreModel() = default;
	explicit HighScoreModel(HighScore::Difficulty difficulty, QObject* parent = nullptr);
	HighScoreModel(const HighScoreModel& other);
	HighScoreModel(HighScoreModel&& other) noexcept;
	HighScoreModel& operator=(const HighScoreModel& other);
	HighScoreModel& operator=(HighScoreModel&& other) noexcept;

	void addHighScore(const HighScore& score);
	void setDifficulty(HighScore::Difficulty difficulty);
	void setHighScores(const QVector<HighScore>& scores);

	[[nodiscard]] HighScore::Difficulty     difficulty() const;
	[[nodiscard]] const QVector<HighScore>& highScores() const;
	[[nodiscard]] bool                      isHighScore(int time) const;

	[[nodiscard]] QModelIndex   index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	[[nodiscard]] QModelIndex   parent(const QModelIndex& child) const override;
	[[nodiscard]] int           rowCount(const QModelIndex& parent = QModelIndex()) const override;
	[[nodiscard]] int           columnCount(const QModelIndex& parent = QModelIndex()) const override;
	[[nodiscard]] QVariant      data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	[[nodiscard]] Qt::ItemFlags flags(const QModelIndex& index) const override;
	[[nodiscard]] QVariant      headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:

	HighScore::Difficulty m_difficulty{HighScore::beginner};
	QVector<HighScore>    m_highScores;
};

Q_DECLARE_METATYPE(HighScoreModel);

QDataStream& operator<<(QDataStream& out, const HighScoreModel&);
QDataStream& operator>>(QDataStream& in, HighScoreModel&);