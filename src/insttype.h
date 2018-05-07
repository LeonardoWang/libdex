#pragma once

/** @file
 *  Deal with instruction types.
 **/

#include <cstdint>
#include <stdexcept>

/**
 *  Representing Instruction's type
 *  For example, both `add-int v1, v2, v3` and `add-int v2, v5, v1` have the same type `add-int`.
 *
 *  Use `inst_types[op]` to get an InstType, do not create InstType objects after initialization.
 **/
class InstType {
public:
    struct Getter;

    /** Human-friendly form of op **/
    const char * mnemonic;
    /** Human-friendly form of oprands' syntax **/
    const char * syntax;

    /**
     *  Getter of oprands and length
     *  Users should use not this directly.
     *  Use member functions of `Inst` instead.
     **/
    const Getter * get;


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

    /** The constructor is for initializer only; users should not attempt to create `InstType` objects by themselves **/
    InstType(const char * mnemonic, int format, const char * syntax = nullptr)
        : mnemonic(mnemonic), syntax(syntax), get(getters[format])
    { }

private:
    static const Getter * getters[];
};

/** A list of all instruction types, indexed by op code **/
extern const InstType inst_types[];
