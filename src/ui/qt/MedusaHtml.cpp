#include "MedusaHtml.hpp"

QString			MedusaHtml::getStyle(QFont const & font, QString const & color)
{
	QString		style = "style=\"color: %1; font-family: %2; font-size: %3pt; font-weight: %4; text-decoration: %5; font-style: %6;\"";

	style = style.arg(color).arg(font.family()).arg(font.pointSize());

	if (font.bold())
		style = style.arg("bold");
	else
		style = style.arg("normal");

	QString decoration = "";

	if (font.underline())
		decoration += "underline ";

	if (font.overline())
		decoration += "overline ";

	if (font.strikeOut())
		decoration += "line-through";

	if (decoration.isEmpty())
		style.arg("none");
	else
		style = style.arg(decoration);

	if (font.italic())
		style = style.arg("italic");
	else
		style = style.arg("normal");

	return (style);
}