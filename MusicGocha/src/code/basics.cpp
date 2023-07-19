#include "basics.h"

//取两段字符串的中间字符串，失败返回false
bool bText_between(QString* ret, const QString* text, QString left, QString right, int from)
{
	const int start = text->indexOf(left, from);
	const int from_left = start + left.length();
	const int end = text->indexOf(right, from_left);
	if (start == -1 || end == -1)
		return false;
	const int n = end - from_left;
	*ret = text->mid(from_left, n);
	return true;
}