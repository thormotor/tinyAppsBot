#ifndef I_TINY_APP_H
#define I_TINY_APP_H

#include <string>


namespace tiny_apps {


/** @brief Declares interface for all Tiny Application classes. */
class ITinyApp {
public:
	ITinyApp() = default;
	virtual ~ITinyApp() = default;
	ITinyApp(const ITinyApp&) = delete;
	ITinyApp& operator=(const ITinyApp&) = delete;
	ITinyApp(ITinyApp&&) = delete;
	ITinyApp& operator=(ITinyApp&&) = delete;

	virtual void Turn(const std::string& message) = 0;
};

using ITinyAppPtr = std::unique_ptr<ITinyApp>;


}  // tiny_apps

#endif  // I_TINY_APP_H