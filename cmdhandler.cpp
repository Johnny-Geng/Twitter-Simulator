#include "cmdhandler.h"
#include "util.h"
using namespace std;

QuitHandler::QuitHandler()
{

}

QuitHandler::QuitHandler(Handler* next)
  : Handler(next)
{

}

bool QuitHandler::canHandle(const std::string& cmd) const
{
	return cmd == "QUIT";

}

Handler::HANDLER_STATUS_T QuitHandler::process(TwitEng* eng, std::istream& instr) const
{
	eng->dumpFeeds();
	return HANDLER_QUIT;
}

AndHandler::AndHandler()
{}

AndHandler::AndHandler(Handler* next)
	: Handler(next)
{}

bool AndHandler::canHandle(const std::string& cmd) const
{
	return cmd == "AND";
}

Handler::HANDLER_STATUS_T AndHandler::process(TwitEng* eng, std::istream& instr) const
{
	vector<string> hashterm;
	string temp;
	while (instr >> temp)
	{
		hashterm.push_back(temp);
	}
	vector<Tweet*> tweetResult = eng->search(hashterm,0);
	displayHits(tweetResult);
	return HANDLER_OK;
}

OrHandler::OrHandler()
{}

OrHandler::OrHandler(Handler* next)
	: Handler(next)
{}

bool OrHandler::canHandle(const std::string& cmd) const
{
	return cmd == "OR";
}

Handler::HANDLER_STATUS_T OrHandler::process(TwitEng* eng, std::istream& instr) const
{
	vector<string> hashterm;
	string temp;
	while (instr >> temp)
	{
		hashterm.push_back(temp);
	}
	vector<Tweet*> tweetResult = eng->search(hashterm,1);
	displayHits(tweetResult);
	return HANDLER_OK;
}

TweetHandler::TweetHandler()
{}

TweetHandler::TweetHandler(Handler* next)
	: Handler(next)
{}

bool TweetHandler::canHandle(const std::string& cmd) const
{
	return cmd == "TWEET";
}

Handler::HANDLER_STATUS_T TweetHandler::process(TwitEng* eng, std::istream& instr) const
{
	string username;
	instr >> username;
	if (instr.fail())
	{
		return HANDLER_ERROR;
	}
	if (!eng->validUser(username))
	{
		return HANDLER_ERROR;
	}
	DateTime currTime;
	string tweetContent="",tempWord;
	while (instr >> tempWord)
	{
		tweetContent += tempWord;
		tweetContent += " ";
	}
	rtrim(tweetContent);
	eng->addTweet(username,currTime,tweetContent);
	return HANDLER_OK;
}
