namespace nox {

namespace {

bool debugBreakEnabled = true;

}

void setDebugBreakEnabled(bool value) {
    debugBreakEnabled = value;
}

bool isDebugBreakEnabled() {
    return debugBreakEnabled;
}

} // namespace nox
