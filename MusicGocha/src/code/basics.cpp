#include "basics.h"

//取两段字符串的中间字符串，失败返回false
bool bText_between(QString& ret, const QString& text, QString left, QString right, int from)
{
	const int start = text.indexOf(left, from);
	const int from_left = start + left.length();
	const int end = text.indexOf(right, from_left);
	if (start == -1 || end == -1)
		return false;
	const int n = end - from_left;
	ret = text.mid(from_left, n);
	return true;
}

bool bText_before(QString& ret, const QString& text, QString stop, int from)
{
	if (text.indexOf(stop, from) == -1)
		return false;
	ret = text.mid(from, text.indexOf(stop, from));
	return true;
}

bool bText_after(QString& ret, const QString& text, QString start, int from)
{
	if (text.indexOf(start, from) == -1)
		return false;
	ret = text.right(text.length() - text.indexOf(start, from) - start.length());
	return true;
}