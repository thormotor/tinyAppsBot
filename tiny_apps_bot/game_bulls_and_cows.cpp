#include "game_bulls_and_cows.h"

#include <thread>
#define FMT_HEADER_ONLY
#include "fmt/format.h"


namespace tiny_apps {
namespace app_game_bulls_cows {


GameBullsAndCows::GameBullsAndCows(IReplyHelperPtr reply_helper)
		: reply_helper_(reply_helper)
{
	InitCommandHandlers();
	NewGame();
}


GameBullsAndCows::GameBullsAndCows(IReplyHelperPtr reply_helper, const std::string& secret)
		: secret_word_(secret)
{
	InitCommandHandlers();
}


void GameBullsAndCows::InitCommandHandlers() {
	// here is the list of commands
	command_handlers_.emplace("restart", &GameBullsAndCows::CommandRestart);
	command_handlers_.emplace("??", &GameBullsAndCows::CommandHelp);
}
	

void GameBullsAndCows::NewGame() {
	if (number_of_atempts_ > 0)
		secret_word_ = SecretWord(secret_word_.Length());
	number_of_atempts_ = 0;
	reply_helper_->SendTextMessage(fmt::format(u8"* BULLS & COWS game *\n\nGuess a {}-digit secret number. You have only 10 attempts.\nUse ?? for help.", secret_word_.Length()));
}


void GameBullsAndCows::Turn(const std::string& message) {
	auto it = command_handlers_.find(message);
	if (it != command_handlers_.end()) {
		CommandMethodPtr handler_ptr = command_handlers_.at(message);
		(this->*handler_ptr)();  // handle command
	} else {
		HandleGess(message);
	}
}


void GameBullsAndCows::HandleGess(const std::string & message) {
	using namespace std::chrono_literals;
	auto answer = secret_word_.checkGuess(message);
	if (std::get<0>(answer) != -1) {  // get<0> - bulls, get<1> - cows; "-1" - is an error
		if (std::get<0>(answer) == secret_word_.Length()) {
			reply_helper_->SendTextMessage(fmt::format(u8"You've WON!!! (ﾉ◕ヮ◕)ﾉ :･ﾟ✧\nattempts: {}", number_of_atempts_ + 1));
			std::this_thread::sleep_for(2s);
			NewGame();
		} else if (number_of_atempts_ >= 9) {
			reply_helper_->SendTextMessage(fmt::format(u8"You've LOST!!! ¯\\(°_o)/¯\nnumber was {} ", secret_word_()));
			std::this_thread::sleep_for(2s);
			NewGame();
		} else {
			++number_of_atempts_;
			reply_helper_->SendTextMessage(fmt::format(u8"{})   bulls_{}  cows_{}", number_of_atempts_, std::get<0>(answer), std::get<1>(answer)));
		}
	} else {
		reply_helper_->SendTextMessage(std::get<2>(answer));  // we have an error, third parameter explains it
	}
}


void GameBullsAndCows::CommandRestart() {
	NewGame();
}

void GameBullsAndCows::CommandHelp()
{
	const std::string help_info{\
		"The goal of the game is to uncover the bot's secret number with a minimal number of questions. \
		The bot indicates the number of matches in your proposition. \
		Rules:\
		All digits in the secret number are different.\
		If your try has matching digits on the exact places, they are Bulls.\
		If you have digits from the secret number, but not on the right places, they are Cows.\
		In the answers bot will say the number of bull & cows that match. \
		http://theteacher.info/index.php/algorithms-and-problem-solving-2/computational-thinking/practical-tasks/4364-bulls-and-cows \
		\n\n*To leave this game just send me 'stop'."\
	};

	reply_helper_->SendTextMessage(help_info);
}


}  // namespace app_game_bulls_cows
}  // namespace tiny_apps
