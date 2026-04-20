#ifndef COMMON_COMPILER_ERROR_H
#define COMMON_COMPILER_ERROR_H

#include <stdexcept>
#include <string>
#include <utility>

class CompilerError : public std::runtime_error {
public:
    CompilerError(std::string category, int line, std::string message)
        : std::runtime_error(buildMessage(category, line, message)),
          category_(std::move(category)),
          line_(line),
          detail_(std::move(message)) {}

    const std::string& category() const noexcept { return category_; }
    int line() const noexcept { return line_; }
    const std::string& detail() const noexcept { return detail_; }

private:
    static std::string buildMessage(const std::string& category, int line, const std::string& message) {
        return "ERRO " + category + " na linha " + std::to_string(line) + ": " + message;
    }

    std::string category_;
    int line_;
    std::string detail_;
};

#endif
