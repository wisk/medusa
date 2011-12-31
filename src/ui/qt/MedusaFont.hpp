#ifndef __MEDUSA_FONT_H__
# define __MEDUSA_FONT_H__

# include <QFontMetrics>
# include <QFont>
# include <QString>
# include <QSize>

class MedusaFont
{
public:
	static QSize getSize(QFont const & font, QString const & text);
};

#endif // !__MEDUSA_FONT_H__
