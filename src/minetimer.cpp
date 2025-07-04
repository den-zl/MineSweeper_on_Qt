#include "minetimer.h"
#include <QGuiApplication>
#include <QStyleHints>


MineTimer::MineTimer(QWidget* parent /*= nullptr*/)
	: QLCDNumber(parent)
	, m_seconds(0)
{
	this->setDigitCount(4);
	this->display(0);
	this->setSegmentStyle(QLCDNumber::Flat);
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	this->setTheme(QGuiApplication::styleHints()->colorScheme());
}

void MineTimer::incrementTime()
{
	display(++m_seconds);
}

int MineTimer::time() const
{
	return m_seconds;
}

void  MineTimer::setTheme(Qt::ColorScheme colorScheme)
{
	if (colorScheme == Qt::ColorScheme::Dark)
	{
		this->setStyleSheet(".QLCDNumber { border: 2px inset #303030; background-color: black; color: red; }");
	}
	else
	{
		this->setStyleSheet(".QLCDNumber { border: 2px inset gray; background-color: black; color: red; }");
	}
}

QSize MineTimer::sizeHint() const
{
	return QSize(65, 35);
}