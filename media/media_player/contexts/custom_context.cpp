#include "custom_context.h"

int read_packet(void *opaque, uint8_t *buf, int buf_size) {
    WebObject * obj = (WebObject *)opaque;
    return obj -> read(buf, buf_size);
}
//int write_packet(void *opaque, uint8_t *buf, int buf_size) {}
int64_t seek_stream(void *opaque, int64_t offset, int whence) {
    WebObject * obj = (WebObject *)opaque;

    switch (whence) {
        case AVSEEK_SIZE: { return obj -> lenght(); }
        case SEEK_SET: { return obj -> seek(offset); }
        case SEEK_CUR: { return obj -> seek(obj -> pos() + offset); }
        case SEEK_END: { return obj -> seek(obj -> lenght() - offset); }
        default: {
            qDebug("Some troubles (O_o)");
        }
    }

    return offset;
}

CustomContext::CustomContext(AVFormatContext *context, QUrl & url) {
    obj = new WebObject(0, url);

    if (!obj -> openSync()) {
        errorStr = obj -> lastError();
        delete obj;
        valid = false;
        return;
    }

    size_t avio_ctx_buffer_size = 4096;

    if (!(context = avformat_alloc_context())) {
        errorStr = "Did not create context";
        valid = false;
        return;
    }

    avio_ctx_buffer = (uint8_t *)av_malloc(avio_ctx_buffer_size);
    if (!avio_ctx_buffer) {
        errorStr = "Did not create buffer";
        valid = false;
        return;
    }

    avio_context = avio_alloc_context(avio_ctx_buffer, avio_ctx_buffer_size, 0, obj, &read_packet, NULL, &seek_stream);
    if (!avio_context) {
        errorStr = "Did not create custom context";
        valid = false;
        return;
    }

    context -> pb = avio_context;
    context -> flags = AVFMT_FLAG_CUSTOM_IO | AVFMT_FLAG_NONBLOCK;

    if (avformat_open_input(&context, NULL, NULL, NULL) < 0) {
        errorStr = "Could not open input";
        valid = false;
        return;
    }

    if (avformat_find_stream_info(context, NULL) < 0) {
        errorStr = "Did not find stream info";
        valid = false;
        return;
    }

    av_dump_format(context,
                   0,
                   (url.isLocalFile() ? url.toLocalFile() : url.toString()).toUtf8().data(),
                   false);

    valid = true;
}

CustomContext::~CustomContext() {
    if (avio_context) {
        av_freep(&avio_context -> buffer);
        av_freep(&avio_context);
    }

    delete obj;
}

bool CustomContext::isValid() const {
    return valid;
}

QString CustomContext::lastError() const {
    return errorStr;
}
