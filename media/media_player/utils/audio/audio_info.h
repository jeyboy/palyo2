#ifndef AUDIO_INFO_H
#define AUDIO_INFO_H

struct AudioInfo {
    AudioInfo() {

    }

    bool is_vbr;
    bool is_planar;
    int input_cnannel_count;
    int output_cnannel_count;
};

#endif // AUDIO_INFO_H
