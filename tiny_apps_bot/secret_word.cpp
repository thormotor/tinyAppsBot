#include "secret_word.h"

#include <random>
#include <tuple>
#include <stdexcept>


namespace tiny_apps {
namespace app_game_bulls_cows {

static const size_t kMaxLength = 6;  // max size of secret word


SecretWord::SecretWord(size_t length, const char first_char, const char last_char) 
		: first_char_(first_char), last_char_(last_char), length_(length)
{
	word_ = GetRandomSequence();
	CheckCorrectness(word_);
}


SecretWord::SecretWord(const std::string& word, const char first_char, const char last_char)
		: length_(word.size()), first_char_(first_char), last_char_(last_char)
{
	word_ = word;
	CheckCorrectness(word_);
}


void SecretWord::CheckCorrectness(const std::string& word) const {
	int possible_length = last_char_ - first_char_ + 1;
	if (word.length() != length_)
		throw std::length_error(std::string("Invalid length. Possible value: ") + std::to_string(length_) + ".");
	if (length_ < 2 || length_ > kMaxLength)
		throw std::length_error(std::string("Invalid length. Possible values: [2-") + std::to_string(kMaxLength) + "].");
	if (possible_length < 2 || possible_length < (int)length_)
		throw std::range_error("Invalid interval boundaries.");
	for (size_t i = 0; i < length_; ++i) {
		if (word.at(i) < first_char_ || word.at(i) > last_char_)
			throw std::range_error(std::string("Symbol ") + word.at(i) + " is not in the interval boundaries.");
		if (word.rfind(word.at(i)) != i)
			throw std::invalid_argument(std::string("Symbol ") + word.at(i) + " is duplicated in the sequence.");
	}
}

std::string SecretWord::GetRandomSequence() const {
	//initialize random generator
	std::uniform_int_distribution<int> distribution(first_char_, last_char_);
	std::random_device rd;
	std::mt19937 engine(rd());
	//generate random sequence
	std::string word;
	for (size_t i = 0; i < length_; i++) {
		char rand_char;
		do {
			rand_char = distribution(engine);
		} while (word.find(rand_char) != std::string::npos);
		word.push_back(rand_char);
	}
	return word;
}


bool SecretWord::operator==(const SecretWord& secret_word) const {
	return this->word_ == secret_word.word_;
}


std::string SecretWord::operator()() const {
	return word_;
}


std::tuple<int, int, std::string> SecretWord::checkGuess(const std::string& guess) const {
	try	{
		CheckCorrectness(guess);
	} catch (const std::exception& err) {
		return std::make_tuple(-1, -1, err.what());
	}
	int bulls = 0;
	int cows = 0;
	for (size_t i = 0; i < length_; ++i) {
		cows += std::count(guess.begin(), guess.end(), word_.at(i));
		if (word_.at(i) == guess.at(i)) {
			--cows;
			++bulls;
		}
	}
	return std::make_tuple(bulls, cows, "ok");
}

size_t SecretWord::Length() const {
	return length_;
}


}  // namespace app_bulls_cows
}  // namespace tiny_apps