#ifndef OVERLOAD_HPP
#define OVERLOAD_HPP
#include <utility>

template <class ...Fs>
struct overload : Fs... {
    overload(const Fs& ...ts) : Fs{ts}... {}
    overload(Fs&&... ts) : Fs{ts}... {}

    using Fs::operator()...;
};

template <class ...Ts>
overload(Ts&&...) -> overload<std::decay_t<Ts>...>;

#endif // OVERLOAD_HPP
