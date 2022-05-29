#pragma once

#define MAX_CHANNEL				20
#define PORT					6667
#define SERVER_NAME				"sehan.irc"
#define MAX_THREAD				10
#define PASSWORD				"1234"
#define VERSION					"1.0"

#define RPL_WELCOME				"001"
#define RPL_YOURHOST			"002"
#define RPL_CREATED				"003"
#define RPL_MYINFO				"004"
#define RPL_USERHOST			"302"
#define RPL_WHOISUSER			"311"
#define RPL_WHOISSERVER			"312"
#define RPL_WHOWASUSER			"314"
#define RPL_ENDOFWHOIS			"318"
#define RPL_WHOISCHANNELS		"319"
#define RPL_LIST				"322"
#define RPL_LISTEND				"323"
#define RPL_NOTOPIC				"331"
#define RPL_TOPIC				"332"
#define RPL_INVITING			"341"
#define RPL_NAMREPLY			"353"
#define RPL_ENDOFNAMES			"366"
#define RPL_ENDOFWHOWAS			"369"
#define RPL_TIME				"391"

#define ERR_NOSUCHNICK			"401"
#define ERR_NOSUCHCHANNEL		"403"
#define ERR_CANNOTSENDTOCHAN	"404"
#define ERR_TOOMANYCHANNELS		"405"
#define ERR_WASNOSUCHNICK		"406"

#define ERR_NORECIPIENT			"411"
#define ERR_NOTEXTTOSEND		"412"
#define ERR_NONICKNAMEGIVEN		"431"
#define ERR_ERRONEOUSNICKNAME	"432"
#define ERR_NICKNAMEINUSE		"433"
#define ERR_USERNOTINCHANNEL	"441"
#define ERR_NOTONCHANNEL		"442"
#define ERR_USERONCHANNEL		"443"

#define ERR_NEEDMOREPARAMS		"461"
#define	ERR_ALREADYREGISTER		"462"
#define ERR_CHANOPRIVSNEEDED	"482"



//SendMsg(":" + this->mData.GetServName() + " " + ERR_NONICKNAMEGIVEN + " " + this->mData.GetUser()[sock].GetNick() + " :No nickname given", sock);