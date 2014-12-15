#ifndef COLOR_CONVERSION_H
#define COLOR_CONVERSION_H

#include <qmath.h>
#include <QMatrix4x4>

static const QMatrix4x4 kGBR2RGB =
           QMatrix4x4(0, 0, 1, 0,
                      1, 0, 0, 0,
                      0, 1, 0, 0,
                      0, 0, 0, 1);

static const QMatrix4x4 yuv2rgb_bt601 =
           QMatrix4x4(
                1.0f,  0.000f,  1.402f, 0.0f,
                1.0f, -0.344f, -0.714f, 0.0f,
                1.0f,  1.772f,  0.000f, 0.0f,
                0.0f,  0.000f,  0.000f, 1.0f)
            *
            QMatrix4x4(
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, -0.5f,
                0.0f, 0.0f, 1.0f, -0.5f,
                0.0f, 0.0f, 0.0f, 1.0f);

static const QMatrix4x4 yuv2rgb_bt709 =
           QMatrix4x4(
                1.0f,  0.000f,  1.5701f, 0.0f,
                1.0f, -0.187f, -0.4664f, 0.0f,
                1.0f,  1.8556f, 0.000f,  0.0f,
                0.0f,  0.000f,  0.000f,  1.0f)
            *
            QMatrix4x4(
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, -0.5f,
                0.0f, 0.0f, 1.0f, -0.5f,
                0.0f, 0.0f, 0.0f, 1.0f);



class ColorConversion {
public:
    enum ColorSpace {
        RGB,
        GBR,
        BT601,
        BT709
    };

    ColorConversion(ColorSpace source, ColorSpace destination) :
        hue(0)
        , saturation(0)
        , contrast(0)
        , brightness(0)
        , in(source)
        , out(destination)
    {
        compute();
    }

    QMatrix4x4 matrix() const { return M; }
    const QMatrix4x4& matrixRef() const { return M; }
//    template<typename T> void matrixData(T * M) const {
//        const QMatrix4x4 &m = matrixRef();
//        M[0] = m(0,0), M[4] = m(0,1), M[8] = m(0,2), M[12] = m(0,3),
//        M[1] = m(1,0), M[5] = m(1,1), M[9] = m(1,2), M[13] = m(1,3),
//        M[2] = m(2,0), M[6] = m(2,1), M[10] = m(2,2), M[14] = m(2,3),
//        M[3] = m(3,0), M[7] = m(3,1), M[11] = m(3,2), M[15] = m(3,3);
//    }

    ColorSpace getSourceSpace() const { return in; }
    void setSourceSpace(ColorSpace source) {
        if (in == source) return;
        in = source;
        compute();
    }

    ColorSpace getDestinationSpace() const { return out; }
    void setDestinationSpace(ColorSpace destination) {
        if (out == destination) return;
        out = destination;
        compute();
    }

    void setBrightness(qreal brightness) {
        if (this -> brightness == brightness) return;
        this -> brightness = brightness;
        compute();
    }
    qreal getBrightness() const { return brightness; }

    // -1~1
    void setHue(qreal hue) {
        if (this -> hue == hue) return;
        this -> hue = hue;
        compute();
    }

    qreal getHue() const { return hue; }

    void setContrast(qreal contrast) {
        if (this -> contrast == contrast) return;
        this -> contrast = contrast;
        compute();
    }
    qreal getContrast() const { return contrast; }

    void setSaturation(qreal saturation) {
        if (this -> saturation == saturation) return;
        this -> saturation = saturation;
        compute();
    }
    qreal getSaturation() const { return saturation; }

protected:
    const QMatrix4x4& YUV2RGB(ColorSpace cs) {
        switch (cs) {
            case BT709:
                return yuv2rgb_bt709;
            default:
                return yuv2rgb_bt601;
        }
    }

    void compute() {
        //http://docs.rainmeter.net/tips/colormatrix-guide
        //http://www.graficaobscura.com/matrix/index.html
        //http://beesbuzz.biz/code/hsv_color_transforms.php

        M = QMatrix4x4(  1, 0, 0, brightness,
                         0, 1, 0, brightness,
                         0, 0, 1, brightness,
                         0, 0, 0, 1);


        if (contrast != 0) {
            float c = contrast + 1.0;
            M *= QMatrix4x4( c, 0, 0, 0,
                             0, c, 0, 0,
                             0, 0, c, 0,
                             0, 0, 0, 1);
        }

        if (saturation != 0) {
            const float wr = .3086f;  //lumR = 0.3086  or  0.2125
            const float wg = .6094f;  //lumG = 0.6094  or  0.7154
            const float wb = .0820f;  //lumB = 0.0820  or  0.0721
            float s = saturation + 1.0f;
            M *= QMatrix4x4(
                (1.0f - s) * wr + s, (1.0f - s) * wg    , (1.0f - s) * wb    , 0.0f,
                (1.0f - s) * wr    , (1.0f - s) * wg + s, (1.0f - s) * wb    , 0.0f,
                (1.0f - s) * wr    , (1.0f - s) * wg    , (1.0f - s) * wb + s, 0.0f,
                               0.0f,                0.0f,                0.0f, 1.0f
            );
        }

        if (hue != 0) {
            // Hue
            const float n = 1.0f / sqrtf(3.0f);       // normalized hue rotation axis: sqrt(3)*(1 1 1)
            const float h = hue * M_PI;               // hue rotation angle
            const float hc = cosf(h);
            const float hs = sinf(h);
            const float nn = n * n;
            const float rel = nn *(1.0f - hc);

            M *= QMatrix4x4(     // conversion of angle/axis representation to matrix representation
                rel + hc       , rel - n * hs , rel + n * hs , 0.0f,
                rel + n * hs   , rel + hc     , rel - n * hs , 0.0f,
                rel - n * hs   , rel + n * hs , rel + hc     , 0.0f,
                        0.0f   ,         0.0f ,         0.0f , 1.0f
            );

    //        QMatrix4x4 H(     // conversion of angle/axis representation to matrix representation
    //            nn * (1.0f - hc) + hc       , nn * (1.0f - hc) - n * hs , nn * (1.0f - hc) + n * hs , 0.0f,
    //            nn * (1.0f - hc) + n * hs   , nn * (1.0f - hc) + hc     , nn * (1.0f - hc) - n * hs , 0.0f,
    //            nn * (1.0f - hc) - n * hs   , nn * (1.0f - hc) + n * hs , nn * (1.0f - hc) + hc     , 0.0f,
    //                                 0.0f   ,                      0.0f ,                      0.0f , 1.0f
    //        );
        }

        switch (in) {
            case ColorConversion::RGB:
                break;
            case ColorConversion::GBR:
                M *= kGBR2RGB;
                break;
            default:
                M *= YUV2RGB(in);
                break;
        }

        switch (out) {
            case ColorConversion::RGB:
                break;
            case ColorConversion::GBR:
                M = kGBR2RGB.inverted() * M;
                break;
            default:
                M = YUV2RGB(out).inverted() * M;
                break;
        }
    }
private:
    ColorSpace in, out;
    qreal hue;
    qreal saturation;
    qreal contrast;
    qreal brightness;
    mutable QMatrix4x4 M;
};

#endif // COLOR_CONVERSION_H
