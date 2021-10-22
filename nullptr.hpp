#pragma once

namespace ft {

    static struct nullptr_t {
        template<class T>
        operator T*() const { return 0; }

        template<class T, class U>
        operator T U::*() const { return 0; }

        friend bool operator==(nullptr_t, nullptr_t) { return true; }
        friend bool operator!=(nullptr_t, nullptr_t) { return false; }
    } nullptr_v;

}
