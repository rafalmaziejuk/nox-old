#include "window/linux/linux_window.h"
#include "window/linux/linux_window_helper.h"

#include <X11/Xos.h>
#include <X11/Xutil.h>

namespace NOX {

std::unique_ptr<Window> Window::create(const WindowDescriptor &descriptor) {
    return std::make_unique<LinuxWindow>(descriptor);
}

LinuxWindow::LinuxWindow(const WindowDescriptor &descriptor) : m_descriptor{descriptor} {
    createWindow();
    initializeWindowSpecification();
}

LinuxWindow::~LinuxWindow() {
    XDestroyWindow(m_display, m_window);
    XCloseDisplay(m_display);

    m_display = nullptr;
    NOX_LOG_TRACE(WINDOW, "Destroyed [{}] window", m_descriptor.title);
}

void LinuxWindow::createWindow() {
    m_display = XOpenDisplay(static_cast<char *>(nullptr));
    NOX_ASSERT_MSG(m_display == nullptr, "Unable to connect to X11 display");

    ::Screen *screen = DefaultScreenOfDisplay(m_display); // NOLINT
    const int32_t screenId = DefaultScreen(m_display);
    const int32_t depth = DefaultDepth(m_display, screenId);
    ::Visual *visualType = DefaultVisual(m_display, screenId);

    XSetWindowAttributes attributes;
    attributes.colormap = XCreateColormap(m_display, DefaultRootWindow(m_display), visualType, AllocNone);
    attributes.event_mask = (KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | ResizeRedirectMask);
    attributes.border_pixel = 0;
    attributes.background_pixel = BlackPixel(m_display, screenId);
    attributes.override_redirect = m_descriptor.isBorderless;

    const Vector2D<int32_t> windowPosition = LinuxWindowHelper::getWindowPosition(m_descriptor, m_display);

    m_window = XCreateWindow(
        m_display,
        RootWindowOfScreen(screen),
        windowPosition.x(), windowPosition.y(),
        m_descriptor.size.x(), m_descriptor.size.y(),
        0,
        depth,
        InputOutput,
        visualType,
        CWEventMask | CWOverrideRedirect | CWColormap,
        (&attributes));
    NOX_LOG_INFO(WINDOW, "Created window (title: [{}], size: [{} x {}], position: [{} x {}])", m_descriptor.title, m_descriptor.size.x(), m_descriptor.size.y(), windowPosition.x(), windowPosition.y());
}

void LinuxWindow::initializeWindowSpecification() {
    XStoreName(m_display, m_window, m_descriptor.title.c_str());
    XSetWindowBackground(m_display, m_window, BlackPixel(m_display, DefaultScreen(m_display)));

    if (m_descriptor.isBorderless) {
        XGrabKeyboard(m_display, m_window, True, GrabModeAsync, GrabModeAsync, CurrentTime);
        XGrabPointer(m_display, m_window, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, m_window, None, CurrentTime);
    }

    if (!m_descriptor.isResizable) {
        XSizeHints sizeHints;
        sizeHints.flags = PMinSize | PMaxSize;
        sizeHints.max_width = sizeHints.min_width = m_descriptor.size.x();
        sizeHints.max_height = sizeHints.min_height = m_descriptor.size.y();

        XSetWMNormalHints(m_display, m_window, &sizeHints);
    }

    if (m_descriptor.isVisible) {
        show();
    }
}

void LinuxWindow::show() const {
    XClearWindow(m_display, m_window);

    XMapWindow(m_display, m_window);

    if (m_descriptor.isBorderless) {
        XSetInputFocus(m_display, m_window, RevertToParent, CurrentTime);
    }

    m_closeWindowAtom = XInternAtom(m_display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(m_display, m_window, &m_closeWindowAtom, 1);

    NOX_LOG_TRACE(WINDOW, "Shown window");
}

void LinuxWindow::processEvents() const {
    XEvent event;

    XPending(m_display);

    while (XQLength(m_display)) {
        XNextEvent(m_display, &event);
        postEvent(event);
    }
}

void LinuxWindow::postEvent(XEvent const &event) const {
    if ((event.type == ClientMessage) && (event.xclient.data.l[0] == static_cast<int32_t>(m_closeWindowAtom))) {
        postCloseEvent();
    } else if (event.type == ResizeRequest) {
        postResizeEvent(event.xresizerequest.width, event.xresizerequest.height);
    }
}

void LinuxWindow::postCloseEvent() const {
    onCloseEvent();
    NOX_LOG_TRACE(WINDOW, "Close event posted");
}

void LinuxWindow::postResizeEvent(uint32_t width, uint32_t height) const {
    onResizeEvent(width, height);
    NOX_LOG_TRACE(WINDOW, "Resize event posted");
}

void *LinuxWindow::getNativeHandle() const {
    return m_display;
}

Vector2D<uint32_t> LinuxWindow::getSize() const {
    return m_descriptor.size;
}

::Window LinuxWindow::getWindow() const {
    return m_window;
}

} // namespace NOX
