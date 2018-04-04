#pragma once

#include <cstdint>
#include <stdexcept>

class InstType {
public:
    struct Getter;

    const char * mnemonic;
    const Getter * get;
    const char * syntax = nullptr;


    struct Getter {
        virtual uint64_t a([[maybe_unused]] const uint8_t * data) const { throw std::runtime_error("Cannot get A"); }
        virtual uint64_t b([[maybe_unused]] const uint8_t * data) const { throw std::runtime_error("Cannot get B"); }
        virtual uint64_t c([[maybe_unused]] const uint8_t * data) const { throw std::runtime_error("Cannot get C"); }
        virtual uint64_t d([[maybe_unused]] const uint8_t * data) const { throw std::runtime_error("Cannot get D"); }
        virtual uint64_t e([[maybe_unused]] const uint8_t * data) const { throw std::runtime_error("Cannot get E"); }
        virtual uint64_t f([[maybe_unused]] const uint8_t * data) const { throw std::runtime_error("Cannot get F"); }
        virtual uint64_t g([[maybe_unused]] const uint8_t * data) const { throw std::runtime_error("Cannot get G"); }
        virtual uint64_t h([[maybe_unused]] const uint8_t * data) const { throw std::runtime_error("Cannot get H"); }
        virtual int length() const = 0;
    };

    InstType(const char * mnemonic, int format, const char * syntax = nullptr)
        : mnemonic(mnemonic), get(getters[format]), syntax(syntax)
    { }

private:
    static const Getter * getters[];
};

extern const InstType inst_types[];
