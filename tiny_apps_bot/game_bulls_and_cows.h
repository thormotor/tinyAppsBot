#ifndef GAME_BULLS_AND_COWS_H
#define GAME_BULLS_AND_COWS_H

#include <string>
#include <map>
#include <memory>

#include "boost/property_tree/ptree.hpp"

#include "secret_word.h"
#include "i_tiny_app.h"
#include "i_reply_helper.h"


namespace tiny_apps {
/** @brief Contains classes for tiny application Bulls&Cows. It's a simple game. */
namespace app_game_bulls_cows {

class GameBullsAndCows;
using handlers::output::IReplyHelperPtr;
using CommandMethodPtr = void(GameBullsAndCows::*)();


/** @brief Provides application logic and incoming messages handling. */
class GameBullsAndCows : public ITinyApp {
public:
	explicit GameBullsAndCows(IReplyHelperPtr reply_helper);
	explicit GameBullsAndCows(IReplyHelperPtr reply_helper, const std::string& word);
	~GameBullsAndCows() = default;

	void Turn(const std::string& message) override;
private:
	// --- command handlers initializer ---
	void InitCommandHandlers();
	// --- command handlers methods ---
	void CommandRestart();
	void CommandHelp();
	// --------------------------------
	void HandleGess(const std::string& message);
	void NewGame();

	SecretWord secret_word_;
	size_t number_of_atempts_ = 0;
	IReplyHelperPtr reply_helper_;
	std::map<std::string, CommandMethodPtr> command_handlers_;
};


}  // namespace app_game_bulls_cows
}  // namespace tiny_apps

#endif  // GAME_BULLS_AND_COWS_H