#ifndef __MEDUSA_HTML_H__
# define __MEDUSA_HTML_H__

# include <QFont>
# include <QString>

class MedusaHtml
{
public:
	static QString		getStyle(QFont const & font, QString const & color);
};

#endif // !__MEDUSA_HTML_H__
