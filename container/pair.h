#pragma once

namespace xxds
{
    template<typename A, typename B>
    class pair
    {
        A ft;
        B sd;

        using self = pair<A, B>;
    public:
        pair() {}
        pair(const A &a, const B &b): ft(a), sd(b) {}
        bool operator == (const self &rhs) const {
            return first() == rhs.first() && second() == rhs.second();
        }
        bool operator < (const self &rhs) const {
            if(first() == rhs.first()) {
                return second() < rhs.second();
            }
            else {
                return first() < rhs.first();
            }
        }
        bool operator != (const self &rhs) const {
            return !operator==(rhs);
        }
        A & first() {
            return ft;
        }
        B & second() {
            return sd;
        }
        const A& first() const {
            return ft;
        }
        const B& second() const {
            return sd;
        }
    };
}