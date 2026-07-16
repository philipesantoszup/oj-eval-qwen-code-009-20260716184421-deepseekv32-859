#ifndef SJTU_EXCEPTIONS_HPP
#define SJTU_EXCEPTIONS_HPP

#include <cstddef>
#include <cstring>
#include <string>

namespace sjtu {

class exception {
   protected:
    std::string variant = "";
    std::string detail = "";

   public:
    exception() {
    }
    exception(const exception &ec) : variant(ec.variant), detail(ec.detail) {
    }
    virtual std::string what() {
        return variant + " " + detail;
    }
};

class index_out_of_bound : public exception {
public:
    index_out_of_bound() : exception() {
        variant = "index_out_of_bound";
    }
    index_out_of_bound(const std::string &detail) : exception() {
        variant = "index_out_of_bound";
        this->detail = detail;
    }
};

class runtime_error : public exception {
public:
    runtime_error() : exception() {
        variant = "runtime_error";
    }
    runtime_error(const std::string &detail) : exception() {
        variant = "runtime_error";
        this->detail = detail;
    }
};

class invalid_iterator : public exception {
public:
    invalid_iterator() : exception() {
        variant = "invalid_iterator";
    }
    invalid_iterator(const std::string &detail) : exception() {
        variant = "invalid_iterator";
        this->detail = detail;
    }
};

class container_is_empty : public exception {
public:
    container_is_empty() : exception() {
        variant = "container_is_empty";
    }
    container_is_empty(const std::string &detail) : exception() {
        variant = "container_is_empty";
        this->detail = detail;
    }
};
}  // namespace sjtu

#endif
