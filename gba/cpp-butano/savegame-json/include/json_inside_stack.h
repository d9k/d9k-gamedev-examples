#ifndef JSON_INSIDE_STACK_H
#define JSON_INSIDE_STACK_H

#include "bn_vector.h"
#include "bn_string.h"

typedef bn::string<128> JsonInsideStackToString;

class JsonInsideStack {
public:
    static constexpr const bool INSIDE_ARRAY = true;
    static constexpr const bool INSIDE_OBJECT = false;

    bn::vector<bool, 32> _inside_stack;

    void add(bool inside_what) {
        _inside_stack.push_back(inside_what);
    }

    void pop() {
        _inside_stack.pop_back();
    }

    int level() {
        return _inside_stack.size();
    }

    JsonInsideStackToString to_string() {
        JsonInsideStackToString result;
        bn::ostringstream result_ss(result);
        int last_item = _inside_stack.size() - 1;
        for (int i = 0; i <= last_item; i++) {
            result_ss << (_inside_stack[i] == INSIDE_ARRAY ?  "arr" : "obj");
            result_ss << (i != last_item ? " > " : "");
        }
        return result;
    }

    JsonInsideStackToString debug_string() {
        JsonInsideStackToString result;
        bn::ostringstream result_ss(result);
        result_ss << "lvl " << this->level() << ": " << this->to_string();
        return result;
    }
};

#endif // JSON_INSIDE_STACK_H