#ifndef SECRET_WORD_H
#define SECRET_WORD_H

#include <string>


namespace tiny_apps {
namespace app_game_bulls_cows {

/** @brief Provides manage for the randomly generated number. Checks user's guesses and returns result. */
class SecretWord {
 public:
	SecretWord(size_t length = 4, const char first_char = '0', const char last_char = '9');
	SecretWord(const std::string& word, const char first_char = '0', const char last_char = '9');
	~SecretWord() = default;
	SecretWord(const SecretWord&) = default;
	SecretWord(SecretWord&&) = default;
	SecretWord& operator=(const SecretWord&) = default;
	SecretWord& operator=(SecretWord&&) = default;
	bool operator==(const SecretWord& secret_word) const;
	std::string operator()() const;
	std::tuple<int, int, std::string> checkGuess(const std::string& guess) const;
	size_t Length() const;
 private:
	void CheckCorrectness(const std::string& word) const;
	std::string GetRandomSequence() const;

	std::string word_;
	char first_char_;
	char last_char_;
	size_t length_;
};


}  // namespace app_bulls_cows
}  // namespace tiny_apps

#endif  // SECRET_WORD_H