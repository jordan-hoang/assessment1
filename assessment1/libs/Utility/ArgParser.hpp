#pragma once
// Minimal header-only command-line parser. Replaces Boost.program_options so the
// project builds with nothing but a C++17 compiler and CMake.
//
// Supports flags (--help / -h) and options taking one value
// (--query path, --query=path, -q path, -qpath). Unknown options and stray
// positional arguments are errors, matching Boost's parse_command_line behaviour.
//
//   ArgParser args("Allowed options");
//   args.flag("help", 'h', "Show help message")
//       .option("query", 'q', "Path to json you want to parse");
//   args.parse(argc, argv);          // throws std::runtime_error on bad input
//   if (args.has("query")) use(args.get("query"));

#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

class ArgParser {
public:
    explicit ArgParser(std::string caption) : caption_(std::move(caption)) {}

    ArgParser& flag(const std::string& name, char short_name, const std::string& help) {
        specs_.push_back({name, short_name, help, false});
        return *this;
    }

    ArgParser& option(const std::string& name, char short_name, const std::string& help) {
        specs_.push_back({name, short_name, help, true});
        return *this;
    }

    void parse(int argc, char* argv[]) {
        values_.clear();
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            const Spec* spec = nullptr;
            std::string value;
            bool has_inline_value = false;

            if (arg.size() > 2 && arg.compare(0, 2, "--") == 0) {
                std::string name = arg.substr(2);
                auto eq = name.find('=');
                if (eq != std::string::npos) {
                    value = name.substr(eq + 1);
                    name = name.substr(0, eq);
                    has_inline_value = true;
                }
                spec = findLong(name);
            } else if (arg.size() >= 2 && arg[0] == '-' && arg[1] != '-') {
                spec = findShort(arg[1]);
                if (arg.size() > 2) {
                    value = arg.substr(2);
                    has_inline_value = true;
                }
            } else {
                throw std::runtime_error("unexpected argument '" + arg + "'");
            }

            if (!spec) {
                throw std::runtime_error("unrecognised option '" + arg + "'");
            }
            if (spec->takes_value) {
                if (!has_inline_value) {
                    if (i + 1 >= argc) {
                        throw std::runtime_error("option '--" + spec->name + "' requires a value");
                    }
                    value = argv[++i];
                }
                values_[spec->name] = value;
            } else {
                if (has_inline_value) {
                    throw std::runtime_error("option '--" + spec->name + "' does not take a value");
                }
                values_[spec->name] = "";
            }
        }
    }

    bool has(const std::string& name) const { return values_.count(name) != 0; }

    std::string get(const std::string& name) const {
        auto it = values_.find(name);
        if (it == values_.end()) {
            throw std::runtime_error("option '--" + name + "' was not given");
        }
        return it->second;
    }

    std::string help() const {
        std::ostringstream out;
        out << caption_ << ":\n";
        for (const auto& s : specs_) {
            std::string left = "  ";
            if (s.short_name) left += std::string("-") + s.short_name + " [ --" + s.name + " ]";
            else left += "--" + s.name;
            if (s.takes_value) left += " arg";
            out << left;
            if (left.size() < 32) out << std::string(32 - left.size(), ' ');
            else out << "  ";
            out << s.help << "\n";
        }
        return out.str();
    }

private:
    struct Spec {
        std::string name;
        char short_name;
        std::string help;
        bool takes_value;
    };

    const Spec* findLong(const std::string& name) const {
        for (const auto& s : specs_) if (s.name == name) return &s;
        return nullptr;
    }

    const Spec* findShort(char c) const {
        for (const auto& s : specs_) if (s.short_name == c) return &s;
        return nullptr;
    }

    std::string caption_;
    std::vector<Spec> specs_;
    std::map<std::string, std::string> values_;
};
