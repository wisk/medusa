#include "MedusaFont.hpp"

QSize		MedusaFont::getSize(QFont const & font, QString const & text)
{
	QFontMetrics	metrics(font);

	return (metrics.size(Qt::TextSingleLine, text));
}