#include "debugtrace.h"

void debugtrace::printToFile(const QString& str)
{
	if (isValid())
	{
		fout << str;
		fout.flush();
	}
}
void debugtrace::printToQDebug(const QString& str)
{
	if (isValid())
	{
		qDebugFullMessageHolder += str;
	}
}

void debugtrace::printToString(const QString& str)
{
	if (isValid())
	{
		sout << str;
	}
}

void debugtrace::printToAll(const QString& str)
{
	if (isValid())
	{
		fout << str;
		fout.flush();
		qDebug() << str;
		sout << str;
	}
}
void debugtrace::printToConsole(const QString& str)
{
	if (isValid())
	{
		std::cout << str.toStdString();
		std::cout.flush();
	}
}
void debugtrace::printToSome(const QString& str)
{
	if (isValid())
	{
		for (int i = 0; i < omode_united.size(); ++i)
		{
			(*this.*(omode_united.at(i)))(str);
		}
	}
}

void debugtrace::printToConsBuff(const QString& str)
{
	if (isValid())
	{
		buffer << str.toStdString();
	}
}

bool debugtrace::isValid()
{
	if (blackaswhite)
	{
		for (int i = 0; i < bllen; ++i)
		{
			if (msgPriorityLvl == blacklist[i])
				return true;
		}
	}
	if (priorityLvl == all || msgPriorityLvl <= priorityLvl)
	{
		for (int i = 0; i < bllen; ++i)
		{
			if (msgPriorityLvl == blacklist[i])
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

void debugtrace::flushQDebug()
{
	qDebugFullMessageHolder.replace('\n', ' ');
	qDebug() << qDebugFullMessageHolder;
	qDebugFullMessageHolder.clear();
}

void debugtrace::flushBuffer()
{
	std::cout << buffer.str();
	buffer.str("");
}

debugtrace::debugtrace(DebugPriority priority, OutputMode mode,
	OutputMode  onlyOutputTo[], int oolen,
	DebugPriority  Blacklist[], int blen, bool blackAswhite, bool nosp)
	:outfile(FOUTPATH), fout(&outfile), priorityLvl(priority),
	msgPriorityLvl(notImportantMessage),
	omode(mode), outstring("debugtrace:\n"), sout(&outstring)
	, outmethod(&debugtrace::printToQDebug), blacklist(Blacklist), bllen(blen),
	omode_united(), blackaswhite(blackAswhite), nospaces(nosp)
{
	outfile.open(QIODevice::WriteOnly | QIODevice::Text);
	fout.setDevice(&outfile);
	changeOutputMode(omode, onlyOutputTo, oolen);
}
debugtrace::debugtrace(QString outpath, DebugPriority priority, OutputMode mode)
: outfile(outpath), fout(&outfile), priorityLvl(priority),
msgPriorityLvl(notImportantMessage),
omode(mode), outstring("debugtrace:\n"), sout(&outstring)
, outmethod(&debugtrace::printToQDebug), blacklist(), bllen(),
omode_united(), blackaswhite(), nospaces()
{
	outfile.open(QIODevice::WriteOnly | QIODevice::Text);
	fout.setDevice(&outfile);
	changeOutputMode(mode,&mode, 0);

}
debugtrace& debugtrace::operator<<(const DebugPriority pri)
{
	msgPriorityLvl = pri;
	return *this;
}

debugtrace& debugtrace::operator<<(const int intout)
{
	(*this.*outmethod)(QString::number(intout));
	return *this;
}

debugtrace& debugtrace::operator<<(const long long int val)
{
	(*this.*outmethod)(QString::number(val));
	return *this;
}

debugtrace& debugtrace::operator<<(const char* msg)
{
	QString temp(msg);
	if (nospaces)
	{
		temp.replace('\n', ' ');
	}
	(*this.*outmethod)(temp);
	return *this;
}

debugtrace& debugtrace::operator<<(const QString& msg)
{
	QString tmp(msg);
	if (nospaces)
	{
		tmp.replace('\n', ' ');
	}
	(*this.*outmethod)(tmp);
	return *this;
}

debugtrace& debugtrace::operator<<(const bool value)
{
	(*this.*outmethod)((value) ? "true" : "false");
	return *this;
}

debugtrace& debugtrace::operator<<(const double dout)
{
	(*this.*outmethod)(QString::number(dout));
	return *this;
}

debugtrace& debugtrace::operator<<(const std::exception& ex)
{
	(*this.*outmethod)(ex.what());
	return *this;
}
debugtrace& debugtrace::operator <<(const char c)
{
	if (omode == qDeb || omode == toall)
	{
		if (c == '\n')
		{
			flushQDebug();
			if (!nospaces)(*this.*outmethod)(QString() + c);
			return *this;
		}
	}
	if (omode == toall || omode == buffConsole)
	{
		if (c == '\n')
		{
			flushBuffer();
			if (!nospaces) (*this.*outmethod)(QString() + c);
			return *this;
		}
	}
	(*this.*outmethod)(QString() + c);
	return *this;
}

debugtrace& debugtrace::operator<<(const QPoint& p)
{
	(*this.*outmethod)(QString::number(p.x()) + " " + QString::number(p.y()));
	return *this;
}

debugtrace& debugtrace::operator<<(const std::string& str)
{
	(*this.*outmethod)(QString::fromStdString(str));
	return *this;
}

debugtrace& debugtrace::operator<<(const std::wstring& str)
{
	(*this.*outmethod)(QString::fromStdWString(str));
	return *this;
}

debugtrace& debugtrace::operator<<(const size_t sz)
{
	(*this.*outmethod)(QString::number(sz));
	return *this;
}

debugtrace& debugtrace::operator<<(const unsigned int val)
{
	(*this.*outmethod)(QString::number(val));
	return *this;
}

void debugtrace::changeOutputMode(const OutputMode mode, OutputMode v[], int oolen)
{
	switch (mode)
	{
	case toall:
		outmethod = &debugtrace::printToAll;
		break;
	case file:
		outmethod = &debugtrace::printToFile;
		break;
	case qStr:
		outmethod = &debugtrace::printToString;
		break;
	case qDeb:
		outmethod = &debugtrace::printToQDebug;
		break;
	case Cons:
		outmethod = &debugtrace::printToConsole;
		break;
	case buffConsole:
		outmethod = &debugtrace::printToConsBuff;
		break;
	case some_united:
		outmethod = &debugtrace::printToSome;
		for (int i = 0; i < oolen; ++i)
		{
			switch (v[i])
			{
			case file:
				omode_united.push_back(&debugtrace::printToFile);
				break;
			case qStr:
				omode_united.push_back(&debugtrace::printToString);
				break;
			case qDeb:
				omode_united.push_back(&debugtrace::printToQDebug);
				break;
			case Cons:
				omode_united.push_back(&debugtrace::printToConsole);
				break;
			case buffConsole:
				omode_united.push_back(&debugtrace::printToConsBuff);
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
}
QString debugtrace::getCurrentString()
{
	return outstring;
}

static OutputMode onlyOutputTo[] = { file, qDeb };
static int ootolen = 2;
static DebugPriority blacklist[] = { methodDataSnapshot }; // this blacklist removes type of messages
static int blackllen = 1;
debugtrace detrace(all, file, onlyOutputTo, ootolen, blacklist, blackllen, true, true);