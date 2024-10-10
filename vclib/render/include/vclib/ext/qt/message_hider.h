#ifndef VCL_EXT_QT_MESSAGE_HIDER_H
#define VCL_EXT_QT_MESSAGE_HIDER_H

#include <QtMessageHandler>

namespace vcl::qt {

class MessageHider
{
    inline static QtMessageHandler originalHandler = nullptr;
    inline static bool             debug           = true;
    inline static bool             info            = false;
    inline static bool             warning         = false;
    inline static bool             critical        = true;
    inline static bool             fatal           = true;

    static void message(
        QtMsgType                 type,
        const QMessageLogContext& context,
        const QString&            msg)
    {
        switch (type) {
        case QtMsgType::QtDebugMsg:
            if (debug)
                originalHandler(type, context, msg);
            break;
        case QtMsgType::QtInfoMsg:
            if (info)
                originalHandler(type, context, msg);
            break;
        case QtMsgType::QtWarningMsg:
            if (warning)
                originalHandler(type, context, msg);
            break;
        case QtMsgType::QtCriticalMsg:
            if (critical)
                originalHandler(type, context, msg);
            break;
        case QtMsgType::QtFatalMsg:
            if (fatal)
                originalHandler(type, context, msg);
            break;
        }
    }

public:
    static void activate()
    {
        originalHandler = qInstallMessageHandler(message);
    }

    static void deactivate() { qInstallMessageHandler(originalHandler); }

    static void hideMessages(
        bool debug,
        bool info,
        bool warning,
        bool critical,
        bool fatal)
    {
        MessageHider::debug    = debug;
        MessageHider::info     = info;
        MessageHider::warning  = warning;
        MessageHider::critical = critical;
        MessageHider::fatal    = fatal;
    }
};

} // namespace vcl::qt

#endif // VCL_EXT_QT_MESSAGE_HIDER_H
