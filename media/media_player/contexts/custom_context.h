#ifndef CUSTOM_CONTEXT_H
#define CUSTOM_CONTEXT_H

#include "media/media_player/utils/media_player_utils.h"
#include "../utils/web/web_object.h"
#include <qurl.h>

class CustomContext {
public:
    CustomContext(AVFormatContext *context, QUrl & url);
    ~CustomContext();

    bool isValid() const;
    QString lastError() const;
private:
    QString errorStr;
    bool valid;
    AVIOContext *avio_context;
    uint8_t * avio_ctx_buffer;
    WebObject * obj;
};

#endif // CUSTOM_CONTEXT_H
