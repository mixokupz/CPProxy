
void throw_errno(const std::string& message, bool no_errno = false) {
    throw std::system_error(no_errno ? 0 : errno, std::generic_category(), message);
}
